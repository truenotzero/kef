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

typedef union kVec3f {
    float a[3];
    struct { float x, y, z; };
    struct { float r, g, b; };
} kVec3f;

enum kType {
    KTYPE_F32,
    KTYPE_VEC3F,
};

#define kGetType(expr) \
    _Generic( (expr) \
    , f32: KTYPE_F32 \
    , kVec3f: KTYPE_VEC3F \
    )

int kGetTypeSize(enum kType);

#endif // KTYPE_H_
