// Time: O(n * m)
// Memory: O(1)
//
// Processes 1.6 GB/sec on an Intel Core i5-1030NG7.

#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

int
main() noexcept {
    // Lazy iterator that yields one line of the file at a time.
    ReadLines lines;
    if (!lines.start("input")) {
        serr << "Could not read input\n";
        return 1;
    }

    size_t hits[5] = {};
    size_t x[5] = {};

    size_t y = 0;

    StringView first = lines++;
    size_t width = first.size;

    for (StringView line = lines++; line.data; line = lines++) {
        y += 1;

        x[0] = (x[0] + 1) % width;
        x[1] = (x[1] + 3) % width;
        x[2] = (x[2] + 5) % width;
        x[3] = (x[3] + 7) % width;

        hits[0] += line[x[0]] == '#';
        hits[1] += line[x[1]] == '#';
        hits[2] += line[x[2]] == '#';
        hits[3] += line[x[3]] == '#';
        if (y % 2 == 0) {
            x[4] = (x[4] + 1) % width;
            hits[4] += line[x[4]] == '#';
        }
    }

    sout << hits[0] * hits[1] * hits[2] * hits[3] * hits[4] << '\n';

    return 0;
}
