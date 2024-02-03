#ifdef _WIN32

#include <dy/kdycode.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <ktype.h>

#include <stdio.h>

// find library by name on disk, return 0 on error
HMODULE kDyPlatLoadLib(c_str lib_name) {
    return LoadLibraryA(lib_name);
}

// unload a previously loaded library
extern u0 kDyPlatUnloadLib(HMODULE h) {
    FreeLibrary(h);
}

// locate a function fun_name in the os handle, return 0 on error
extern kDyfun kDyPlatGetDyFun(HMODULE h, c_str fun_name) {
    return (kDyfun) GetProcAddress(h, fun_name);
}

#endif // _WIN32
