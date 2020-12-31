#ifndef CC_OPTIONS_H_
#define CC_OPTIONS_H_

#include "util/string.h"
#include "util/vector.h"

struct ProgramOptions {
    Vector<String> includePaths;
};

#endif  // CC_OPTIONS_H_
