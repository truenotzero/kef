#ifndef KGL_H_
#define KGL_H_

#include <ktype.h>
#include <GL/glew.h>

/// verifies a call to OpenGL, all raw GL calls should be wrapped in this
#define KGL(call) call ; k__renderCheckGlImpl(__FILE__, __LINE__)
/// for internal use
u0 k__renderCheckGlImpl(c_str file, int line);

#endif // KGL_H_
