#ifndef KWINDOW_H_
#define KWINDOW_H_

#include "ktype.h"


b8 kWindowCreate(void);
void kWindowDestroy(void);
void kWindowLoop(void);
u0 kWindowSetCursorVisible(b8 is_visible);

// user-defined
extern void kWindowUpdate(void);
extern void kWindowRender(void);

u0 *k__windowRawHandle(u0);

#endif // KWINDOW_H_
