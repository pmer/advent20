// Time: O(n ln n)
// Memory: O(n)

#include "util/int.h"
#include "util/io.h"
#include "util/move.h"
#include "util/sort.h"
#include "util/string-view.h"
#include "util/string2.h"
#include "util/vector.h"

static int
parseInt(StringView s) noexcept {
    switch (s.size) {
        case 1:
            return s[0] - '0';
        case 2:
            return (s[0] - '0') * 10 + s[1] - '0';
        case 3:
            return (s[0] - '0') * 100 + (s[1] - '0') * 10 + s[2] - '0';
        default:
            return -1;
    }
}

int
main() noexcept {
    ReadLines lines("10");
    if (!lines) {
        serr << "Could not read input\n";
        return 1;
    }

    Vector<int> adapters;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        int n = parseInt(line);
        adapters.push_back(n);
    }

#define LESS(i, j) adapters[i] < adapters[j]
#define SWAP(i, j) swap_(adapters[i], adapters[j])
    QSORT(adapters.size, LESS, SWAP);

    int ones = 0;
    int threes = 0;

    // The charging outlet.
    switch (adapters[0]) {
        case 1: ones++;   break;
        case 3: threes++; break;
    }

    // Your adapters.
    for (size_t i = 0; i < adapters.size - 1; i++) {
        switch (adapters[i + 1] - adapters[i]) {
            case 1: ones++;   break;
            case 3: threes++; break;
        }
    }

    // Your laptop.
    threes++;

    sout << ones * threes << '\n';
    return 0;
}

// Higher than 1020
