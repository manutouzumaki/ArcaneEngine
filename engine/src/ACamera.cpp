#include "ACamera.h"
#include "../util/ADefines.h"

#include <glm/gtc/matrix_transform.hpp>

ACamera::ACamera(glm::vec3 position)
{
    this->position = position;
}

glm::mat4 ACamera::getProjMatrix()
{
    projectionMatrix = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, 0.0f, 100.0f);
    return projectionMatrix;
}

glm::mat4 ACamera::getViewMatrix()
{
    glm::vec3 target = glm::vec3(position.x, position.y, -1);
    glm::vec3 up = glm::vec3(0, 1, 0);
    viewMatrix = glm::lookAt(position, target, up);
    return viewMatrix; 
}
