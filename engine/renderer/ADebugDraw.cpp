#include "ADebugDraw.h"
#include <glad/glad.h>
#include "../src/AWindow.h"
#include "../util/AAssetPool.h"
#include "../util/ADefines.h"
#include <malloc.h>

#include <glm/gtx/rotate_vector.hpp>

// TODO: fix a Bug in this class...

int ADebugDraw::linesCount = 0;
ALine **ADebugDraw::lines = nullptr;
float *ADebugDraw::vertexArray = nullptr;
AShader *ADebugDraw::shader = nullptr;
unsigned int ADebugDraw::VAO;
unsigned int ADebugDraw::VBO;
bool ADebugDraw::started = false;
bool ADebugDraw::initialized = false;

void ADebugDraw::init()
{
    shader = AAssetPool::getShader("lines");
    lines = (ALine **)malloc(MAX_LINES * sizeof(ALine *));
    vertexArray = (float *)malloc(MAX_LINES * 6 * 2 * sizeof(float));
    for(int i = 0; i < MAX_LINES; ++i)
    {
        lines[i] = nullptr;
    } 
    initialized = true;
}

void ADebugDraw::shutDown()
{
    int StopHere = 0;
    for(int i = 0; i < linesCount; ++i)
    {
        if(lines[i]) delete lines[i];
    }
    if(vertexArray) free(vertexArray);
    if(lines) free(lines);
}

void ADebugDraw::start()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_LINES * 6 * 2 * sizeof(float), 0, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glLineWidth(2.0f);
}

void removeLine(ALine **linesArray, int index, int *size)
{
    for(int i = index; i < *size; ++i)
    {
        if(i == index)
        {
            delete linesArray[i];
            *size = *size - 1;
        } 
        if(i < *size)
        {
            linesArray[i] = linesArray[i + 1];
        }
    }
    linesArray[*size] = nullptr;
}

void ADebugDraw::beginFrame()
{
    if(!started)
    {
        start();
        started = true;
    }
    for(int i = 0; i < linesCount; ++i)
    {
        if(lines[i] && lines[i]->beginFrame() < 0)
        {
            removeLine(lines, i, &linesCount);
            i--;
        } 
    }
}

void ADebugDraw::draw()
{
    if(linesCount <= 0)
    {
        return;
    }

    int index = 0;
    for(int i = 0; i < linesCount; ++i)
    {
        for(int j = 0; j < 2; ++j)
        {
            glm::vec2 position = (j == 0) ? lines[i]->getFrom() : lines[i]->getTo();
            glm::vec3 color = lines[i]->getColor();
            vertexArray[index] = position.x;
            vertexArray[index + 1] = position.y;
            vertexArray[index + 2] = -10.0f;
            vertexArray[index + 3] = color.r;
            vertexArray[index + 4] = color.g;
            vertexArray[index + 5] = color.b;
            index += 6;
        } 
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, linesCount * 6 * 2 * sizeof(float), vertexArray);

    shader->use();
    shader->updateMat4f("uProj", AWindow::getScene()->getCamera()->getProjMatrix());
    shader->updateMat4f("uView", AWindow::getScene()->getCamera()->getViewMatrix());

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_LINES, 0, linesCount * 6 * 2);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    shader->detach();
}

void ADebugDraw::addLine(glm::vec2 from, glm::vec2 to, glm::vec3 color, int lifeTime)
{
    if(!initialized)
    {
        Assert(!"ERROR: call ADebugDraw::init(), before adding lines");
    }
    if(linesCount >= MAX_LINES)
    {
        return;
    }
    lines[linesCount++] = new ALine(from, to, color, lifeTime);
}

void ADebugDraw::addLine(glm::vec2 from, glm::vec2 to, glm::vec3 color)
{
    addLine(from, to, color, 1);
}

void ADebugDraw::addLine(glm::vec2 from, glm::vec2 to)
{
    addLine(from, to, glm::vec3(0, 1, 0), 1);
}

void ADebugDraw::addBox(glm::vec2 center, glm::vec2 dimension, float rotation, glm::vec3 color, int lifeTime)
{
    glm::vec2 radius = dimension * 0.5f;
    glm::vec2 a = glm::rotate((center + radius) - center, rotation) + center;
    glm::vec2 b = glm::rotate((center + glm::vec2(-radius.x, radius.y)) - center, rotation) + center;
    glm::vec2 c = glm::rotate((center - radius) - center, rotation) + center;
    glm::vec2 d = glm::rotate((center + glm::vec2(radius.x, -radius.y)) - center, rotation) + center;
    addLine(a, b, color, lifeTime);
    addLine(b, c, color, lifeTime);
    addLine(c, d, color, lifeTime);
    addLine(d, a, color, lifeTime);
}

void ADebugDraw::addCircle(glm::vec2 center, float radius, glm::vec3 color, int lifeTime)
{
    glm::vec2 segments[NUM_DEBUG_CIRCLE_SEGMENT];
    float angle = 0;
    float angleIncrement = 360 / NUM_DEBUG_CIRCLE_SEGMENT;
    for(int i = 0; i < NUM_DEBUG_CIRCLE_SEGMENT; ++i)
    {
        segments[i] = glm::rotate(glm::vec2(radius, 0), glm::radians(angle)) + center; 
        angle += angleIncrement;
        if(i > 0)
        {
            addLine(segments[i - 1], segments[i], color, lifeTime);
        }
    }
    addLine(segments[NUM_DEBUG_CIRCLE_SEGMENT - 1], segments[0], color, lifeTime);
}
