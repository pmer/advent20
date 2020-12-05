// Time: O(n)
// Memory: O(1)

#include "os/c.h"
#include "os/os.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/string2.h"

#define makeTag(a, b, c) \
    static const constexpr uint32_t a##b##c = \
        (#a[0] << 0) | (#b[0] << 8) | (#c[0] << 16)

makeTag(b, y, r);
makeTag(i, y, r);
makeTag(e, y, r);
makeTag(h, g, t);
makeTag(h, c, l);
makeTag(e, c, l);
makeTag(p, i, d);
//makeTag(c, i, d);  // Ignore.

makeTag(a, m, b);
makeTag(b, l, u);
makeTag(b, r, n);
makeTag(g, r, y);
makeTag(g, r, n);
makeTag(h, z, l);
makeTag(o, t, h);

#define TAG(s) (((s)[0] << 0) | ((s)[1] << 8) | ((s)[2] << 16))

#define IS_DIGIT(c) ('0' <= (c) && (c) <= '9')
#define IS_HEX(c) (('0' <= (c) && (c) <= '9') || ('a' <= (c) && (c) <= 'f'))

#define ONES(c) (c - '0')
#define TENS(c) (ONES(c) * 10)
#define HUNDREDS(c) (ONES(c) * 100)
#define THOUSANDS(c) (ONES(c) * 1000)

#define TWO_DIGIT_DECIMAL(s) \
    (TENS((s)[0]) + ONES((s)[1]))
#define THREE_DIGIT_DECIMAL(s) \
    (HUNDREDS((s)[0]) + TENS((s)[1]) + ONES((s)[2]))
#define FOUR_DIGIT_DECIMAL(s) \
    (THOUSANDS((s)[0]) + HUNDREDS((s)[1]) + TENS((s)[2]) + ONES((s)[3]))

static inline bool
isByr(StringView value) noexcept {
    if (value.size != 4) {
        return false;
    }
    uint32_t yr = FOUR_DIGIT_DECIMAL(value.data);
    return 1920 <= yr && yr <= 2002;
}

static inline bool
isIyr(StringView value) noexcept {
    if (value.size != 4) {
        return false;
    }
    uint32_t yr = FOUR_DIGIT_DECIMAL(value.data);
    return 2010 <= yr && yr <= 2020;
}

static inline bool
isEyr(StringView value) noexcept {
    if (value.size != 4) {
        return false;
    }
    uint32_t yr = FOUR_DIGIT_DECIMAL(value.data);
    return 2020 <= yr && yr <= 2030;
}

static inline bool
isHgt(StringView value) noexcept {
    if (value.size == 4) {
        uint32_t h = TWO_DIGIT_DECIMAL(value.data);
        return value[2] == 'i' && value[3] == 'n' && 59 <= h && h <= 76;
    }
    else if (value.size == 5) {
        uint32_t h = THREE_DIGIT_DECIMAL(value.data);
        return value[3] == 'c' && value[4] == 'm' && 150 <= h && h <= 193;
    }
    else {
        return false;
    }
}

static inline bool
isHcl(StringView value) noexcept {
    return value.size == 7 &&
           value[0] == '#' &&
           IS_HEX(value[1]) && IS_HEX(value[2]) && IS_HEX(value[3]) &&
           IS_HEX(value[4]) && IS_HEX(value[5]) && IS_HEX(value[6]);
}

static inline bool
isEcl(StringView value) noexcept {
    switch (TAG(value.data)) {
        case amb: case blu: case brn: case gry: case grn: case hzl: case oth:
            return true;
        default:
            return false;
    }
}

static inline bool
isPid(StringView value) noexcept {
    return value.size == 9 &&
           IS_DIGIT(value[0]) && IS_DIGIT(value[1]) && IS_DIGIT(value[2]) &&
           IS_DIGIT(value[3]) && IS_DIGIT(value[4]) && IS_DIGIT(value[5]) &&
           IS_DIGIT(value[6]) && IS_DIGIT(value[7]) && IS_DIGIT(value[8]);
}

int
main() noexcept {
    FileTokenStream<'\n'> lines;
    if (!lines.start("input")) {
        printf("Could not read input\n");
        return 1;
    }

    size_t id = 0;
    size_t numValid = 0;

    bool valid = true;

    for (StringView line = lines++; line.data; line = lines++) {
        if (line.size == 0) {
            if (valid) {
                numValid += id == 127;  // lowest 7 bits set
            }
            id = 0;
            valid = true;
            continue;
        }

        Tokens<' '> words = split<' '>(line);

        for (StringView word = words++; word.data && valid; word = words++) {
            StringView data = word.substr(4);
            switch (TAG(word.data)) {
                case byr:
                    if (!isByr(data)) {
                        valid = false;
                    }
                    else {
                        id |=  1;
                    }
                    break;
                case iyr:
                    if (!isIyr(data)) {
                        valid = false;
                    }
                    else {
                        id |=  2;
                    }
                    break;
                case eyr:
                    if (!isEyr(data)) {
                        valid = false;
                    }
                    else {
                        id |=  4;
                    }
                    break;
                case hgt:
                    if (!isHgt(data)) {
                        valid = false;
                    }
                    else {
                        id |=  8;
                    }
                    break;
                case hcl:
                    if (!isHcl(data)) {
                        valid = false;
                    }
                    else {
                        id |= 16;
                    }
                    break;
                case ecl:
                    if (!isEcl(data)) {
                        valid = false;
                    }
                    else {
                        id |= 32;
                    }
                    break;
                case pid:
                    if (!isPid(data)) {
                        valid = false;
                    }
                    else {
                        id |= 64;
                    }
                    break;
            }
        }
    }

    printf("%zu\n", numValid);

    return 0;
}
