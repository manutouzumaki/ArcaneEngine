#ifndef ATRANSLATEGIZMO_H
#define ATRANSLATEGIZMO_H

#include "../src/AComponent.h"
#include "ASpriteComponent.h"
#include "../editor/APropertiesWindow.h"

#include <glm/glm.hpp>

class AGameObject;

class ATranslateGizmo : public AComponent
{
private:
    glm::vec4 xAxisColor = glm::vec4(1, 0, 0, 1);
    glm::vec4 xAxisColorHover = glm::vec4(0);
    glm::vec4 yAxisColor = glm::vec4(0, 0, 1, 1);
    glm::vec4 yAxisColorHover = glm::vec4(0);
    
    AGameObject *xAxisObject;
    AGameObject *yAxisObject;
    ASpriteComponent *xAxisSprite;
    ASpriteComponent *yAxisSprite;
    AGameObject *activeGameObject;
    APropertiesWindow *propertiesWindow;
public:
    ATranslateGizmo(ASprite *sprite, APropertiesWindow *propertiesWindow);
    void start() override;
    void update(float dt) override;
    void setActive();
    void setInactive();
};

#endif
