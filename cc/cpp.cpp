#include "cpp.h"

#include "options.h"
#include "string-pool.h"
#include "os/c.h"
#include "os/os.h"
#include "util/hashtable.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"

struct Context {
    CppOutput* output;
    ProgramOptions* options;

    StringPool strings;
    Hashmap<StringView, StringView> defines;
    Vector<String> currentFile;

    // Number of #if, #ifdef, #ifndef directives we are inside.
    uint32_t ifs;

    // Bitmask. If bit 0 is set that means the highest-level #if, #ifdef, or
    // #ifndef directive test failed. Bit 1 means the second-highest, and so on.
    uint32_t failedIfs;
};

enum IncludeType {
    ABSOLUTE,
    RELATIVE,
};

// Using tabs is a syntax error.
#define IS_WHITE(c) ((c) == ' ')
//#define IS_WHITE(c) ((c) == ' ' || (c) == '\t')

#define SKIP_WHITE(s) \
    do { \
        while (IS_WHITE(s[0]) && (s).size > 0) { \
            (s).data += 1; \
            (s).size -= 1; \
        } \
    } while (0)

static bool
doFile(Context* ctx, StringView path) noexcept;

static void
set(uint32_t* n, uint32_t bit) noexcept {
    *n |= 1 << bit;
}

static void
reset(uint32_t* n, uint32_t bit) noexcept {
    *n &= ~(1 << bit);
}

static void
toggle(uint32_t* n, uint32_t bit) noexcept {
    *n ^= 1 << bit;
}

static uint32_t
test(uint32_t n, uint32_t bit) noexcept {
    return n & (1 << bit);
}

static StringView
file(Context* ctx) noexcept {
    if (ctx->currentFile.size) {
        return ctx->currentFile[ctx->currentFile.size - 1];
    }
    else {
        return "<top-level>";
    }
}

static Output&
warn(Context* ctx) noexcept {
    return serr << file(ctx) << ": warning: ";
}

static Output&
err(Context* ctx) noexcept {
    return serr << file(ctx) << ": error: ";
}

static StringView
stripComments(bool* inComment, String* buf, StringView line) noexcept {
    // FIXME: Ignore characters inside quoted strings.

    bool copied = false;

    size_t slash = line.find('/');

    while (slash != SV_NOT_FOUND) {
        if (*inComment) {
            if (slash > 0 && line[slash - 1] == '*') {
                // End of multi-line comment.
                *inComment = false;
                line = line.substr(slash + 1);
                // Don't bother putting a while-loop to skip remaining slashses.
                // (Skip the optimization.)
                slash = line.find('/');
            }
            else {
                // Continue being in a multi-line comment.
                // If there is more than one slashes, skip them all.
                // (Optimization.)
                slash += 1;
                for (; slash < line.size && line[slash] == '/'; slash++)
                    ;

                if (slash < line.size) {
                    slash = line.find('/');
                }
            }
        }
        else if (slash == line.size - 1) {
            // Not in a multi-line comment & the line ended with a slash.
            return line;
        }
        else if (line[slash + 1] == '/') {
            // Single-line comment. Like this one! :)
            return line.substr(0, slash);
        }
        else if (line[slash + 1] == '*') {
            // Start of a multi-line comment.
            size_t start = slash;
            size_t end = line.find('/', start + 1);

            // Check to see if the comment ends on the same line.
            while (end != SV_NOT_FOUND) {
                if (end > start + 2 && line[end - 1] == '*') {
                    // The comment ended on the same line.
                    break;
                }
                else {
                    // Keep searching.
                    end = line.find('/', end + 1);
                }
            }

            if (end == SV_NOT_FOUND) {
                // The multi-line comment will continue on the next line.
                *inComment = true;
                return line.substr(0, start);
            }
            else if (!copied) {
                // Populate the buffer with our current data so we can remove
                // the /* and */ pairing from within it since our "line"
                // argument is read-only.
                *buf = line.substr(0, start);
                *buf << line.substr(end + 1);
                line = *buf;
                copied = true;
            }
            else {
                // We have a buffer in use. Remove the /* and */ pairing from
                // within the buffer.
                size_t len = end + 1 - start;
                memmove(buf->data + start, buf->data + end, len);
                line.size -= len;
            }
            // Now that the slash at `slash` has been removed, we can perform a
            // search starting from where that slash used to reside.
            slash = line.find('/', start);
        }
        else {
            // We're not in a multi-line comment and the slash we found did not
            // have any play with comments. Continue searching.
            slash = line.find('/', slash + 1);
        }
    }

    return line;
}

static bool
findInclude(Context* ctx,
            String* resolvedPath,
            Vector<String>* includePaths,
            IncludeType type,
            StringView requestedPath) noexcept {
    if (type == ABSOLUTE) {
        err(ctx) << "#include: Absolute paths not implemented\n";
        return false;
    }

    if (resolvedPath->size < 32) {
        resolvedPath->reserve(256);
    }

    for (StringView path : *includePaths) {
        *resolvedPath = path;
        *resolvedPath << '/' << requestedPath;
        if (getFileSize(*resolvedPath) != FS_ERROR) {
            return true;
        }
    }

    err(ctx) << "#include: Could not find " << requestedPath << '\n';
    return false;
}

static bool
doDefine(Context* ctx, StringView line) noexcept {
    SKIP_WHITE(line);
    if (line.size == 0) {
        err(ctx) << "#define: Empty\n";
        return false;
    }

    size_t i;
    for (i = 0; i < line.size && !IS_WHITE(line[i]); i++)
        ;

    // TODO: Check to see if the user is defining a macro function.

    StringView key = line.substr(0, i);
    StringView value;

    if (i != line.size) {
        line.data += i;
        line.size -= i;

        SKIP_WHITE(line);
        if (line.size == 0) {
            err(ctx) << "#define: Trailing whitespace\n";
            return false;
        }

        for (i = 0; i < line.size && !IS_WHITE(line[i]); i++)
            ;

        value = line.substr(0, i);

        // TODO: Check for any remaining tokens in the line & issue
        //       warning/error if found.
    }

    // Create a permanent storage for the key and value.
    StringView copiedKey = ctx->strings << key;

    if (value.size) {
        StringView copiedValue = ctx->strings << value;
        ctx->defines[copiedKey] = copiedValue;
    }
    else {
        ctx->defines[copiedKey] = StringView();
    }

    return true;
}

static bool
doElif(Context* ctx, StringView line) noexcept {
    if (ctx->ifs == 0) {
        err(ctx) << "#elif: No matching #if\n";
        return false;
    }

    // TODO

    return true;
}

static bool
doElse(Context* ctx, StringView line) noexcept {
    if (ctx->ifs == 0) {
        err(ctx) << "#else: No matching #if\n";
        return false;
    }

    // TODO

    return true;
}

static bool
doEndif(Context* ctx, StringView line) noexcept {
    if (ctx->ifs == 0) {
        err(ctx) << "#endif: No matching #if\n";
        return false;
    }

    // TODO: Check for any remaining tokens in the line & issue
    //       warning/error if found.

    ctx->ifs -= 1;
    reset(&ctx->failedIfs, ctx->ifs);
    return true;
}

static bool
doError(Context* ctx, StringView line) noexcept {
    warn(ctx) << "#error" << line << '\n';
    return true;

    //serr << "error: #error" << line << '\n';
    //return false;
}

static bool
doIf(Context* ctx, StringView line) noexcept {
    // TODO: Check for whitespace.

    if (ctx->ifs == 32) {
        err(ctx) << "#ifdef: Too many nested #if directives (>32)\n";
        return false;
    }

    // TODO

    ctx->ifs += 1;
    return true;
}

static bool
doIfdef(Context* ctx, StringView line) noexcept {
    // TODO: Check for whitespace.

    if (ctx->ifs == 32) {
        err(ctx) << "#ifdef: Too many nested #if directives (>32)\n";
        return false;
    }

    SKIP_WHITE(line);
    if (line.size == 0) {
        err(ctx) << "#ifdef: Empty\n";
        return false;
    }

    size_t i;
    for (i = 0; i < line.size && !IS_WHITE(line[i]); i++)
        ;

    StringView key = line.substr(0, i);

    // TODO: Check for any remaining tokens in the line & issue
    //       warning/error if found.

    StringView* value = ctx->defines.tryAt(key);
    if (!value || (value->size == 1 && (*value)[0] == '0')) {
        set(&ctx->failedIfs, ctx->ifs);
    }

    ctx->ifs += 1;
    return true;
}

static bool
doIfndef(Context* ctx, StringView line) noexcept {
    // TODO: Check for whitespace.

    if (ctx->ifs == 32) {
        err(ctx) << "#ifndef: Too many nested #if directives (>32)\n";
        return false;
    }

    if (!doIfdef(ctx, line)) {
        return false;
    }

    toggle(&ctx->failedIfs, ctx->ifs - 1);
    return true;
}

static bool
doInclude(Context* ctx, StringView line) noexcept {
    size_t start = SIZE_MAX, end = SIZE_MAX;
    IncludeType type;

    for (size_t i = 0; i < line.size; i++) {
        if (line[i] == '<') {
            type = ABSOLUTE;
            start = i;
            break;
        }
        else if (line[i] == '"') {
            type = RELATIVE;
            start = i;
            break;
        }
        else if (IS_WHITE(line[i])) {
            continue;
        }
        else {
            err(ctx) << "#include: Expected < or \"\n";
            return false;
        }
    }

    if (start == SIZE_MAX) {
        err(ctx) << "#include: Expected < or \"\n";
        return false;
    }

    if (type == ABSOLUTE) {
        for (size_t i = start + 1; i < line.size; i++) {
            if (line[i] == '>') {
                end = i;
                break;
            }
        }
    }
    else {
        for (size_t i = start + 1; i < line.size; i++) {
            if (line[i] == '"') {
                end = i;
                break;
            }
        }
    }

    if (end == SIZE_MAX) {
        err(ctx) << "#include: Expected " << (type == ABSOLUTE ? '>' : ':')
                 << '\n';
        return false;
    }

    if (start + 1 == end) {
        err(ctx) << "#include: Empty path\n";
        return false;
    }

    StringView requestedPath = line.substr(start + 1, end - start - 1);

    // TODO: Check for any remaining tokens in the line & issue warning/error
    //       if found.

    String resolvedPath;
    if (!findInclude(ctx, &resolvedPath, &ctx->options->includePaths, type,
                     requestedPath)) {
        // FIXME: Change to an error.
        warn(ctx) << "#include: Skipping " << requestedPath << '\n';
        return true;
    }

    return doFile(ctx, resolvedPath);
}

static bool
doPragma(Context* ctx, StringView line) noexcept {
    return true;
}

static bool
doDirective(Context* ctx, StringView line) noexcept {
    SKIP_WHITE(line);
    if (line.size == 0) {
        err(ctx) << "#: Empty directive\n";
        return false;
    }

    if (line.size >= 6 && line[0] == 'd' && line[1] == 'e' && line[2] == 'f' &&
        line[3] == 'i' && line[4] == 'n' && line[5] == 'e') {
        if (ctx->failedIfs != 0) {
            return true;
        }
        return doDefine(ctx, line.substr(6));
    }
    else if (line.size >= 4 && line[0] == 'e' && line[1] == 'l' &&
             line[2] == 'i' && line[3] == 'f') {
        return doElif(ctx, line.substr(4));
    }
    else if (line.size >= 4 && line[0] == 'e' && line[1] == 'l' &&
             line[2] == 's' && line[3] == 'e') {
        return doElse(ctx, line.substr(4));
    }
    else if (line.size >= 5 && line[0] == 'e' && line[1] == 'n' &&
             line[2] == 'd' && line[3] == 'i' && line[4] == 'f') {
        return doEndif(ctx, line.substr(5));
    }
    else if (line.size >= 5 && line[0] == 'e' && line[1] == 'r' &&
             line[2] == 'r' && line[3] == 'o' && line[4] == 'r') {
        if (ctx->failedIfs != 0) {
            return true;
        }
        return doError(ctx, line.substr(5));
    }
    else if (line.size >= 5 && line[0] == 'i' && line[1] == 'f' &&
             line[2] == 'd' && line[3] == 'e' && line[4] == 'f') {
        return doIfdef(ctx, line.substr(5));
    }
    else if (line.size >= 6 && line[0] == 'i' && line[1] == 'f' &&
             line[2] == 'n' && line[3] == 'd' && line[4] == 'e' &&
             line[5] == 'f') {
        return doIfndef(ctx, line.substr(6));
    }
    else if (line.size >= 2 && line[0] == 'i' && line[1] == 'f') {
        return doIf(ctx, line.substr(2));
    }
    else if (line.size >= 7 && line[0] == 'i' && line[1] == 'n' &&
             line[2] == 'c' && line[3] == 'l' && line[4] == 'u' &&
             line[5] == 'd' && line[6] == 'e') {
        if (ctx->failedIfs != 0) {
            return true;
        }
        return doInclude(ctx, line.substr(7));
    }
    else if (line.size >= 6 && line[0] == 'p' && line[1] == 'r' &&
             line[2] == 'a' && line[3] == 'g' && line[4] == 'm' &&
             line[5] == 'a') {
        if (ctx->failedIfs != 0) {
            return true;
        }
        return doPragma(ctx, line.substr(6));
    }
    else {
        err(ctx) << "Unrecognized directive: #" << line << '\n';
        return false;
    }
}

static bool
doFile(Context* ctx, StringView path) noexcept {
    ReadLines lines(path);
    if (!lines) {
        err(ctx) << "Could not open " << path << '\n';
        return false;
    }

    ctx->currentFile.push_back(path);

    String buf;
    bool inComment = false;
    for (StringView line = lines.next(); line.data; line = lines.next()) {
        if (!line.size) {
            continue;
        }

        line = stripComments(&inComment, &buf, line);
        if (!line.size) {
            continue;
        }

        if (inComment) {
            continue;
        }

        SKIP_WHITE(line);
        if (!line.size) {
            continue;
        }

        if (line[0] == '#') {
            if (!doDirective(ctx, line.substr(1))) {
                return false;
            }
            continue;
        }

        if (ctx->failedIfs) {
            continue;
        }

        ctx->output->code << line;
        ctx->output->code << '\n';
    }

    if (inComment) {
        err(ctx) << "Open multi-line comment\n";
        return false;
    }

    ctx->currentFile.pop_back();

    return true;
}

bool
cpp(CppOutput* output, ProgramOptions* options, StringView path) noexcept {
    Context ctx;

    ctx.output = output;
    ctx.options = options;

    ctx.defines.reserve(4 * 1024);
    ctx.defines["__clang__"] = StringView();  // This is a total lie.
    ctx.defines["__cplusplus"] = "201103";  // This is also a total lie.
#if defined(__APPLE__)
    ctx.defines["__APPLE__"] = StringView();
#elif defined(__linux__)
    ctx.defines["__linux__"] = StringView();
#elif defined(__FreeBSD__)
    ctx.defines["__FreeBSD__"] = StringView();
#elif defined(__NetBSD__)
    ctx.defines["__NetBSD__"] = StringView();
#else
#error Not implemented yet
#endif

    ctx.ifs = 0;
    ctx.failedIfs = 0;

    return doFile(&ctx, path);
}
