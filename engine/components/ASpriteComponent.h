#ifndef ASPRITECOMPONENT_H
#define ASPRITECOMPONENT_H

#include "../renderer/ATexture.h"
#include "../renderer/ASprite.h"
#include "../src/AGameObject.h"
#include <glm/glm.hpp>

class ASpriteComponent : public AComponent
{
public:
    ASpriteComponent(ASprite *sprite);
    ASpriteComponent(glm::vec4 color);
    ~ASpriteComponent();
    void start() override;
    void update(float dt) override;
    void imgui() override;
    glm::vec4 getColor();
    ATexture *getTexture();
    glm::vec2 *getTexCoords();
    void setColor(glm::vec4 color);
    void setSprite(ASprite *sprite);
    bool isDirty;
private:
    glm::vec4 color;
    ASprite *sprite;
    ATransform lastTransform;
};

#endif
