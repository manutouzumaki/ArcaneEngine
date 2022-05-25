#ifndef ASPRITE_H
#define ASPRITE_H

#include "ATexture.h"
#include <glm/glm.hpp>

class ASprite
{
public:
    ASprite(ATexture *texture, int index);
    ASprite(ATexture *texture, glm::vec2 *texCoords, int index);
    ATexture *getTexture();
    glm::vec2 *getTexCoords();
    int getIndex();
    const char *getTileSheet();
    void setTileSheet(const char *tileSheet);
private:
    int index;
    const char *tileSheet;
    ATexture *texture;
    glm::vec2 texCoords[4];
};

#endif
