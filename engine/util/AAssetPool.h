#ifndef AASSETPOOL_H
#define AASSETPOOL_H

#include "AHashmap.h"
#include "../renderer/AShader.h"
#include "../renderer/ATexture.h"
#include "../renderer/ASpritesheet.h"

class AAssetPool
{
public:
    static void addShader(AString resourceName, const char *vPath, const char *fPath);
    static AShader *getShader(AString resourceName);
    static void addTexture(AString resourceName, const char *path);
    static ATexture *getTexture(AString resourceName);
    static void addSpritesheet(AString resourceName, ASpritesheet *spritesheet);
    static ASpritesheet *getSpritesheet(AString resourceName);
private:
    static AHashmap<AShader *> shaders;
    static AHashmap<ATexture *> textures;
    static AHashmap<ASpritesheet *> spritesheets;
};

#endif
