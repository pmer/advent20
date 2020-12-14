// Time: O(2**n)
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

struct Machine {
    int32_t acc;
    Vector<Instruction> is;
};

static short
parseI(StringView s) noexcept {
    short i;
    switch (s.size) {
        case 2:
            i = (s[1] - '0') * 1;
            break;
        case 3:
            i = (s[1] - '0') * 10 + (s[2] - '0') * 1;
            break;
        case 4:
            i = (s[1] - '0') * 100 + (s[2] - '0') * 10 + (s[3] - '0') * 1;
            break;
    }
    return s[0] == '+' ? i : -i;
}

static bool
create(Machine& m, StringView path) noexcept {
    // Lazy iterator that yields one line of the file at a time.
    ReadLines lines(path);
    if (!lines) {
        return false;
    }

    m.acc = 0;
    m.is.reserve(1024);

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        Instruction i;

        i.visited = false;
        switch (line[0]) {
            case 'a': i.type = ACC; break;
            case 'j': i.type = JMP; break;
            case 'n': i.type = NOP; break;
            default: sout << "Unknown line " << line << '\n';
        }
        i.data = parseI(line.substr(4));

        m.is.push_back(i);
    }

    return true;
}

static bool
exe(Machine& m, int32_t pc, bool corrected) noexcept {
    if (pc == m.is.size) {
        return true;
    }
    if (pc < 0 || pc > m.is.size) {
        sout << "Out of bounds\n";
        return false;
    }

    Instruction& i = m.is[pc];

    if (i.visited) {
        return false;
    }
    i.visited = true;

    switch (i.type) {
        case ACC:
            m.acc += i.data;
            if (exe(m, pc + 1, corrected)) {
                return true;
            }
            m.acc -= i.data;
            break;

        case JMP:
            if (exe(m, pc + i.data, corrected)) {
                return true;
            }
            if (!corrected && exe(m, pc + 1, true)) {
                return true;
            }
            break;

        case NOP:
            if (exe(m, pc + 1, corrected)) {
                return true;
            }
            if (!corrected && exe(m, pc + i.data, true)) {
                return true;
            }
            break;
    }

    i.visited = false;
    return false;
}

int
main() noexcept {
    Machine m;
    if (!create(m, "8")) {
        sout << "Couldn't create machine\n";
        return 1;
    }

    if (!exe(m, 0, false)) {
        sout << "Machine never finished\n";
        return 1;
    }

    sout << m.acc << '\n';

    return 0;
}
