// Time: N/A
// Memory: O(n)

#include "os/c.h"
#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

#define SIZE 98

typedef bool Grid[SIZE * SIZE];

static bool
visible(Grid& layout, Grid& occupied, int x0, int y0, int dx, int dy) noexcept {
    int x = x0 + dx;
    int y = y0 + dy;

    while (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        int i = y * SIZE + x;
        if (layout[i]) {
            return occupied[i];
        }
        x += dx;
        y += dy;
    }

    return false;
}

int
main() noexcept {
    ReadLines lines("11");
    if (!lines) {
        serr << "Could not read input\n";
        return 1;
    }

    Grid layout = {};

    size_t row = 0;
    for (StringView line = lines.next(); line.data; line = lines.next()) {
        for (size_t col = 0; col < SIZE; col++) {
            if (line[col] == 'L') {
                layout[row * SIZE + col] = true;
            }
        }
        row++;
    }

    // Occupied: previous/current state & upcoming state.
    Grid prev = {}, next;

    bool changing;
    int i = 0;
    do {
        changing = false;

        for (size_t row = 0; row < SIZE; row++) {
            for (size_t col = 0; col < SIZE; col++) {
                size_t i = row * SIZE + col;

                if (!layout[i]) {
                    // This is a floor space.
                    next[i] = false;
                    continue;
                }

                int adjacent = visible(layout, prev, col, row, -1, -1) +
                               visible(layout, prev, col, row, -1,  0) +
                               visible(layout, prev, col, row, -1, +1) +
                               visible(layout, prev, col, row,  0, -1) +
                               visible(layout, prev, col, row,  0, +1) +
                               visible(layout, prev, col, row, +1, -1) +
                               visible(layout, prev, col, row, +1,  0) +
                               visible(layout, prev, col, row, +1, +1);

                if (prev[i]) {
                    // Changed to 5.
                    next[i] = adjacent < 5;
                }
                else {
                    next[i] = adjacent == 0;
                }
            }
        }

        changing = memcmp(prev, next, sizeof(prev)) != 0;
        memcpy(prev, next, sizeof(prev));
    } while (changing);

    size_t numOccupied = 0;
    for (bool isOccupied : prev) {
        numOccupied += isOccupied;
    }
    sout << numOccupied << '\n';

    return 0;
}
