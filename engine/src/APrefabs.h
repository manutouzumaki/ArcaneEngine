#ifndef APREFABS_H
#define APREFABS_H

#include "AGameObject.h"
#include "../renderer/ASprite.h"

class APrefabs
{
public:
    static AGameObject *generateSpriteObject(ASprite *sprite, int sizeX, int sizeY);
};

#endif
