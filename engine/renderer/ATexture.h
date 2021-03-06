#ifndef ATEXTURE_H
#define ATEXTURE_H

class ATexture
{
public:
    ATexture(const char *filePath);
    ATexture(int width, int height);
    void bind();
    void unbind();
    unsigned int getID();
    int getWidth();
    int getHeight();
    const char *getPath();
private:
    const char *filePath;
    unsigned int textureID;
    int width;
    int height;
};

#endif
