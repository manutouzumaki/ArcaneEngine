#ifndef ATEXTURE_H
#define ATEXTURE_H

class ATexture
{
public:
    ATexture(const char *filePath);
    void bind();
    void unbind();
    unsigned int getID();
    int getWidth();
    int getHeight();
private:
    const char *filePath;
    unsigned int textureID;
    int width;
    int height;
};

#endif
