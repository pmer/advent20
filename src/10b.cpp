// Time: O(n ln n)
// Memory: O(n)

#include "util/int.h"
#include "util/io.h"
#include "util/move.h"
#include "util/sort.h"
#include "util/string-view.h"
#include "util/string2.h"
#include "util/vector.h"

static int64_t
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

    Vector<int64_t> adapters;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        int64_t n = parseInt(line);
        adapters.push_back(n);
    }

#define LESS(i, j) adapters[i] < adapters[j]
#define SWAP(i, j) swap_(adapters[i], adapters[j])
    QSORT(adapters.size, LESS, SWAP);

    Vector<int64_t> combos;
    combos.resize(adapters[adapters.size - 1] + 1);

    // The charging outlet.
    combos[0] = 1;

    // Your adapters.
    for (int64_t jolts : adapters) {
        int64_t sum = 0;
        if (jolts >= 1) {
            sum += combos[jolts - 1];
        }
        if (jolts >= 2) {
            sum += combos[jolts - 2];
        }
        if (jolts >= 3) {
            sum += combos[jolts - 3];
        }
        combos[jolts] = sum;
    }

    sout << combos[combos.size - 1] << '\n';
    return 0;
}
