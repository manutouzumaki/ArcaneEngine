#ifndef ASPRITESHEET_H
#define ASPRITESHEET_H

#include "ASprite.h"
#include "../util/AArray.h"

class ASpritesheet
{
public:
    ASpritesheet(ATexture *texture, int spriteWidth, int spriteHeight, int numSprites, int spacing);
    ~ASpritesheet();
    ASprite *getSprite(int index);
private:
    ATexture *texture;
    AArray<ASprite *> sprites;
};

#endif
