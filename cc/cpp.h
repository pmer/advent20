#ifndef CC_CPP_H_
#define CC_CPP_H_

#include "options.h"
#include "util/string-view.h"
#include "util/string.h"

struct CppOutput {
    String code;
};

bool
cpp(CppOutput* output, ProgramOptions* options, StringView path) noexcept;

#endif  // CC_CPP_H_
