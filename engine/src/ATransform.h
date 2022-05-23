#ifndef ATRANSFORM_H
#define ATRANSFORM_H

#include <glm/glm.hpp>

class ATransform
{
public:
    ATransform();
    ATransform(glm::vec2 position);
    ATransform(glm::vec2 position, glm::vec2 scale);
    void init(glm::vec2 position, glm::vec2 scale);
    bool operator==(ATransform &a);
    bool operator!=(ATransform &a);
    glm::vec2 position;
    glm::vec2 scale;
};

#endif
