#include "ALine.h"

#include <glad/glad.h>

ALine::ALine(glm::vec2 from, glm::vec2 to, glm::vec3 color, int lifeTime)
{
    this->from = from;
    this->to = to;
    this->color = color;
    this->lifeTime = lifeTime;
}

glm::vec2 ALine::getFrom()
{
    return from;
}

glm::vec2 ALine::getTo()
{
    return to;
}

glm::vec3 ALine::getColor()
{
    return color;
}

int ALine::beginFrame()
{
    lifeTime--;
    return lifeTime;
}
