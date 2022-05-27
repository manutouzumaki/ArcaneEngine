#include "ACamera.h"
#include "../util/ADefines.h"

#include <glm/gtc/matrix_transform.hpp>

ACamera::ACamera(glm::vec3 position)
{
    this->position = position;
    this->projectionSize = glm::vec2(32 * 40, 32 * 21);
}

glm::mat4 ACamera::getProjMatrix()
{
    projectionMatrix = glm::ortho(0.0f, projectionSize.x, 0.0f, projectionSize.y, 0.0f, 100.0f);
    invProjectionMatrix = glm::inverse(projectionMatrix);
    return projectionMatrix;
}

glm::mat4 ACamera::getViewMatrix()
{
    glm::vec3 target = glm::vec3(position.x, position.y, -1);
    glm::vec3 up = glm::vec3(0, 1, 0);
    viewMatrix = glm::lookAt(position, target, up);
    invViewMatrix = glm::inverse(viewMatrix);
    return viewMatrix; 
}

glm::mat4 ACamera::getInvProjMatrix()
{
    return invProjectionMatrix;
}

glm::mat4 ACamera::getInvViewMatrix()
{
    return invViewMatrix;
}
