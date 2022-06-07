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
    glm::vec4 xAxisColor = glm::vec4(1, 0.3f, 0.3f, 1);
    glm::vec4 xAxisColorHover = glm::vec4(1, 0 , 0, 1);
    glm::vec4 yAxisColor = glm::vec4(0.3f, 1, 0.3f, 1);
    glm::vec4 yAxisColorHover = glm::vec4(0, 1, 0, 1);
    
    AGameObject *xAxisObject;
    AGameObject *yAxisObject;
    ASpriteComponent *xAxisSprite;
    ASpriteComponent *yAxisSprite;
    AGameObject *activeGameObject;
    APropertiesWindow *propertiesWindow;

    float gizmoWidth;
    float gizmoHeight;
    bool checkXHoverState();
    bool checkYHoverState();

    bool xAxisActive = false;
    bool yAxisActive = false;
    
    bool inUse = true;
public:
    ATranslateGizmo(ASprite *sprite, APropertiesWindow *propertiesWindow);
    void start() override;
    void editorUpdate(float dt) override;
    void update(float dt) override;
    void setActive();
    void setInactive();
    void setInUse();
    void setNotUse();
};

#endif
