#include "ASprite.h"

ASprite::ASprite(ATexture *texture, int index)
{
    this->texture = texture;
    texCoords[0] = glm::vec2(1, 1);
    texCoords[1] = glm::vec2(1, 0);
    texCoords[2] = glm::vec2(0, 0);
    texCoords[3] = glm::vec2(0, 1);
    this->index = index;
}

ASprite::ASprite(ATexture *texture, glm::vec2 *texCoords, int index)
{
    this->texture = texture;
    memcpy(this->texCoords, texCoords, 4 * sizeof(glm::vec2));
    this->index = index;
}

ATexture *ASprite::getTexture()
{
    return texture;
}

glm::vec2 *ASprite::getTexCoords()
{
    return texCoords;
}

int ASprite::getIndex()
{
    return index;
}

const char *ASprite::getTileSheet()
{
    return tileSheet;
}

void ASprite::setTileSheet(const char *tileSheet)
{
    this->tileSheet = tileSheet;
}
