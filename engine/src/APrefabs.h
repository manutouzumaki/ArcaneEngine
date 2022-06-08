#ifndef APREFABS_H
#define APREFABS_H

#include "AGameObject.h"
#include "../renderer/ASprite.h"

class APrefabs
{
public:
    static AGameObject *generateSpriteObject(ASprite *sprite, float sizeX, float sizeY);
    static AGameObject *generateObject();
    static AGameObject *generateGuy();
    static AGameObject *generateMario();
    static AGameObject *generateQuestionBlock();
};

#endif
