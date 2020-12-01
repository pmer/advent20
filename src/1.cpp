// Time: O(n)
// Memory: O(n)

#include "os/c.h"
#include "os/os.h"
#include "util/int.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"
#include "util/vector.h"

#define TARGET 2020

int main() noexcept {
    // Read the input data.
    String data;
    readFile("input", data);

    // Parse the integers.
    Vector<StringView> lines = splitStr(data, "\n");

    Vector<int> numbers;
    numbers.reserve(lines.size);

    String line_;
    for (StringView line : lines) {
        line_.clear();
        line_ << line;
        int i;
        if (!parseInt(i, line_)) {
            printf("Invalid int: %s\n", line_.null());
            return 1;
        }

        numbers.push_back(i);
    }

    // We start having not seen anything.
    Vector<bool> seen;
    seen.resize(TARGET + 1);
    for (size_t i = 0; i < seen.size; i++) {
        seen[i] = false;
    }

    for (int i : numbers) {
        // The compliment to i.
        //   i + j = TARGET
        int j = TARGET - i;

        // If we've seen the necessary compliment for TARGET, we are done.
        if (seen[j]) {
            printf("%d\n", i * j);
            return 0;
        }
        // Otherwise, if we ever counter j later on, we'll know we've already
        // seen i.
        else {
            seen[i] = true;
        }
    }

    printf("Not found\n");
    return 0;
}
