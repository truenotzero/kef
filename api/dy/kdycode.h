#ifndef KDYCODE_H_
#define KDYCODE_H_

#include "ktype.h"
#include <time.h>

#ifndef KMAX_DYFUNCS
#define KMAX_DYFUNCS 16
#endif // KMAX_DYFUNCS


/// Generic function pointer type
/// Needed since ANSI C does not guarantee
/// That sizeof(void*) == sizeof(void(*)(void))
typedef u0 (*kDyfun)(u0);

/// A handle to a dynamically loaded library/plugin
/// Zero-Initialize to use
/// All fields are for private use, instead, use the `kDy`
/// family of functions
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

/// Binds a dynamic library handle to a physical library on disk
u0 kDyBindLib(kDylib *self, c_str dylib_name);

/// Binds a function in the library to a function pointer
b8 kDyBindFun(kDylib *self, c_str dyfun_name, kDyfun *dyfun);

/// Reloads the dynamic library if a change is detected
b8 kDyRequestReload(kDylib *self);

/// Clean up the dynamic library handle
u0 kDyCleanup(kDylib *self);

#endif // KDYCODE_H_
