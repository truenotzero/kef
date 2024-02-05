#ifndef KGL_H_
#define KGL_H_

#include <ktype.h>
#include <GL/glew.h>

#define KGL(call) call ; k__renderCheckGlImpl(__FILE__, __LINE__)
u0 k__renderCheckGlImpl(c_str file, int line);

#endif // KGL_H_
