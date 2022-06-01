#ifndef ACAMERA_H
#define ACAMERA_H

#include <glm/glm.hpp>

class ACamera
{
public:
    ACamera(glm::vec3 position);
    glm::mat4 getProjMatrix();
    glm::mat4 getViewMatrix();
    glm::mat4 getInvProjMatrix();
    glm::mat4 getInvViewMatrix();
    glm::vec3 position;
    glm::vec2 projectionSize;
    float getZoom();
    void setZoom(float zoom);
    void addZoom(float value);
private:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 invProjectionMatrix;
    glm::mat4 invViewMatrix;
    float zoom;
};

#endif
