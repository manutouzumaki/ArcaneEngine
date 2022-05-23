#include "ASpritesheet.h"
#include <glm/glm.hpp>

ASpritesheet::ASpritesheet(ATexture *texture, int spriteWidth, int spriteHeight, int numSprites, int spacing)
{
    this->texture = texture;
    int currentX = 0;
    int currentY = texture->getHeight() - spriteHeight;
    for(int i = 0; i < numSprites; ++i)
    {
        float topY = (currentY + spriteHeight) / (float)texture->getHeight();
        float rightX = (currentX + spriteWidth) / (float)texture->getWidth();
        float leftX = currentX / (float)texture->getWidth();
        float bottomY = currentY / (float)texture->getHeight();

        glm::vec2 texCoords[4];
        texCoords[0] = glm::vec2(rightX, topY);
        texCoords[1] = glm::vec2(rightX, bottomY);
        texCoords[2] = glm::vec2(leftX, bottomY);
        texCoords[3] = glm::vec2(leftX, topY);
        sprites.add(new ASprite(texture, texCoords));

        currentX += spriteWidth + spacing;
        if(currentX >= texture->getWidth())
        {
            currentX = 0;
            currentY -= spriteHeight + spacing;
        }
    }
}

ASpritesheet::~ASpritesheet()
{
    for(int i = 0; i < sprites.size(); ++i)
    {
        delete sprites[i];
    }
}

ASprite *ASpritesheet::getSprite(int index)
{
    return sprites[index];
}
