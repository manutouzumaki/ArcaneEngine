#include "AMousePicking.h"
#include <glad/glad.h>
#include "../util/ADefines.h"
#include <glm/glm.hpp>

AMousePicking::AMousePicking(int width, int height)
{
    if(!init(width, height))
    {
        Assert(!"ERROR: intializing AMousePicking");
    }
}
    
bool AMousePicking::init(int width, int height)
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &pickingTextureID);
    glBindTexture(GL_TEXTURE_2D, pickingTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingTextureID, 0);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Assert(!"ERROR: frame buffer is NOT complete");
        return false; 
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void AMousePicking::enableWritting()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void AMousePicking::disableWritting()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int AMousePicking::readPixel(int x, int y)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    float pixels[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
    return (int)(pixels[0]) - 1;
}

