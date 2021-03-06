#include "ARenderBatch.h"
#include <glad/glad.h>

#include "../src/AWindow.h"
#include "../util/ADefines.h"
#include "../util/AAssetPool.h"
#include "ARenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

ARenderBatch::ARenderBatch(int zIndex, ARenderer *renderer)
{
    numSprites = 0;
    hasRoom = true;
    sprites = (ASpriteComponent **)malloc(sizeof(ASpriteComponent *) * MAX_BATCH_SIZE);
    vertices = (float *)malloc(MAX_BATCH_SIZE * (4 * VERTEX_SIZE) * sizeof(float));
    this->zIndex = zIndex;
    this->renderer = renderer;
}

ARenderBatch::~ARenderBatch()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
    glVertexAttribPointer(4, OBJ_ID_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void *)OBJ_ID_OFFSET);
    glEnableVertexAttribArray(4);

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

        if(sprite->gameObject->transform->zIndex != this->zIndex)
        {
            destroyIfExist(sprite);
            renderer->add(sprite->gameObject);
            i++;
        }
    }
    if(rebufferData)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_BATCH_SIZE * (4 * VERTEX_SIZE_BYTES), vertices);
    }
    AShader *shader = ARenderer::getBindShader();
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

bool ARenderBatch::destroyIfExist(ASpriteComponent *sprite)
{
    for(int i = 0; i < numSprites; ++i)
    {
        if(sprites[i] == sprite)
        {
            for(int j = i; j < numSprites - 1; ++j)
            {
                sprites[j] = sprites[j + 1];
                sprites[j]->isDirty = true;
            }
            numSprites--;
            return true;
        }
    }
    return false;
}

void ARenderBatch::loadVertexProperties(int index)
{
    ASpriteComponent *sprite = sprites[index];
    glm::vec2 *texCoords = sprite->getTexCoords();

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

    glm::mat4 transformMat = glm::mat4(1.0f);
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(sprite->gameObject->transform->position.x, sprite->gameObject->transform->position.y, 0.0f));
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(sprite->gameObject->transform->rotation), glm::vec3(0, 0, 1));
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(sprite->gameObject->transform->scale.x, sprite->gameObject->transform->scale.y, 0.0f));
    transformMat = translationMat * rotationMat * scaleMat;
    
    float x = 0.5f;
    float y = 0.5f; 
    for(int i = 0; i < 4; ++i)
    {
        if(i == 1)
            y = -0.5f;   
        else if(i == 2)
            x = -0.5f;
        else if(i == 3)
            y = 0.5f;

        glm::vec4 currentP = transformMat * glm::vec4(x, y, 0, 1);
        vertices[offset] = currentP.x;
        vertices[offset + 1] = currentP.y;
        vertices[offset + 2] = color.r;
        vertices[offset + 3] = color.g;
        vertices[offset + 4] = color.b;
        vertices[offset + 5] = color.a;
        // TODO: load texture Coords
        vertices[offset + 6] = texCoords[i].x;
        vertices[offset + 7] = texCoords[i].y;

        // TODO: load texture ID
        vertices[offset + 8] = (float)texID;
        vertices[offset + 9] = (float)sprite->gameObject->getUID() + 1;

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
