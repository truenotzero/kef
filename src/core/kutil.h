#ifndef KUTIL_H_
#define KUTIL_H_

#include <ktype.h>

// reads a text file as a null terminated string
// reads up to buf_sz - 1 characters
// returns the length of the string
int kUtilReadTextFile(c_str filename, char *buf, unsigned buf_sz);

#endif // KUTIL_H_
