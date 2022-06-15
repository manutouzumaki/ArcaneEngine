#ifndef ASOUND_H
#define ASOUND_H

class ASound
{
private:
    unsigned int bufferID;
    unsigned int sourceID;
    const char *filepath;
    bool isPlaying;
    bool loop;
public:
    ASound(const char *filepath, bool loop, bool isPlaying);
    ~ASound();

    void play();
    void stop();
    const char *getFilepath();
    bool getIsPlaying();
};

#endif
