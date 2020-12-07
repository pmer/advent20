// Time: O(n)
// Memory: O(1)
//
// Processes 300 MB/sec on an Intel Core i5-1030NG7.

#include "os/c.h"
#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

int
main() noexcept {
    // Lazy iterator that yields one line of the file at a time.
    ReadLines lines("input");
    if (!lines) {
        serr << "Could not read input\n";
        return 1;
    }

    size_t as[26] = {};
    size_t family = 0;
    size_t sum = 0;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        if (line.size) {
            family += 1;
#pragma GCC unroll 26
            for (size_t i = 0; i < line.size; i++) {
                as[line[i] - 'a'] += 1;
            }
        }
        else {
#pragma GCC unroll 26
            for (size_t i = 0; i < 26; i++) {
                sum += as[i] == family;
            }

            memset(as, 0, sizeof(as));
            family = 0;
        }
    }

    for (size_t i = 0; i < 26; i++) {
        sum += as[i] == family;
    }

    sout << sum << '\n';

    return 0;
}
