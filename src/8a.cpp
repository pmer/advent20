// Time: O(n)
// Memory: O(n)

#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"
#include "util/vector.h"

enum Type {
    NOP,
    ACC,
    JMP,
};

struct Instruction {
    bool visited;
    char type;
    short data;
};

#define ONES(c) (c - '0')
#define TENS(c) (ONES(c) * 10)
#define HUNDREDS(c) (ONES(c) * 100)

#define ONE_DIGIT_DECIMAL(s) \
    (ONES((s)[0]))
#define TWO_DIGIT_DECIMAL(s) \
    (TENS((s)[0]) + ONES((s)[1]))
#define THREE_DIGIT_DECIMAL(s) \
    (HUNDREDS((s)[0]) + TENS((s)[1]) + ONES((s)[2]))

short
parseI(StringView s) {
    short i;
    switch (s.size) {
        case 2:
            i = ONE_DIGIT_DECIMAL(s.data + 1);
            break;
        case 3:
            i = TWO_DIGIT_DECIMAL(s.data + 1);
            break;
        case 4:
            i = THREE_DIGIT_DECIMAL(s.data + 1);
            break;
    }
    return s[0] == '+' ? i : -i;
}

int
main() noexcept {
    // Lazy iterator that yields one line of the file at a time.
    ReadLines lines("8");
    if (!lines) {
        serr << "Could not read input\n";
        return 1;
    }

    Vector<Instruction> is;
    is.reserve(1024);

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        Instruction i;

        i.visited = false;
        switch (line[0]) {
            case 'a': i.type = ACC; break;
            case 'j': i.type = JMP; break;
            case 'n': i.type = NOP; break;
        }
        if (i.type == ACC || i.type == JMP) {
            i.data = parseI(line.substr(4));
        }

        is.push_back(i);
    }

    size_t pc = 0;
    int32_t acc = 0;

    while (!is[pc].visited) {
        is[pc].visited = true;
        switch (is[pc].type) {
            case ACC:
                acc += is[pc].data;
                pc += 1;
                break;
            case JMP:
                pc += is[pc].data;
                break;
            case NOP:
                pc += 1;
                break;
        }
    }

    sout << acc << '\n';

    return 0;
}
