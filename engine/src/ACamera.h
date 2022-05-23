#ifndef ACAMERA_H
#define ACAMERA_H

#include <glm/glm.hpp>

class ACamera
{
public:
    ACamera(glm::vec3 position);
    glm::mat4 getProjMatrix();
    glm::mat4 getViewMatrix();
    glm::vec3 position;
private:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

};

#endif
