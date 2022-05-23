#ifndef ARENDERER_H
#define ARENDERER_H

#include "../src/AGameObject.h"
#include "../util/AArray.h"
#include "ARenderBatch.h"

class ARenderer
{
public:
    ~ARenderer();
    void add(AGameObject *gameObject);
    void add(ASpriteComponent *sprite);
    void render();
private:
    AArray<ARenderBatch *> batchs;
};

#endif
