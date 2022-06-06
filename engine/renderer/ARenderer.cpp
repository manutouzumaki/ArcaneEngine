#include "ARenderer.h"

AShader *ARenderer::currentShader = nullptr;

ARenderer::~ARenderer()
{
    for(int i = 0; i < batchs.size(); ++i)
    {
        delete batchs[i];
    }
}

void ARenderer::add(AGameObject *gameObject)
{
    if(gameObject->hasComponent("ASpriteComponent"))
    {
        ASpriteComponent *sprite = (ASpriteComponent *)gameObject->getComponent("ASpriteComponent");
        add(sprite);
    }
}


static void SortBatchByZIndex(AArray<ARenderBatch *> *batchs)
{
    for(int j = 1; j < batchs->size(); ++j)
    {
        ARenderBatch *key = (*batchs)[j];
        int i = j - 1;
        while(i > -1 && (*batchs)[i]->getZIndex() > key->getZIndex())
        {
            (*batchs)[i + 1] = (*batchs)[i];
            --i;
        }
        (*batchs)[i + 1] = key;
    }
}

void ARenderer::add(ASpriteComponent *sprite)
{
    bool added = false;
    for(int i = 0; i < batchs.size(); ++i)
    {
        if(batchs[i]->hasRoom && batchs[i]->getZIndex() == sprite->gameObject->transform->zIndex)
        {
            ATexture *texture = sprite->getTexture();
            if(texture == nullptr || (batchs[i]->hasTexture(texture) || batchs[i]->hasTextureRoom()))
            {
                batchs[i]->addSprite(sprite);
                added = true;
                break;
            }
        } 
    }
    if(!added)
    {
        ARenderBatch *batch = new ARenderBatch(sprite->gameObject->transform->zIndex);
        batch->start();
        batch->addSprite(sprite);
        batchs.add(batch); 
        SortBatchByZIndex(&batchs); 
    }
}

void ARenderer::render()
{
    currentShader->use();
    for(int i = 0; i < batchs.size(); ++i)
    {
        batchs[i]->render();
    }
}

void ARenderer::destroyGameObject(AGameObject *gameObject)
{
    if(gameObject->hasComponent("ASpriteComponent"))
    {
        ASpriteComponent *sprite = (ASpriteComponent *)gameObject->getComponent("ASpriteComponent");
        for(int i = 0; i < batchs.size(); ++i)
        {
            if(batchs[i]->destroyIfExist(sprite))
            {
                return;
            }
        }

    }
}


void ARenderer::bindShader(AShader *shader)
{
    currentShader = shader;
}

AShader *ARenderer::getBindShader()
{
    return currentShader;
}
