// Time: O(n)
// Memory: O(n)

#include "util/int.h"
#include "util/io.h"
#include "util/queue.h"
#include "util/string-view.h"
#include "util/string2.h"

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

    Queue<int> window;
    int sum = 0;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        int n = parseInt(line);

        sum += n;
        window.push(n);

        while (sum > ANSWER) {
            sum -= window.front();
            window.pop();
        }

        if (sum == ANSWER) {
            // End of search. This is it!
            int lo = INT32_MAX, hi = 0;
            for (size_t i = 0; i < window.size; i++) {
                int n = window[i];
                lo = n < lo ? n : lo;
                hi = n > hi ? n : hi;
            }
            sout << lo + hi << '\n';
            return 0;
        }
    }

    serr << "Not found\n";
    return 1;
}
