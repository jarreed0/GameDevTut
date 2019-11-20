#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>

class Audio {
public:
    Audio();
    ~Audio();
    void load(const char* filename);
    void play();
private:
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_AudioDeviceID deviceId;
};

#endif //AUDIO_H