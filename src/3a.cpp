// Time: O(n)
// Memory: O(1)

#include "os/c.h"
#include "util/int.h"
#include "util/string-view.h"
#include "util/string2.h"

int
main() noexcept {
    // Lazy iterator that yields one line of the file at a time.
    ReadLines lines;
    if (!lines.start("input")) {
        printf("Could not read input\n");
        return 1;
    }

    size_t hits = 0;

    size_t x = 0;

    StringView first = lines++;
    size_t width = first.size;

    for (StringView line = lines++; line.data; line = lines++) {
        x = (x + 3) % width;
        hits += line[x] == '#';
    }

    printf("%zu\n", hits);

    return 0;
}
