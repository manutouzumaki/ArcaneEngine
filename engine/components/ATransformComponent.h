#ifndef ATRANSFORMCOMPONENT_H
#define ATRANSFORMCOMPONENT_H

#include "../src/AComponent.h"
#include <glm/glm.hpp>

class ATransformComponent : public AComponent
{
public:
    ATransformComponent();
    ~ATransformComponent();
    ATransformComponent(glm::vec2 position);
    ATransformComponent(glm::vec2 position, glm::vec2 scale, float rotation, int zIndex);
    void init(glm::vec2 position, glm::vec2 scale);
    bool operator==(ATransformComponent *a);
    bool operator!=(ATransformComponent *a);
    void imgui() override;
    void serialize(TiXmlElement *parent) override;
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
    int zIndex;
};

#endif
