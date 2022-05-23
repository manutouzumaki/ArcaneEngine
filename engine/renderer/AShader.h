#ifndef ASHADER_H
#define ASHADER_H

#include <glm/glm.hpp>

class AShader
{
public:
    AShader(const char *vertexPath, const char *fragmentPath);
    void use();
    void detach();
    void updateVec3f(const char *varName, glm::vec3 vector);
    void updateMat4f(const char *varName, glm::mat4 matrix);
    void updateTexture(const char *varName, unsigned int id);
    void updateIntArray(const char *varName, int size, int *array);
private:
    int shaderProgram;
    bool isUsed;
};

#endif
