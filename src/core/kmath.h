#ifndef KMATH_H_
#define KMATH_H_

#include <ktype.h>
#include "kutil.h"

/* matrix creation */

#define K_PIF 3.14159265f
#define kRad(a) (a)
#define kDegf(a) (a * (K_PIF / 180.0f))

// basic matrix
kMat4f kMatIdentity4f(u0);
// scale by desired scalars in each axis
kMat4f kMatScale4f(f32 x_scale, f32 y_scale, f32 z_scale);
// translate by the amount specified
kMat4f kMatTranslate4f(f32 x_trans, f32 y_trans, f32 z_trans);
// TODO: orthographic projection matrix

// creates a symmetrical frustum (perspective) transform matrix by defining its corners
// used to transform from camera-space to clip-space
// for more details, read:
// http://www.songho.ca/opengl/gl_projectionmatrix.html
// all parameters must be positive!
kMat4f kMatFrustum4f(f32 right, f32 top, f32 near, f32 far);
// creates a perspective transformation matrix as defined by a view angle (fov) and a canvas aspect ratio
// all parameters must be positive!
kMat4f kMatPerspective4f(f32 fov, f32 aspect_ratio, f32 near, f32 far);
// rotates by t radians around the axis a
// axis must be normalized
kMat4f kMatRotate4f(kVec3f a, f32 t);

/* matrix operations */

// multiplies matrices together
// effectively, composes multiple matrix operations into one
// kMatMul4f(m2, m1) * v
// applies m1 to v, then applies m2 to v
#define kMatMul4f(...) kMatMul4fImpl(K_VA_ARRAY(kMat4f, __VA_ARGS__), K_VA_ARRAY_SZ(kMat4f, __VA_ARGS__))
kMat4f kMatMul4fImpl(kMat4f *m, i32 amount);

// pretty print the matrix to standard out
u0 kMatPrint4f(kMat4f m);

/* vector operations */

#define K_VEC3F_ZERO ((kVec3f) {{ 0.0f, 0.0f, 0.0f }})
#define K_VEC3F_RIGHT ((kVec3f) {{ 1.0f, 0.0f, 0.0f }})
#define K_VEC3F_LEFT ((kVec3f) {{ -1.0f, 0.0f, 0.0f }})
#define K_VEC3F_UP ((kVec3f) {{ 0.0f, 1.0f, 0.0f }})
#define K_VEC3F_DOWN ((kVec3f) {{ 0.0f, -1.0f, 0.0f }})
#define K_VEC3F_FORWARD ((kVec3f) {{ 0.0f, 0.0f, 1.0f }})
#define K_VEC3F_BACK ((kVec3f) {{ 0.0f, 0.0f, -1.0f }})

// compute the length squared of this vector
// useful when comparing the lengths of two vectors
// (since len > 0 therefore len2a > len2b iff lena > lenb)
// or checking if the length is 0 or 1 (as 0*0 = 0 and 1*1 = 1)
f32 kVecLen23f(kVec3f v);

// comptue vector length (magnitude)
f32 kVecLen3f(kVec3f v);

// normalize a vector, turning it into a unit vector in a direction
kVec3f kVecNorm3f(kVec3f v);

// dot product
f32 kVecDot4f(kVec4f lhs, kVec4f rhs);
// matrix-vector product
kVec4f kVecMatMul4f(kMat4f lhs, kVec4f rhs);
// pretty print the vector to standard out
u0 kVecPrint4f(kVec4f m);

#endif // KMATH_H_

