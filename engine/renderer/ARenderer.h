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
    static void bindShader(AShader *shader);
    static AShader *getBindShader();
private:
    AArray<ARenderBatch *> batchs;
    static AShader *currentShader;
};

#endif
