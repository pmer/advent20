#ifndef CC_STRING_POOL_H_
#define CC_STRING_POOL_H_

#include "util/string-view.h"
#include "util/string.h"

class StringPool {
 public:
     StringView
     operator <<(StringView src) noexcept;

 public:
     String buf;
};

#endif  // CC_STRING_POOL_H_
