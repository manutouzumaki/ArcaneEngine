#include "ATexture.h"

#include <glad/glad.h>
#include "stb_image.h"

ATexture::ATexture(const char *filePath)
{
    this->filePath = filePath;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath, &width, &height, &nrComponents, 0);
    if(data)
    {
        GLenum format;
        if(nrComponents == 1)
            format = GL_RED;
        else if(nrComponents == 3)
            format = GL_RGB;
        else if(nrComponents == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        printf("Error Loading Texture\n");
    }
    stbi_image_free(data);

}

void ATexture::bind()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void ATexture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int ATexture::getID()
{
    return textureID;
}

int ATexture::getWidth()
{
    return width;
}

int ATexture::getHeight()
{
    return height;
}
