#ifndef KDYCODE_H_
#define KDYCODE_H_

#include "ktype.h"
#include <time.h>

#ifndef KMAX_DYFUNCS
#define KMAX_DYFUNCS 16
#endif // KMAX_DYFUNCS

typedef u0 (*kDyfun)(u0);

typedef struct kDylib {
    u0 *os_handle;
    time_t stamp;
    c_str name;
    int num_bindings;
    struct {
        c_str dyfun_name;
        kDyfun *dyfun;
    } bindings[KMAX_DYFUNCS];
} kDylib;

u0 kDyBindLib(kDylib *self, c_str dylib_name);
b8 kDyBindFun(kDylib *self, c_str dyfun_name, kDyfun *dyfun);
b8 kDyRequestReload(kDylib *self);
u0 kDyCleanup(kDylib *self);

#endif // KDYCODE_H_
