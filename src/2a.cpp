// Time: O(n)
// Memory: O(1)

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
        lo = (data[0] - '0') * 10 + data[1] - '0';
        offset = 3;
    }

    if (size - offset == 1) {
        hi = data[offset] - '0';
    }
    else {
        hi = (data[offset] - '0') * 10 + data[offset + 1] - '0';
    }
}

int main() noexcept {
    String data;
    if (!readFile("input", data)) {
        printf("Could not read input\n");
        return 1;
    }

    int numValid = 0;

    Lines lines = readLines(data);

    for (StringView line = lines++; line.size; line = lines++) {
        StringView rangeToken = StringView(
            line.data,
            line.find(' ')
        );

        char needle = rangeToken.data[rangeToken.size + 1];

        size_t begin = rangeToken.size + 4;
        StringView haystackToken = StringView(
            line.data + begin,
            line.size - begin
        );

        int lo, hi;
        parseR(lo, hi, rangeToken);

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
