#include "ASound.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "../util/stb_vorbis.h"

#include <stdio.h>
#include <malloc.h>

ASound::ASound(const char *filepath, bool loop, bool isPlaying)
{
    this->filepath = filepath;
    this->isPlaying = isPlaying;
    this->loop = loop;

    int channels = 0;
    int sampleRate = 0;
    short *data = nullptr; 
    int sampleCount = stb_vorbis_decode_filename(filepath, &channels, &sampleRate, &data);
    ALsizei size = (ALsizei)(sampleCount * sizeof(short) * 2);
    if(data && sampleCount >= 0)
    {
        // find the correct openAl format
        int format = -1;
        if(channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else if(channels == 2)
        {
            format = AL_FORMAT_STEREO16;
        }   

        alGenBuffers(1, &bufferID);
        alBufferData(bufferID, format, data, size, sampleRate);

        alGenSources(1, &sourceID);
        alSourcei(sourceID, AL_BUFFER, bufferID);
        alSourcei(sourceID, AL_LOOPING, loop);
        alSourcei(sourceID, AL_POSITION, 0);
        alSourcef(sourceID, AL_GAIN, 0.3f);

        free(data);    
    }
    else
    {
        printf("Error: Failed loading sound: '%s'\n", filepath);
    }
        
}

ASound::~ASound()
{
    alDeleteBuffers(1, &bufferID);
    alDeleteSources(1, &sourceID);
}

void ASound::play()
{
    int state;
    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    if(state == AL_STOPPED)
    {
        isPlaying = false;
        alSourcei(sourceID, AL_POSITION, 0);
    }

    if(!isPlaying)
    {
        alSourcePlay(sourceID);
        isPlaying = true;
    
    }
}

void ASound::stop()
{
    if(isPlaying)
    {
        alSourceStop(sourceID);
        isPlaying = false;
    }
}

const char *ASound::getFilepath()
{
    return filepath;
}

bool ASound::getIsPlaying()
{
    int state;
    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    if(state == AL_STOPPED)
    {
        isPlaying = false;
    }
    return isPlaying;
}
