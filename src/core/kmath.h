#ifndef KMATH_H_
#define KMATH_H_

#include <ktype.h>

/* matrix creation */

#define K_PIF 3.14159265f
#define kRad(a) (a)
#define kDegf(a) (a * (K_PIF / 180.0f))

// basic matrix
kMat4f kMatIdentity4f(u0);
// scale by desired scalars in each axis
kMat4f kMatScale4f(f32 x_scale, f32 y_scale, f32 z_scale);
// translate by the amount specified
kMat4f kMatTrans4f(f32 x_trans, f32 y_trans, f32 z_trans);
// orthographic projection matrix
// TODO

// creates a symmetrical frustum (perspective) transform matrix by defining its corners
// used to transform from camera-space to clip-space
// for more details, read:
// http://www.songho.ca/opengl/gl_projectionmatrix.html
// all parameters must be positive!
kMat4f kMatFrustum4f(f32 left_right, f32 top_bottom, f32 near, f32 far);

// rotates by t radians around the axis a
// axis must be normalized
kMat4f kMatRotate4f(kVec3f a, f32 t);

/* matrix operations */

// multiplies two matrices
// effectively, composes the two matrix operations into one
// kMatMul4f(lhs, rhs) * v
// applies rhs to v, then applies lhs to v
kMat4f kMatMul4f(kMat4f lhs, kMat4f rhs);
// pretty print the matrix to standard out
u0 kMatPrint4f(kMat4f m);

/* vector operations */
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

