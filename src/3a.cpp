// Time: O(n)
// Memory: O(1)
//
// Processes 1.9 GB/sec on an Intel Core i5-1030NG7.

#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

int
main() noexcept {
    // Lazy iterator that yields one line of the file at a time.
    ReadLines lines("3");
    if (!lines) {
        printf("Could not read input\n");
        return 1;
    }

    size_t hits = 0;
    size_t x = 0;

    // We start on the first line, so we skip it.
    StringView first = lines.next();

    // Grab the width of each line. It is always 31.
    size_t width = first.size;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        x = (x + 3) % width;
        hits += line[x] == '#';
    }

    sout << hits << '\n';

    return 0;
}
