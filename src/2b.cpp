// Time: O(n)
// Memory: O(n)

#include "os/c.h"
#include "os/os.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"
#include "util/vector.h"

int main() noexcept {
    String data;
    readFile("input", data);

    Vector<StringView> lines;
    Vector<StringView> tokens;
    Vector<int> range;
    int numValid = 0;

    splitStr(lines, data, "\n");
    for (StringView line : lines) {
        tokens.clear();
        splitStr(tokens, line, " ");
        if (tokens.size != 3) {
            printf("Invalid line: %s\n", String(line).null());
            return 1;
        }

        StringView rangeToken = tokens[0];
        StringView needleToken = tokens[1];
        StringView haystackToken = tokens[2];

        range.clear();
        if (!parseRanges(range, rangeToken)) {
            printf("Invalid range: %s\n", String(rangeToken).null());
            return 1;
        }
        int lower = range[0];
        int higher = range[range.size - 1];

        if (needleToken.size != 2 ||
            !('a' <= needleToken[0] && needleToken[0] <= 'z') ||
            needleToken[1] != ':') {
            printf("Invalid needle: %s\n", String(needleToken).null());
            return 1;
        }
        char needle = needleToken[0];

        if (haystackToken.size < higher) {
            continue;
        }

        bool loFound = haystackToken[lower - 1] == needle;
        bool hiFound = haystackToken[higher - 1] == needle;

        bool valid = loFound ^ hiFound;
        if (valid) {
            numValid += 1;
        }
    }

    printf("%d\n", numValid);

    return 0;
}
