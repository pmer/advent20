// Time: O(n)
// Memory: O(1)
//
// Processes 200 MB/sec on an Intel Core i5-1030NG7.

#include "os/os.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"
#include "util/vector.h"

int
main() noexcept {
    ReadLines lines("input");
    if (!lines) {
        serr << "Could not read input\n";
        return 1;
    }

    Vector<StringView> tokens;
    int numValid = 0;
    String buf;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        tokens.clear();
        splitStr(tokens, line, " ");
        if (tokens.size != 3) {
            serr << "Invalid line: " << line << '\n';
            return 1;
        }

        StringView rangeToken = tokens[0];
        StringView needleToken = tokens[1];
        StringView haystackToken = tokens[2];

        int lo, hi;
        if (!parseRange(lo, hi, rangeToken, buf)) {
            serr << "Invalid range: " << rangeToken << '\n';
            return 1;
        }

        if (needleToken.size != 2 ||
            !('a' <= needleToken[0] && needleToken[0] <= 'z') ||
            needleToken[1] != ':') {
            serr << "Invalid needle: " << needleToken << '\n';
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

    sout << numValid << '\n';

    return 0;
}
