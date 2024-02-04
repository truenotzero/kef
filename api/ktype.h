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

#endif // KTYPE_H_
