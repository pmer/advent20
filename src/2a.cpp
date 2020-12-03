// Time: O(n)
// Memory: O(1)

#include "os/c.h"
#include "os/os.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"
#include "util/vector.h"

// parseR()
//
// When called:
//   int lo, hi;
//   parseR(lo, hi, "3-12");
//
// Effect:
//   lo <- 3
//   hi <- 12
//
// Only supports 1- and 2-digits numbers.
static void
parseR(int& lo, int& hi, StringView s) noexcept {
    const char* data = s.data;
    size_t size = s.size;
    size_t offset;

    if (data[1] == '-') {
        // First number has 1 digit.
        lo = data[0] - '0';
        offset = 2;
    }
    else {
        // First number has 2 digits.
        lo = (data[0] - '0') * 10 + data[1] - '0';
        offset = 3;
    }

    if (size - offset == 1) {
        // Second number has 1 digit.
        hi = data[offset] - '0';
    }
    else {
        // Second number has 2 digits.
        hi = (data[offset] - '0') * 10 + data[offset + 1] - '0';
    }
}

int
main() noexcept {
    String data;
    if (!readFile("input", data)) {
        printf("Could not read input\n");
        return 1;
    }

    Lines lines = readLines(data);

    int numValid = 0;

    for (StringView line = lines++; line.size; line = lines++) {
        // Extract the range.
        StringView range = StringView(
            line.data,
            line.find(' ')
        );

        int lo, hi;
        parseR(lo, hi, range);

        // Extract the needle.
        char needle = range.data[range.size + 1];

        // Extract the haystack.
        size_t begin = range.size + 4;
        StringView haystack = StringView(
            line.data + begin,
            line.size - begin
        );

        // Optimization.
        if (haystack.size < lo) {
            continue;
        }

        // Count the occurences.
        int occurences = 0;
        for (char letter : haystack) {
            occurences += needle == letter;
        }

        // If occurences are in the right range, increment numValid.
        bool valid = lo <= occurences && occurences <= hi;
        if (valid) {
            numValid += 1;
        }
    }

    printf("%d\n", numValid);

    return 0;
}



/*
int
main() noexcept {
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

        if (haystackToken.size < lower) {
            continue;
        }

        int found = 0;
        for (char letter : haystackToken) {
            found += needle == letter;
        }

        bool valid = lower <= found && found <= higher;
        if (valid) {
            numValid += 1;
        }
    }

    printf("%d\n", numValid);

    return 0;
}
*/
