#ifndef AMOUSEPICKING_H
#define AMOUSEPICKING_H

class AMousePicking
{
private:
    unsigned int pickingTextureID;
    unsigned int FBO;
    unsigned int depthTexture;
public:
    AMousePicking(int width, int height);
    bool init(int width, int height);
    void enableWritting();
    void disableWritting();
    int readPixel(int x, int y);
};

#endif
