#include "ARenderBatch.h"
#include <glad/glad.h>

#include "../src/AWindow.h"
#include "../util/ADefines.h"
#include "../util/AAssetPool.h"

#include <glm/glm.hpp>
#include <malloc.h>

static void loadElementIndices(unsigned *indices, int index)
{
    int offsetArrayIndex = 6 * index;
    int offset = 4 * index; 
    indices[offsetArrayIndex] = offset + 3;
    indices[offsetArrayIndex + 1] = offset + 2;
    indices[offsetArrayIndex + 2] = offset;
    indices[offsetArrayIndex + 3] = offset;
    indices[offsetArrayIndex + 4] = offset + 2;
    indices[offsetArrayIndex + 5] = offset + 1;
}

static void generateIndices(unsigned int *indices)
{
    for(int i = 0; i < MAX_BATCH_SIZE; ++i)
    {
        loadElementIndices(indices, i);        
    }
}

ARenderBatch::ARenderBatch(int zIndex)
{
    shader = AAssetPool::getShader("default");
    numSprites = 0;
    hasRoom = true;
    sprites = (ASpriteComponent **)malloc(sizeof(ASpriteComponent *) * MAX_BATCH_SIZE);
    vertices = (float *)malloc(MAX_BATCH_SIZE * (4 * VERTEX_SIZE) * sizeof(float));
    this->zIndex = zIndex;
}

ARenderBatch::~ARenderBatch()
{
    printf("deleteing Render Batch\n");
    free(vertices);
    free(sprites);
}

void ARenderBatch::start()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (MAX_BATCH_SIZE * (4 * VERTEX_SIZE) * sizeof(float)), 0, GL_DYNAMIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    unsigned int *indices = (unsigned int *)malloc((6 * MAX_BATCH_SIZE) * sizeof(unsigned int));
    generateIndices(indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (6 * MAX_BATCH_SIZE) * sizeof(unsigned int), indices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, POS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void *)POS_OFFSET);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, COLOR_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void *)COLOR_OFFSET);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, TEX_COORDS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void *)TEX_COORDS_OFFSET);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, TEX_ID_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void *)TEX_ID_OFFSET);
    glEnableVertexAttribArray(3);

    free(indices);
}

void ARenderBatch::render()
{
    bool rebufferData = false;
    for(int i = 0; i < numSprites; ++i)
    {
        ASpriteComponent *sprite = sprites[i];
        if(sprite->isDirty)
        {
            loadVertexProperties(i);
            sprite->isDirty = false;
            rebufferData = true;
        }
    }
    if(rebufferData)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_BATCH_SIZE * (4 * VERTEX_SIZE_BYTES), vertices);
    }
    shader->use();
    shader->updateMat4f("uProj", AWindow::getScene()->getCamera()->getProjMatrix());
    shader->updateMat4f("uView", AWindow::getScene()->getCamera()->getViewMatrix());
    
    for(int i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        textures[i]->bind();
    }
    
    shader->updateIntArray("uTextures", 8, texSlots);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, numSprites * 6, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    
    for(int i = 0; i < textures.size(); ++i)
    {
        textures[i]->unbind();
    }
    shader->detach();
}

void ARenderBatch::addSprite(ASpriteComponent *sprite)
{
    int index = numSprites;
    sprites[index] = sprite;
    numSprites++;
    
    ATexture *texture = sprite->getTexture(); 
    if(texture)
    {
        if(!hasTexture(texture))
        {
            textures.add(texture);
        }
    }

    loadVertexProperties(index);

    if(numSprites >= MAX_BATCH_SIZE)
    {
        hasRoom = false;
    }
}

void ARenderBatch::loadVertexProperties(int index)
{
    ASpriteComponent *sprite = sprites[index];

    int offset = index * 4 * VERTEX_SIZE;
    glm::vec4 color = sprite->getColor();
    ATexture *texture = sprite->getTexture(); 
    int texID = 0;
    if(texture)
    {
        for(int i = 0; i < textures.size(); ++i)
        {
            if(textures[i]->getID() == texture->getID())
            {
                texID = i + 1;
                break;
            }
        }
    }
    glm::vec2 *texCoords = sprite->getTexCoords();

    float x = 1.0f;
    float y = 1.0f; 
    for(int i = 0; i < 4; ++i)
    {
        if(i == 1)
            y = 0.0f;   
        else if(i == 2)
            x = 0.0f;
        else if(i == 3)
            y = 1.0f;

        vertices[offset] = sprite->gameObject->transform.position.x + (x * sprite->gameObject->transform.scale.x);
        vertices[offset + 1] = sprite->gameObject->transform.position.y + (y * sprite->gameObject->transform.scale.y);
        vertices[offset + 2] = color.r;
        vertices[offset + 3] = color.g;
        vertices[offset + 4] = color.b;
        vertices[offset + 5] = color.a;
        // TODO: load texture Coords
        vertices[offset + 6] = texCoords[i].x;
        vertices[offset + 7] = texCoords[i].y;

        // TODO: load texture ID
        vertices[offset + 8] = (float)texID;

        offset += VERTEX_SIZE;
    }
}

bool ARenderBatch::hasTextureRoom()
{
    return textures.size() < 8;
}

bool ARenderBatch::hasTexture(ATexture *texture)
{
    bool found = false;
    for(int i = 0; i < textures.size(); ++i)
    {
        if(texture->getID() == textures[i]->getID())
        {
            found = true;
            break;
        }
    }
    return found;
}

int ARenderBatch::getZIndex()
{
    return zIndex;
}
