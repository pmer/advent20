// Time: O(n)
// Memory: O(1)

#include "util/hashtable.h"
#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

template<>
unsigned long
hash_<int>(int const& n) noexcept {
    return n;
}

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

    int first, last;
    Hashmap<int, int> window(50), compliments(50);

    int preamble = 25;
    for (StringView line = lines.next(); line.data; line = lines.next()) {
        int n = parseInt(line);

        if (preamble) {
            if (preamble == 25) {
                first = last = n;
            }
            else {
                window[last] = n;
                last = n;
            }
            preamble -= 1;
            continue;
        }

        // Construct compliment lookup table.
        compliments.clear();
        for (auto& entry : window) {  // Slow: Hashmap iteration.
            compliments[n - entry.key] = 0;
        }
        compliments[n - last] = 0;
        
        // Check for compliment.
        bool ok = false;
        for (auto& entry : window) {
            if (compliments.contains(entry.key)) {
                ok = true;
            }
        }
        if (compliments.contains(last)) {
            ok = true;
        }
        
        if (!ok) {
            // Our task is to find the first invalid number. We are done.
            sout << n << '\n';
            return 0;
        }

        // Remove old.
        int oldFirst = first;
        first = window[first];
        window.erase(oldFirst);

        // Add new.
        if (window.contains(n)) {
            // Duplicates would require additional bookkeeping. Issue error.
            serr << "Duplicate found for " << n << '\n';
            return 1;
        }
        window[last] = n;
        last = n;
    }

    return 0;
}
