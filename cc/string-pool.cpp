#include "string-pool.h"

#include "util/string-view.h"

StringView
StringPool::operator <<(StringView src) noexcept {
    buf << src;
    return StringView(buf.data + buf.size - src.size, src.size);
}
