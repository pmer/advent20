// Time: N/A
// Memory: O(n)

#include "os/c.h"
#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

#define SIZE 98

typedef bool Grid[SIZE * SIZE];

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

                bool top = row == 0;
                bool bottom = row == SIZE - 1;
                bool left = col == 0;
                bool right = col == SIZE - 1;

                int adjacent =
                    (!top    && !left  && prev[i - SIZE - 1]) +
                    (!top    && !right && prev[i - SIZE + 1]) +
                    (!bottom && !left  && prev[i + SIZE - 1]) +
                    (!bottom && !right && prev[i + SIZE + 1]) +
                    (!top    &&           prev[i - SIZE]) +
                    (!bottom &&           prev[i + SIZE]) +
                    (!left   &&           prev[i - 1]) +
                    (!right  &&           prev[i + 1]);

                if (prev[i]) {
                    // If a seat is occupied (#) and four or more seats adjacent
                    // to it are also occupied, the seat becomes empty.
                    next[i] = adjacent < 4;
                }
                else {
                    // If a seat is empty (L) and there are no occupied seats
                    // adjacent to it, the seat becomes occupied.
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
