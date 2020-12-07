// Time: O(n * m)
// Memory: O(n * m)

#include "util/fnv.h"
#include "util/hashtable.h"
#include "util/hash.h"
#include "util/int.h"
#include "util/io.h"
#include "util/string-view.h"
#include "util/string2.h"

class StringPool {
 public:
     StringView
     operator <<(StringView src) noexcept;

 public:
     String buf;
};

StringView
StringPool::operator <<(StringView src) noexcept {
    StringView dst(buf.data + buf.size, src.size);
    buf << src;
    return dst;
}

// Directed (hopefully acyclic) graph node.
struct Bag {
    Vector<Bag*> parents;  // Edges in.
};

size_t
hash_(Bag* const& bag) noexcept {
    return fnvHash(reinterpret_cast<const char*>(bag), sizeof(bag));
}

static Bag*
getBag(StringView needle, StringPool& namePool,
       Hashmap<StringView, Bag*>& names, Vector<Bag>& bags) noexcept {
    Bag* target;

    // FIXME: Replace .tryAt() and operator[] with a single operator[].

    // Try searching for it.
    Bag** found = names.tryAt(needle);
    if (found) {
        // It already existed.
        return *found;
    }

    // Allocate a new Bag graph node.
    bags.push_back(Bag());
    target = &bags[bags.size - 1];

    // Allocate permanent storage for this name in case the current
    // file chunk changes & the memory holding this name is overridden.
    StringView name = namePool << needle;

    // This name refers to this bag from now on.
    names[name] = target;

    return target;
}

int
main() noexcept {
    // Lazy iterator that yields one line of the file at a time.
    ReadLines lines("input");
    if (!lines) {
        serr << "Could not read input\n";
        return 1;
    }

    StringPool namePool;
    namePool.buf.reserve(20 * 1024);

    Hashmap<StringView, Bag*> names;
    names.reserve(2 * 1024);

    Vector<Bag> bags;
    bags.reserve(1024);

    for (StringView line = lines.next(); line.data; line = lines.next()) {
        StringView outer = line.substr(0, line.find(" bags"));

        Bag* parent = getBag(outer, namePool, names, bags);

        StringView contents = line.substr(outer.size + 13);  // " bags contain"
        if (contents[1] == 'n') {
            // contents = " no other bags." so continue to next line
            continue;
        }

        Splits<','> items(contents);

        for (StringView item = items.next(); item.data; item = items.next()) {
            item = item.substr(3);  // " 5 "

            size_t flavor = item.find(' ');
            size_t color = item.find(' ', flavor + 1);

            item = item.substr(0, color);

            Bag* child = getBag(item, namePool, names, bags);
            child->parents.push_back(parent);
        }
    }

    StringView goldName("shiny gold");

    if (!names.contains(goldName)) {
        serr << "Couldn't find any shiny gold bags\n";
        return 1;
    }

    Bag* goldBag = names[goldName];

    Hashmap<Bag*, bool> seen(2 * 1024);
    Vector<Bag*> queue;
    queue.reserve(bags.size);

    seen[goldBag] = true;
    queue.push_back(goldBag);

    for (size_t i = 0; i < queue.size; i++) {
        Bag* child = queue[i];
        for (Bag* parent : child->parents) {
            bool& seenThis = seen[parent];
            if (seenThis == false) {
                seenThis = true;
                queue.push_back(parent);
            }
        }
    }

    size_t canContain = queue.size - 1;

    sout << canContain << '\n';

    return 0;
}
