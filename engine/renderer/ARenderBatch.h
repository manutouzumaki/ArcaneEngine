#ifndef ARENDERBATCH_H
#define ARENDERBATCH_H

#include "../components/ASpriteComponent.h"
#include "../util/AArray.h"
#include "AShader.h"

#define MAX_BATCH_SIZE 1000
#define POS_SIZE 2
#define COLOR_SIZE 4
#define TEX_COORDS_SIZE 2
#define TEX_ID_SIZE 1
#define POS_OFFSET 0
#define COLOR_OFFSET ((POS_OFFSET + POS_SIZE) * sizeof(float))
#define TEX_COORDS_OFFSET (COLOR_OFFSET + COLOR_SIZE * sizeof(float))
#define TEX_ID_OFFSET (TEX_COORDS_OFFSET + TEX_COORDS_SIZE * sizeof(float))
#define VERTEX_SIZE 9
#define VERTEX_SIZE_BYTES (VERTEX_SIZE * sizeof(float))

class ARenderBatch
{
public:
    ARenderBatch(int zIndex);
    ~ARenderBatch();
    void start();
    void render();
    void addSprite(ASpriteComponent *sprite);
    bool hasRoom;
    bool hasTextureRoom();
    bool hasTexture(ATexture *texture);
    int getZIndex();
private:    
    void loadVertexProperties(int index);
    ASpriteComponent **sprites;
    int numSprites;
    float *vertices;
    int texSlots[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    unsigned int VAO;
    unsigned int VBO;
    AShader *shader;
    AArray<ATexture *> textures;
    int zIndex;
};

#endif
