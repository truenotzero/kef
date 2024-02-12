#include "kmath.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#define is_normalized(v) (kVecLen23f(v) - 1.0f < 0.001f)

// vector: len2, len, normalize, scalar multiplication, dot product
// vector3f: cross product

// matrix: multiplication

kMat4f kMatIdentity4f(u0) {
    kMat4f ret = {0};
    for (int i = 0; i < 4; ++i) {
        ret.xy[i][i] = 1;
    }
    return ret;
}

kMat4f kMatScale(f32 x_scale, f32 y_scale, f32 z_scale) {
    kMat4f ret = kMatIdentity4f();
    ret.xy[0][0] = x_scale;
    ret.xy[1][1] = y_scale;
    ret.xy[2][2] = z_scale;
    return ret;
}

kMat4f kMatTranslate(f32 x_trans, f32 y_trans, f32 z_trans) {
    kMat4f ret = kMatIdentity4f();
    ret.xy[3][0] = x_trans;
    ret.xy[3][1] = y_trans;
    ret.xy[3][2] = z_trans;
    return ret;
}

kMat4f kMatRotate(kVec3f a, f32 t) {
    // vector must be normalized
    assert(is_normalized(a));

    kMat4f ret = kMatIdentity4f();
    f32 x = a.x;
    f32 y = a.y;
    f32 z = a.z;
    f32 c = cosf(t);
    f32 s = sinf(t);
    // left column
    ret.xy[0][0] = (1 - c) * x * x + c;
    ret.xy[0][1] = (1 - c) * x * y + s * z;
    ret.xy[0][2] = (1 - c) * x * z - s * y;
    // middle column
    ret.xy[1][0] = (1 - c) * x * y - s * z;
    ret.xy[1][1] = (1 - c) * y * y + c;
    ret.xy[1][2] = (1 - c) * y * z + s * x;
    // right column
    ret.xy[2][0] = (1 - c) * x * z + s * y;
    ret.xy[2][1] = (1 - c) * y * z - s * x;
    ret.xy[2][2] = (1 - c) * z * z + c;
    return ret;
}

kMat4f kMatFrustum(f32 right, f32 top, f32 near, f32 far) {
    assert(right > 0.0f);
    assert(top > 0.0f);
    assert(near > 0.0f);
    assert(far > 0.0f);

    f32 t = -1.0f / (far - near);
    kMat4f ret = kMatScale(near / right, near / top, (far + near) * t);
    ret.xy[3][2] = 2.0f * far * near * t;
    ret.xy[2][3] = -1.0f;
    ret.xy[3][3] = 0.0f;
    return ret;
}

kMat4f kMatPerspective(f32 fov, f32 aspect_ratio, f32 near, f32 far) {
    f32 top = tanf(fov * 0.5f) * near;
    f32 right = top * aspect_ratio;
    return kMatFrustum(right, top, near, far);
}

kMat4f kMatLookAt(kVec3f cam_pos, kVec3f look_pos) {
    // calculate the camera's right and up vectors (orthonormal basis for camera space)
    // uses the Gram-Schmidt process
    // by convention the camera's local forward (positive z) direction is behind it
    // we can get this by inverting the direction we're looking at
    kVec3f forward = kVecNorm3f(kVecSub3f(cam_pos, look_pos));
    printf("forward len2: %.2f\n", kVecLen23f(forward));
    assert(is_normalized(forward));
    kVec3f right = kVecNorm3f(kVecCrossf(K_VEC3F_UP, forward));
    kVec3f up = kVecCrossf(forward, right);
    assert(is_normalized(up));
    // create a transposed rotation matrix
    // practically, an inversed rotation matrix is needed
    // but since the matrix is orthonormal, M^T = M^-1
    // saving computing power
    // the inverse is needed since the camera rotates in the opposite
    // direction to the world
    // the rotation (NOT THE INVERSE!) matrix can be created by mapping world space
    // onto camera space (which is why we calculated the orthonormal basis for camera space earlier)
    // then it's just a matter of constructing [r u f]
    // where r,u,f are the camera space's orthonormal basis vectors
    // and transposing it, effectively getting the inverse rotation matrix
    kMat4f rot = kMatIdentity4f();
    rot.xy[0][0] = right.x;
    rot.xy[1][0] = right.y;
    rot.xy[2][0] = right.z;
    rot.xy[0][1] = up.x;
    rot.xy[1][1] = up.y;
    rot.xy[2][1] = up.z;
    rot.xy[0][2] = forward.x;
    rot.xy[1][2] = forward.y;
    rot.xy[2][2] = forward.z;
    kMat4f trans = kMatTranslate(-cam_pos.x, -cam_pos.y, -cam_pos.z);
    // to convert world space to camera space
    // the camera is placed at 0,0,0 so the world geometry must be offset
    // by -pos
    // next, we apply the inverse of the camera's rotation to the world's geometry
    return kMatMul4f(rot, trans);
}

static kMat4f mul_mat4f(kMat4f lhs, kMat4f rhs) {
    kMat4f ret;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            f32 sum = 0;
            for (int e = 0; e < 4; ++e) {
                sum += lhs.xy[e][y] * rhs.xy[x][e];
            }
            ret.xy[x][y] = sum;
        }
    }
    return ret;
}

kMat4f kMatMul4fImpl(kMat4f *m, i32 amount) {
    kMat4f ret = kMatIdentity4f();
    for (i32 i = 0; i < amount; ++i) {
        ret = mul_mat4f(ret, m[i]);
    }
    return ret;
}

u0 kMatPrint4f(kMat4f m) {
    for (int y = 0; y < 4; ++y) {
        printf("[ %.2f %.2f %.2f %.2f ]\n", m.xy[0][y], m.xy[1][y], m.xy[2][y], m.xy[3][y]);
    }
    printf("\n");
}

f32 kVecLen23f(kVec3f v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

f32 kVecLen3f(kVec3f v) {
    return sqrtf(kVecLen23f(v));
}

kVec3f kVecNorm3f(kVec3f v) {
    f32 isqrt = 1.0f / kVecLen3f(v);
    kVec3f ret;
    for (int i = 0; i < 3; ++i) {
        ret.e[i] = v.e[i] * isqrt;
    }
    return ret;
}

kVec3f kVecCrossf(kVec3f lhs, kVec3f rhs) {
    return (kVec3f) {{
        .x = lhs.y * rhs.z - lhs.z * rhs.y,
        .y = lhs.z * rhs.x - lhs.x * rhs.z,
        .z = lhs.x * rhs.y - lhs.y * rhs.x,
    }};
}

kVec2f kVecScale2f(f32 scalar, kVec2f v) {
    return (kVec2f) {{
        .x = scalar * v.x,
        .y = scalar * v.y,
    }};
}

kVec3f kVecScale3f(f32 scalar, kVec3f v) {
    return (kVec3f) {{
        .x = scalar * v.x,
        .y = scalar * v.y,
        .z = scalar * v.z,
    }};
}

kVec3f kVecAdd3f(kVec3f lhs, kVec3f rhs) {
    return (kVec3f) {{
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
        .z = lhs.z + rhs.z,
    }};
}

kVec2f kVecSub2f(kVec2f lhs, kVec2f rhs) {
    return (kVec2f) {{
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
    }};
}

kVec3f kVecSub3f(kVec3f lhs, kVec3f rhs) {
    return (kVec3f) {{
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
        .z = lhs.z - rhs.z,
    }};
}

f32 kVecDot4f(kVec4f lhs, kVec4f rhs) {
    f32 ret = 0;
    for (int i = 0; i < 4; ++i) {
        ret += lhs.e[i] * rhs.e[i];
    }
    return ret;
}

kVec4f kVecMatMul4f(kMat4f lhs, kVec4f rhs) {
    kVec4f ret;
    for (int y = 0; y < 4; ++y) {
        float sum = 0;
        for (int e = 0; e < 4; ++e) {
            sum += lhs.xy[e][y] * rhs.e[e];
        }
        ret.e[y] = sum;
    }
    return ret;
}

u0 kVecPrint4f(kVec4f m) {
    printf("[ %.2f %.2f %.2f %.2f ]\n", m.e[0], m.e[1], m.e[2], m.e[3]);
    printf("\n");
}

kVec4f kVecPoint4f(kVec3f v) {
    return (kVec4f) {{
        .x = v.x,
        .y = v.y,
        .z = v.z,
        .w = 1.0f,
    }};
}

kVec4f kVecDirection4f(kVec3f v) {
    return (kVec4f) {{
        .x = v.x,
        .y = v.y,
        .z = v.z,
        .w = 0.0f,
    }};
}

#define K_EPS 0.001f

b8 kVecIsUnit3f(kVec3f v) {
    return (kVecLen23f(v) - 1.0f) < K_EPS;
}

b8 kVecIsZero3f(kVec3f v) {
    return kVecLen23f(v) < K_EPS;
}
