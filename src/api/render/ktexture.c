#include <render/ktexture.h>

#include <assert.h>
#include <render/kgl.h>

#include "core/stb_image.h"

b8 kRenderTextureCreate(kRenderTexture *self) {
    assert(self);
    glGenTextures(1, &self->obj);
    return ktrue;
}

b8 kRenderTextureDestroy(kRenderTexture *self) {
    assert(self);
    glDeleteTextures(1, &self->obj);
    self->obj = 0;
    return ktrue;
}

b8 kRenderTextureLoad(kRenderTexture *self, c_str texture_name) {
    assert(self);
    assert(self->obj != 0);
    stbi_set_flip_vertically_on_load(1);
    int width, height, _channels;
    void *img_data = stbi_load(texture_name, &width, &height, &_channels, 0);
    if (!img_data) { return kfalse; }

    glBindTexture(GL_TEXTURE_2D, self->obj);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(img_data);
    return ktrue;
}

u0 kRenderTextureUse(kRenderTexture *self) {
    assert(self);
    assert(self->obj != 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self->obj);
}
