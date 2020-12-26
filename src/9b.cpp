// Time: O(n)
// Memory: O(1)

#include "util/int.h"
#include "util/io.h"
#include "util/move.h"
#include "util/sort.h"
#include "util/string-view.h"
#include "util/string2.h"
#include "util/vector.h"

#define ANSWER 26796446

static int
parseInt(StringView s) noexcept {
    int n = 0;
    for (const char* c = s.data; c < s.data + s.size; c++) {
        n = n * 10 + *c - '0';
    }
    return n;
}

int
main() noexcept {
    ReadLines lines("9");
    if (!lines) {
        serr << "Could not read input\n";
        return 1;
    }

    // FIXME: Replace with queue.
    Vector<int> window;
    int sum = 0;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        int n = parseInt(line);

        sum += n;
        window.push_back(n);

        while (sum > ANSWER) {
            sum -= window[0];
            window.erase(0);
        }

        if (sum == ANSWER) {
            // No more searching. This is it.
#define LESS(i, j) window[i] < window[j]
#define SWAP(i, j) swap_(window[i], window[j])
            QSORT(window.size, LESS, SWAP);
            sout << window[0] + window[window.size - 1] << '\n';
            return 0;
        }
    }

    serr << "Not found\n";
    return 1;
}
