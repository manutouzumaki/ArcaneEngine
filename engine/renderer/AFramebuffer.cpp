#include "AFramebuffer.h"
#include <glad/glad.h>

#include "../util/ADefines.h"

AFramebuffer::AFramebuffer(int width, int height)
{
    // generate framebuffer
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // create the texture to render the data to, and attach in to our render buffer
    texture = new ATexture(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getID(), 0);

    // create render buffer store the depth info
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Assert(!"ERROR: frame buffer is NOT complete"); 
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

AFramebuffer::~AFramebuffer()
{
    if(texture) delete texture;
}

void AFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void AFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int AFramebuffer::getTextureID()
{
    return texture->getID();
}

unsigned int AFramebuffer::getFBO()
{
    return FBO;
}

