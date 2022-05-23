#include "ATransform.h"

ATransform::ATransform()
{
    init(glm::vec2(0.0f), glm::vec2(1.0f));
}

ATransform::ATransform(glm::vec2 position)
{
    init(position, glm::vec2(1.0f));
}

ATransform::ATransform(glm::vec2 position, glm::vec2 scale)
{
    init(position, scale);
}

void ATransform::init(glm::vec2 position, glm::vec2 scale)
{
    this->position = position;
    this->scale = scale;
}

bool ATransform::operator==(ATransform &a)
{
    bool positionCheck = (a.position.x == position.x &&
                          a.position.y == position.y);
    bool scaleCheck = (a.scale.x == scale.x &&
                       a.scale.y == scale.y);
    return (positionCheck && scaleCheck);
}

bool ATransform::operator!=(ATransform &a)
{
    bool positionCheck = (a.position.x == position.x &&
                          a.position.y == position.y);
    bool scaleCheck = (a.scale.x == scale.x &&
                       a.scale.y == scale.y);
    return (!positionCheck || !scaleCheck);
}
