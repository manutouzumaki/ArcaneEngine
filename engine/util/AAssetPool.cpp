#include "AAssetPool.h"

AHashmap<AShader *> AAssetPool::shaders;
AHashmap<ATexture *> AAssetPool::textures;
AHashmap<ASpritesheet *> AAssetPool::spritesheets;

void AAssetPool::addShader(AString resourceName, const char *vPath, const char *fPath)
{
    shaders.add(resourceName, new AShader(vPath, fPath));
}

AShader *AAssetPool::getShader(AString resourceName)
{
    return shaders[resourceName];
}

void AAssetPool::addTexture(AString resourceName, const char *path)
{
    textures.add(resourceName, new ATexture(path));
}

ATexture *AAssetPool::getTexture(AString resourceName)
{
    return textures[resourceName];
}

void AAssetPool::addSpritesheet(AString resourceName, ASpritesheet *spritesheet)
{
    spritesheets.add(resourceName, spritesheet);
}

ASpritesheet *AAssetPool::getSpritesheet(AString resourceName)
{
    return spritesheets[resourceName];
}
