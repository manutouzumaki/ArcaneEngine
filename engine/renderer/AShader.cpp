#include "AShader.h"

#include <glad/glad.h>

#include <malloc.h>
#include <cstdio>

// TODO: create AFile class to handle files
void *ReadFile(const char *fileName)
{
    void *data = 0;
    FILE *file = 0;
    fopen_s(&file, fileName, "rb");
    if(file)
    {
        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        data = malloc(fileSize + 1);
        fread_s(data, fileSize, fileSize, 1, file);
        char *lastByte = (char *)data + fileSize;
        *lastByte = 0;
    }
    fclose(file);
    return data;
}

AShader::AShader(const char *vertexPath, const char *fragmentPath)
{
    const char *vertexShaderSrc = (const char *)ReadFile(vertexPath);
    const char *fragmentShaderSrc = (const char *)ReadFile(fragmentPath);

    int succes;
    char infoLog[512];
    
    int vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vertexShaderSrc, 0);
    glCompileShader(vertexID);
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &succes);
    if(!succes)
    {
        printf("Error Compiling Vertex Shader\n");
        glGetShaderInfoLog(vertexID, 512, 0, infoLog);
        printf(infoLog);
        printf("\n");
    }

    int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentID, 1, &fragmentShaderSrc, 0);
    glCompileShader(fragmentID);
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &succes);
    if(!succes)
    {
        printf("Error Compiling Fragment Shader\n");
        glGetShaderInfoLog(fragmentID, 512, 0, infoLog);
        printf(infoLog);
        printf("\n");
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexID);
    glAttachShader(shaderProgram, fragmentID);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &succes);
    if(!succes)
    {
        printf("Error linking Shader to Program\n");
        glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        printf(infoLog);
        printf("\n");
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    free((void *)vertexShaderSrc);
    free((void *)fragmentShaderSrc);

    isUsed = false;
}

void AShader::use()
{
    if(!isUsed)
    {
        glUseProgram(shaderProgram);
        isUsed = true;
    }
}

void AShader::detach()
{
    glUseProgram(0);
    isUsed = false;
}

void AShader::updateVec3f(const char *varName, glm::vec3 vector)
{
    int varLoc = glGetUniformLocation(shaderProgram, varName);
    use();
    glUniform3fv(varLoc, 1, &vector[0]); 
}

void AShader::updateMat4f(const char *varName, glm::mat4 matrix)
{
    int varLoc = glGetUniformLocation(shaderProgram, varName);
    use();
    glUniformMatrix4fv(varLoc, 1, false, &matrix[0][0]);
}

void AShader::updateTexture(const char *varName, unsigned int id)
{ 
    int varLoc = glGetUniformLocation(shaderProgram, varName);
    use();
    glUniform1i(varLoc, id);
}

void AShader::updateIntArray(const char *varName, int size, int *array)
{
    int varLoc = glGetUniformLocation(shaderProgram, varName);
    use();
    glUniform1iv(varLoc, size, array);
}
