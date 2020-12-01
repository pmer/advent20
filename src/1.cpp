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
        if (found[TARGET - i]) {
            printf("%d\n", i * (TARGET - i));
            return 0;
        }
        found[i] = true;
    }

    printf("Not found\n");
    return 0;
}
