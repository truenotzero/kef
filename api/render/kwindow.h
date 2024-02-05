#ifndef KWINDOW_H_
#define KWINDOW_H_

#include "ktype.h"


b8 kWindowCreate(void);
void kWindowDestroy(void);
void kWindowLoop(void);

// user-defined
extern void kWindowUpdate(void);
extern void kWindowRender(void);

#endif // KWINDOW_H_