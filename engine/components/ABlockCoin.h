#ifndef ABLOCKCOIN_H
#define ABLOCKCOIN_H

#include "../src/AComponent.h"
#include <glm/glm.hpp>

class ABlockCoin : public AComponent
{
private:
    glm::vec2 topY;
    float cointSpeed;
public:
    ABlockCoin();
    void start() override;
    void update(float dt) override; 
    void serialize(TiXmlElement *parent) override;
};

#endif
