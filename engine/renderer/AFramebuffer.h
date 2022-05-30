#ifndef AFRAMEBUFFER_H
#define AFRAMEBUFFER_H

#include "ATexture.h"

class AFramebuffer
{
public:
    AFramebuffer(int width, int height);
    ~AFramebuffer();
    void bind();
    void unbind();
    unsigned int getTextureID();
    unsigned int getFBO();
private:
    unsigned int FBO;
    ATexture *texture;
};

#endif
