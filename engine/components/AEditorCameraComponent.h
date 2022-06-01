#ifndef AEDITORCAMERACOMPONENT_H
#define AEDITORCAMERACOMPONENT_H

#include "../src/AComponent.h"
#include <glm/glm.hpp>

class ACamera;

class AEditorCameraComponent : public AComponent
{
private:
    bool clickOriginSet;
    bool reset;
    glm::vec3 clickOrigin;
    ACamera *camera;
    float lerpTime; 
public:
    AEditorCameraComponent(ACamera *camera);
    void update(float dt) override;
};

#endif
