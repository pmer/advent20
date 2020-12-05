// Time: O(n * m)
// Memory: O(n)

#include "os/c.h"
#include "os/os.h"
#include "util/int.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"

static size_t
path(StringView data, size_t r, size_t d) noexcept {
    size_t hits = 0;

    Tokens<'\n'> lines = split<'\n'>(data);

    size_t x = 0;
    size_t y = 0;

    StringView first = lines++;
    size_t width = first.size;

    for (StringView line = lines++; line.data; line = lines++) {
        y += 1;
        if (y % d != 0) {
            continue;
        }

        x = (x + r) % width;
        hits += line[x] == '#';
    }

    return hits;
}

int
main() noexcept {
    String data;
    if (!readFile("input", data)) {
        printf("Could not read input\n");
        return 1;
    }

    printf("%zu\n",
           path(data, 1, 1) * path(data, 3, 1) * path(data, 5, 1) *
               path(data, 7, 1) * path(data, 1, 2));

    return 0;
}
