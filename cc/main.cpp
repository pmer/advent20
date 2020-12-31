#include "cpp.h"

#include "options.h"
#include "os/os.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/vector.h"

struct Args {
    ProgramOptions options;
    bool preprocessorOnly;
    Vector<String> sourcePaths;
};

static bool
parseArgs(Args* args, int argv, char** argc) noexcept {
    for (int i = 1; i < argv; i++) {
        StringView arg = argc[i];

        if (arg.size > 2 && arg[0] == '-' && arg[1] == 'I') {
            args->options.includePaths.push_back(arg.substr(2));
        }
        else if (arg.size == 2 && arg[0] == '-' && arg[1] == 'E') {
            args->preprocessorOnly = true;
        }
        else {
            args->sourcePaths.push_back(arg);
        }
    }

    return true;
}

int
main(int argv, char** argc) noexcept {
    Args args;
    args.preprocessorOnly = false;

    if (!parseArgs(&args, argv, argc)) {
        return 1;
    }

    for (String& path : args.options.includePaths) {
        if (!isDir(path)) {
            serr << "error: Not a directory: " << path << '\n';
            return 1;
        }
    }
    for (String& path : args.sourcePaths) {
        if (getFileSize(path) == FS_ERROR) {
            serr << "error: Not a file: " << path << '\n';
            return 1;
        }
    }

    for (String& path : args.sourcePaths) {
        CppOutput output;
        if (!cpp(&output, &args.options, path)) {
            return 1;
        }

        if (args.preprocessorOnly) {
            sout << output.code;
        }
    }

    return 0;
}
