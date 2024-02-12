#ifndef KTYPE_H_
#define KTYPE_H_

typedef void u0, i0;

typedef unsigned char u8;
typedef signed char i8;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned int u32;
typedef signed int i32;
typedef unsigned long long u64;
typedef signed long long i64;

typedef float f32;

// A boolean type implies that
// false = 0
// true = non-zero
typedef char b8;
#define kfalse 0
#define ktrue 1

typedef char const *c_str;

typedef union kVec2f {
    struct { f32 x, y; };
    struct { f32 u, v; };
    struct { f32 yaw, pitch; };
    f32 e[1];
} kVec2f;

typedef union kVec3f {
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };
    f32 e[1];
} kVec3f;

typedef union kVec4f {
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };
    f32 e[1];
} kVec4f;

typedef union kMat4f {
    struct { f32 xy[4][4]; };
    struct { kVec4f v[4]; };
    f32 e[1];
} kMat4f;

enum kType {
    KTYPE_F32,
    KTYPE_VEC3F,
    KTYPE_MAT4F,
};

#define kGetType(expr) \
    _Generic( (expr) \
    , f32: KTYPE_F32 \
    , kVec3f: KTYPE_VEC3F \
    , kMat4f: KTYPE_MAT4F \
    )

int kGetTypeSize(enum kType);

#endif // KTYPE_H_
