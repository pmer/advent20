// Time: O(n)
// Memory: O(1)
//
// Processes 250 MB/sec on an Intel Core i5-1030NG7.

#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

static constexpr const uint32_t byr = ('b' << 0) + ('y' << 8) + ('r' << 16);
static constexpr const uint32_t iyr = ('i' << 0) + ('y' << 8) + ('r' << 16);
static constexpr const uint32_t eyr = ('e' << 0) + ('y' << 8) + ('r' << 16);
static constexpr const uint32_t hgt = ('h' << 0) + ('g' << 8) + ('t' << 16);
static constexpr const uint32_t hcl = ('h' << 0) + ('c' << 8) + ('l' << 16);
static constexpr const uint32_t ecl = ('e' << 0) + ('c' << 8) + ('l' << 16);
static constexpr const uint32_t pid = ('p' << 0) + ('i' << 8) + ('d' << 16);
static constexpr const uint32_t cid = ('c' << 0) + ('i' << 8) + ('d' << 16);

static uint32_t
tag(StringView s) noexcept {
    return *reinterpret_cast<const uint32_t*>(s.data) & 0x00FFFFFF;
}

int
main() noexcept {
    // Lazy iterator that yields one line of the file at a time.
    ReadLines lines("4");
    if (!lines) {
        serr << "Could not read input\n";
        return 1;
    }

    size_t id = 0;
    size_t numValid = 0;

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        if (line.size == 0) {
            numValid += id == 127;  // lowest 7 bits set
            id = 0;
            continue;
        }

        Splits<' '> words(line);

        for (StringView word = words.next(); word.data; word = words.next()) {
            switch (tag(word)) {
                case byr: id |=  1; break;
                case iyr: id |=  2; break;
                case eyr: id |=  4; break;
                case hgt: id |=  8; break;
                case hcl: id |= 16; break;
                case ecl: id |= 32; break;
                case pid: id |= 64; break;
            }
        }
    }

    numValid += id == 127;  // lowest 7 bits set

    sout << numValid << '\n';

    return 0;
}
