// Time: O(n)
// Memory: O(n)

#include "os/c.h"
#include "os/mapped-file.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"
#include "util/vector.h"

int main() noexcept {
    MappedFile file;
    if (!makeMappedFile(file, "input")) {
        printf("Could not open input\n");
        return 1;
    }

    Vector<StringView> tokens;
    int numValid = 0;

    Lines lines = readLines(file.data);

    for (StringView line = lines++; line.size; line = lines++) {
        tokens.clear();
        splitStr(tokens, line, " ");
        if (tokens.size != 3) {
            printf("Invalid line: %s\n", String(line).null());
            return 1;
        }

        StringView rangeToken = tokens[0];
        StringView needleToken = tokens[1];
        StringView haystackToken = tokens[2];

        int lo, hi;
        if (!parseRange(lo, hi, rangeToken)) {
            printf("Invalid range: %s\n", String(rangeToken).null());
            return 1;
        }

        if (needleToken.size != 2 ||
            !('a' <= needleToken[0] && needleToken[0] <= 'z') ||
            needleToken[1] != ':') {
            printf("Invalid needle: %s\n", String(needleToken).null());
            return 1;
        }
        char needle = needleToken[0];

        if (haystackToken.size < lo) {
            continue;
        }

        int found = 0;
        for (char letter : haystackToken) {
            found += needle == letter;
        }

        bool valid = lo <= found && found <= hi;
        if (valid) {
            numValid += 1;
        }
    }

    printf("%d\n", numValid);

    return 0;
}
