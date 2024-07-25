#ifndef __AUDIO_ENGINE_H__
#define __AUDIO_ENGINE_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

class Audio
{
public:
    static Audio *getInstance();
    int play2d(const std::string filePath, bool loop = false, float volume = 1.0f, const AudioProfile *profile = nullptr);
    void setVolume(int audioId, float volume);
    void resume(int audioId);
    void stopAll();
    void stop(int audioId);

    void pause(int audioId);
    void preload(std::string filePath);
    AudioEngine::AudioState getState(int audioId);
    bool isPlaying(int audioId); // Thm khai bo hm mi

private:
    Audio();
    virtual ~Audio();
    static Audio* instance;
};

#endif // __AUDIO_ENGINE_H__