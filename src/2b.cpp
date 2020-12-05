// Time: O(n)
// Memory: O(1)
//
// Processes 200 MB/sec on an Intel Core i5-1030NG7.

#include "os/c.h"
#include "os/os.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"
#include "util/vector.h"

int
main() noexcept {
    ReadLines lines;
    if (!lines.start("input")) {
        printf("Could not read input\n");
        return 1;
    }

    Vector<StringView> tokens;
    int numValid = 0;
    String buf;

    for (StringView line = lines++; line.data; line = lines++) {
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
        if (!parseRange(lo, hi, rangeToken, buf)) {
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

        if (haystackToken.size < hi) {
            continue;
        }

        bool loFound = haystackToken[lo - 1] == needle;
        bool hiFound = haystackToken[hi - 1] == needle;

        bool valid = loFound ^ hiFound;
        if (valid) {
            numValid += 1;
        }
    }

    printf("%d\n", numValid);

    return 0;
}
