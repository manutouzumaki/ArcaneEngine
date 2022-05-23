#ifndef ASPRITE_H
#define ASPRITE_H

#include "ATexture.h"
#include <glm/glm.hpp>

class ASprite
{
public:
    ASprite(ATexture *texture);
    ASprite(ATexture *texture, glm::vec2 *texCoords);
    ATexture *getTexture();
    glm::vec2 *getTexCoords();
private:
    ATexture *texture;
    glm::vec2 texCoords[4];
};

#endif
