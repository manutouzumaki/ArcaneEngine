#ifndef ADEBUGDRAW_H
#define ADEBUGDRAW_H

#include "ALine.h"
#include "AShader.h"
#include "../util/AArray.h"

#define MAX_LINES 1000

class ADebugDraw
{
public:
    static void init();
    static void shutDown();
    static void start();
    static void beginFrame();
    static void draw();
    static void addLine(glm::vec2 from, glm::vec2 to, glm::vec3 color, int lifeTime);
    static void addLine(glm::vec2 from, glm::vec2 to, glm::vec3 color);
    static void addLine(glm::vec2 from, glm::vec2 to);
    static void addBox(glm::vec2 center, glm::vec2 dimension, float rotation, glm::vec3 color, int lifeTime);
    static void addCircle(glm::vec2 center, float radius, glm::vec3 color, int lifeTime);
private:
    static int linesCount;
    static ALine **lines;
    static float *vertexArray;
    static AShader *shader;
    static unsigned int VAO;
    static unsigned int VBO;
    static bool started;
    static bool initialized;
};

#endif
