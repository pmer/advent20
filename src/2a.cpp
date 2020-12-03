// Time: O(n)
// Memory: O(n)

#include "os/c.h"
#include "os/os.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"
#include "util/vector.h"

void
parseR(int& lo, int& hi, StringView s) noexcept {
    const char* data = s.data;
    size_t size = s.size;
    size_t offset;

    if (data[1] == '-') {
        lo = data[0] - '0';
        offset = 2;
    }
    else {
        lo = (data[0] - '0') * 10 + data[1];
        offset = 3;
    }

    if (size - offset == 1) {
        hi = data[offset] - '0';
    }
    else {
        hi = (data[offset] - '0') * 10 + data[offset + 1];
    }
}

int main() noexcept {
    String data;
    if (!readFile("input", data)) {
        printf("Could not read input\n");
        return 1;
    }

    Vector<StringView> tokens;
    int numValid = 0;

    Lines lines = readLines(data);

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
        parseR(lo, hi, rangeToken);

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
