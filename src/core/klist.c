#include "klist.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct kListImpl {
    i32 size;
    i32 capacity;
    i32 elem_size;
    u8 data[];
} kListImpl;

#define DEFAULT_LIST_CAPACITY 32
#define LIST_GROW_FACTOR 2

static kListImpl *get_impl(kList self) {
    u8 *p_data = self;
    return (u0 *) (p_data - offsetof(kListImpl, data));
}

kList kListCreateImpl(i32 elem_size) {
    kListImpl *ret = malloc(sizeof(*ret) + DEFAULT_LIST_CAPACITY * elem_size);
    if (!ret) { return 0; }
    ret->size = 0;
    ret->capacity = DEFAULT_LIST_CAPACITY;
    ret->elem_size = elem_size;
    return &ret->data;
}

u0 kListDestroyImpl(kList self) {
    free(self);
}

kList kListReserve(kList list, i32 desired_capacity) {
    kListImpl *self = get_impl(list);
    i32 capacity = self->capacity;
    if (capacity >= desired_capacity) { return self; }
    while (capacity < desired_capacity) { capacity *= LIST_GROW_FACTOR; }
    kListImpl *ret = realloc(self, sizeof(*ret) + capacity * self->elem_size);
    if (!ret) { return 0; }
    ret->capacity = capacity;

    return &ret->data;
}
