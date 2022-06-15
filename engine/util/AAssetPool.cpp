#include <stdio.h>
#include "AAssetPool.h"

AHashmap<AShader *> AAssetPool::shaders;
AHashmap<ATexture *> AAssetPool::textures;
AHashmap<ASpritesheet *> AAssetPool::spritesheets;
AHashmap<ASound *> AAssetPool::sounds;

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
    ATexture *tex = textures[resourceName];
    return tex;
}

void AAssetPool::addSpritesheet(AString resourceName, ASpritesheet *spritesheet)
{
    spritesheets.add(resourceName, spritesheet);
}

ASpritesheet *AAssetPool::getSpritesheet(AString resourceName)
{
    return spritesheets[resourceName];
}

void AAssetPool::addSound(AString resourceName, ASound *sound)
{
    sounds.add(resourceName, sound);
}

ASound *AAssetPool::getSound(AString resourceName)
{
    return sounds[resourceName];
}
