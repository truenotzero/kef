#ifndef KHOTRELOAD_H_
#define KHOTRELOAD_H_

#include "ktype.h"

typedef void *kDylib;

b8 kLoadDylib(kDylib *self, c_str dylib_name);
u0 kFreeDylib(kDylib *self);
b8 kGetDyfun(kDylib const self, c_str dyfun_name, void **dyfun);

#endif // KHOTRELOAD_H_
