// Time: O(n)
// Memory: O(1)

#include "os/c.h"
#include "os/os.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"

int
main() noexcept {
    String data;
    if (!readFile("input", data)) {
        printf("Could not read input\n");
        return 1;
    }

    size_t hits = 0;

    Lines lines = readLines(data);

    size_t x = 0;

    StringView first = lines++;
    size_t width = first.size;

    for (StringView line = lines++; line.size; line = lines++) {
        x = (x + 3) % width;
        hits += line[x] == '#';
    }

    printf("%zu\n", hits);

    return 0;
}
