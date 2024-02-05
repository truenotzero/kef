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
