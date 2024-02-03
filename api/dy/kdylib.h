#ifndef KDYLIB_H_
#define KDYLIB_H_

#ifdef _WIN32
#define KDYFUN __declspec(dllexport)
#else
#define KDYFUN
#endif // _WIN32

#ifndef KDYNOCALLBACKS
// define callbacks for loading & unloading

#include <ktype.h>

KDYFUN u0 dylib_setup(u0);
KDYFUN u0 dylib_cleanup(u0);

#endif // KDYNOCALLBACKS


#endif // KDYLIB_H_
