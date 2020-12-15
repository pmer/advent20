#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

int
main() noexcept {
    ReadLines lines("data.csv");
    if (!lines) {
        printf("Could not read input\n");
        return 1;
    }

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        sout << line.size << '\n';
    }

    return 0;
}
