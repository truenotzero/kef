#ifndef KTEXTURE_H_
#define KTEXTURE_H_

#include <ktype.h>

typedef struct kRenderTexture {
    u32 obj;
} kRenderTexture;

b8 kRenderTextureCreate(kRenderTexture *self);
b8 kRenderTextureDestroy(kRenderTexture *self);
b8 kRenderTextureLoad(kRenderTexture *self, c_str texture_name);
u0 kRenderTextureUse(kRenderTexture *self);

#endif // KTEXTURE_H_
