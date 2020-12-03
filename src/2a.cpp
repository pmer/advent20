// Time: O(n)
// Memory: O(1)

#include "os/c.h"
#include "os/os.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"
#include "util/vector.h"

// parseRange()
//
// When called:
//   int lo, hi;
//   parseRange(lo, hi, "3-12");
//
// Effect:
//   lo <- 3
//   hi <- 12
//
// Only supports 1- and 2-digits numbers.
//
// Does 3 or 4 memory reads (depending on if the second number is 1 or 2
// digits), two comparisons, and some arithmetic. The lo and hi parameters will
// be stored in registers, so assigning a value to them will not involve a
// write to memory.
static void
parseRange(int& lo, int& hi, StringView s) noexcept {
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

    // Only place in the program that allocates memory. Everything else is just
    // read-only pointers into this character array and no copies of any part
    // are ever made.
    if (!readFile("input", data)) {
        printf("Could not read input\n");
        return 1;
    }

    // Lazy iterator that yields one line of the file at a time.
    Lines lines = readLines(data);

    int numValid = 0;

    for (StringView line = lines++; line.size; line = lines++) {
        // Extract the range.
        StringView range = StringView(line.data, line.find(' '));

        int lo, hi;
        parseRange(lo, hi, range);

        // Extract the needle.
        char needle = range.data[range.size + 1];  // 1 = space

        // Extract the haystack.
        size_t next = range.size + 4;  // 4 = space, needle, colon, space
        StringView haystack = StringView(line.data + next, line.size - next);

        if (haystack.size < lo) {
            // Optimization: The haystack is not large enough for this line to
            //               be valid. Skip.
            continue;
        }

        // Count the occurences of the needle in the haystack. Most of the
        // program's execution time is running this loop.
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
        // Split the line into three tokens.
        tokens.clear();
        splitStr(tokens, line, " ");
        if (tokens.size != 3) {
            printf("Invalid line: %s\n", String(line).null());
            return 1;
        }

        StringView rangeToken = tokens[0];
        StringView needleToken = tokens[1];
        StringView haystackToken = tokens[2];

        // Extract the range.
        range.clear();
        if (!parseRanges(range, rangeToken)) {
            printf("Invalid range: %s\n", String(rangeToken).null());
            return 1;
        }
        int lower = range[0];
        int higher = range[range.size - 1];

        // Extract the needle.
        if (needleToken.size != 2 ||
            !('a' <= needleToken[0] && needleToken[0] <= 'z') ||
            needleToken[1] != ':') {
            printf("Invalid needle: %s\n", String(needleToken).null());
            return 1;
        }
        char needle = needleToken[0];

        // Count the occurences.
        int occurences = 0;
        for (char letter : haystackToken) {
            occurences += needle == letter;
        }

        // If occurences are in the right range, increment numValid.
        bool valid = lower <= occurences && occurences <= higher;
        if (valid) {
            numValid += 1;
        }
    }

    printf("%d\n", numValid);

    return 0;
}
*/
