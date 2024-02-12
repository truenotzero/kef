#ifndef K_LIST_H_
#define K_LIST_H_

#include <ktype.h>

typedef u0 *kList;

#define kListCreate(type) kListCreateImpl(sizeof(type))
kList kListCreateImpl(i32 elem_size);
#define kListDestroy(self) kListDestroyImpl(self)
u0 kListDestroyImpl(kList self);

// Reserves at least desired_capacity elements of space in the list
kList kListReserve(kList self, i32 desired_capacity);

#endif // K_LIST_H_
