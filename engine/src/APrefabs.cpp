#include "APrefabs.h"
#include <glm/glm.hpp>
#include "../components/ASpriteComponent.h"
#include "../util/AString.h"

AGameObject *APrefabs::generateSpriteObject(ASprite *sprite, int sizeX, int sizeY)
{
    int NameSize = StringLength("Sprite_Object_Gen");
    char *name = (char *)malloc(NameSize + 1);
    StringCopy(name, NameSize, "Sprite_Object_Gen");
    name[NameSize] = '\0';

    AGameObject *block = new AGameObject(name, ATransform(glm::vec2(0.0f), glm::vec2((float)sizeX, (float)sizeY)), 1);
    block->addComponent("ASpriteComponent", new ASpriteComponent(sprite));
    return block;
}
