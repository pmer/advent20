// Time: O(n^2)
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
    String data;
    readFile("input", data);

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

    Vector<bool> found;
    found.resize(TARGET + 1);
    for (size_t i = 0; i < found.size; i++) {
        found[i] = false;
    }

    for (int i : numbers) {
        for (int j : numbers) {
            int k = TARGET - i - j;
            if (k >= 0 && k < TARGET && found[k]) {
                // No duplicates in the input, so no need to check if we have
                // seen two of the same value when j = k.
                printf("%d\n", i * j * k);
                return 0;
            }
        }
        found[i] = true;
    }

    printf("Not found\n");
    return 0;
}
