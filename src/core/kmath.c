#include "kmath.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

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

kMat4f kMatScale4f(f32 x_scale, f32 y_scale, f32 z_scale) {
    kMat4f ret = kMatIdentity4f();
    ret.xy[0][0] = x_scale;
    ret.xy[1][1] = y_scale;
    ret.xy[2][2] = z_scale;
    return ret;
}

kMat4f kMatTrans4f(f32 x_trans, f32 y_trans, f32 z_trans) {
    kMat4f ret = kMatIdentity4f();
    ret.xy[3][0] = x_trans;
    ret.xy[3][1] = y_trans;
    ret.xy[3][2] = z_trans;
    return ret;
}

kMat4f kMatRotate4f(kVec3f a, f32 t) {
    // vector must be normalized
    assert(kVecLen23f(a) - 1.0f < 0.001f);

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

kMat4f kMatFrustum4f(f32 left_right, f32 top_bottom, f32 near, f32 far) {
    assert(left_right > 0.0f);
    assert(top_bottom > 0.0f);
    assert(near > 0.0f);
    assert(far > 0.0f);
    
    f32 t = -1.0f / (far - near);
    kMat4f ret = kMatScale4f(near / left_right, near / top_bottom, (far + near) * t);
    ret.xy[3][2] = 2.0f * far * near * t;
    ret.xy[2][3] = -1;
    return ret;
}

kMat4f kMatMul4f(kMat4f lhs, kMat4f rhs) {
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
    f32 isqrt = 1.0f / kVecLen23f(v);
    kVec3f ret;
    for (int i = 0; i < 3; ++i) {
        ret.e[i] = v.e[i] * isqrt;
    }
    return ret;
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
