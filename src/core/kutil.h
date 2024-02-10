#ifndef KUTIL_H_
#define KUTIL_H_

#include <ktype.h>

// reads a text file as a null terminated string
// reads up to buf_sz - 1 characters
// returns the length of the string
int kUtilReadTextFile(c_str filename, char *buf, unsigned buf_sz);

#define K_VA_ARRAY(type, ...) ((type[]) { __VA_ARGS__ })
#define K_VA_ARRAY_SZ(type, ...) (sizeof(K_VA_ARRAY(type, __VA_ARGS__)) / sizeof(*K_VA_ARRAY(type, __VA_ARGS__)))

#endif // KUTIL_H_
