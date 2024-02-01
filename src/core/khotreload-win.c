#ifdef _WIN32
#include "khotreload.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// static HMODULE native(kDylib self) {
//     return (HMODULE) self;
// }

b8 kLoadDylib(kDylib *self, c_str dylib_name) {
    if (!self) return kfalse;

    if (*self != NULL) {
        kFreeDylib(self);
    }

    *self = LoadLibraryA(dylib_name);
    if (*self == NULL) {
        return kfalse;
    }

    return ktrue;
}

u0 kFreeDylib(kDylib *self) {
    if (!self) return;

    FreeLibrary(*self);
    *self = NULL;
}

b8 kGetDyfun(kDylib const self, c_str dyfun_name, void **dyfun) {
    if (!self) return kfalse;
    if (!dyfun) return kfalse;

    *dyfun = (void *) GetProcAddress(self, dyfun_name);
    return *dyfun != NULL;
}

#endif // _WIN32
