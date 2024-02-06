#include "kutil.h"

#include <stdio.h>
#include <assert.h>

int kUtilReadTextFile(c_str filename, char *buf, unsigned buf_sz) {
    assert(buf);
    assert(buf_sz > 1);

    FILE *f = fopen(filename, "r");
    if (!f) { return 0; }
    int ret = fread(buf, sizeof(*buf), buf_sz - 1, f);
    fclose(f);
    return ret;
}


// impl for ktype.h
#include <ktype.h>

int kGetTypeSize(enum kType type) {
    switch (type) {
        case KTYPE_F32:
            return sizeof(f32);
        case KTYPE_VEC3F:
            return sizeof(kVec3f);
        case KTYPE_MAT4F:
            return sizeof(kMat4f);
        default:
            return -1;
    }
}
