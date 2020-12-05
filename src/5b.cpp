// Time: O(n)
// Memory: O(1)
//
// Processes 550 MB/sec on an Intel Core i5-1030NG7.

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

    bool taken[1024] = {};

    for (StringView line = lines++; line.data; line = lines++) {
        uint32_t seat = ((line[0] == 'B') << 9) +
                        ((line[1] == 'B') << 8) +
                        ((line[2] == 'B') << 7) +
                        ((line[3] == 'B') << 6) +
                        ((line[4] == 'B') << 5) +
                        ((line[5] == 'B') << 4) +
                        ((line[6] == 'B') << 3) +
                        ((line[7] == 'R') << 2) +
                        ((line[8] == 'R') << 1) +
                        ((line[9] == 'R') << 0);
        taken[seat] = true;
    }

    uint32_t i = 0;
    while (taken[i] == false) {
        i++;
    }
    while (taken[i] == true) {
        i++;
    }

    printf("%u\n", i);

    return 0;
}
