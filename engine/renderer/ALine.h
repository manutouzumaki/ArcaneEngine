#ifndef ALINE_H
#define ALINE_H

#include <glm/glm.hpp>

class ALine
{
public:
    ALine(glm::vec2 from, glm::vec2 to, glm::vec3 color, int lifeTime);
    glm::vec2 getFrom();
    glm::vec2 getTo();
    glm::vec3 getColor();
    int beginFrame();
private:
    glm::vec2 from;
    glm::vec2 to;
    glm::vec3 color;
    int lifeTime;
};

#endif
