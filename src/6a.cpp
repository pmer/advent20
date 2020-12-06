// Time: O(n)
// Memory: O(1)
//
// Processes 150 MB/sec on an Intel Core i5-7600K.

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

    bool as[26] = {};
    size_t sum = 0;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        if (line.size) {
            for (size_t i = 0; i < line.size; i++) {
                as[line[i] - 'a'] = true;
            }
        }
        else {
            size_t count = 0;
            for (size_t i = 0; i < 26; i++) {
                count += as[i];
            }

            sum += count;
            memset(as, 0, sizeof(as));
        }
    }

    size_t count = 0;
    for (size_t i = 0; i < 26; i++) {
        count += as[i];
    }
    sum += count;

    sout << sum << '\n';

    return 0;
}
