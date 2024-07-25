#include "AudioEngine.h"

Audio *Audio::instance = nullptr;

Audio *Audio::getInstance()
{
    if (!instance)
    {
        instance = new Audio();
    }
    return instance;
}

Audio::Audio()
{
}

Audio::~Audio()
{
}

int Audio::play2d(
    const std::string filePath,
    bool loop,
    float volume,
    const AudioProfile *profile)
{
    return cocos2d::AudioEngine::play2d(filePath, loop, volume, profile);
}
void Audio::setVolume(
    int audioId,
    float volume)
{
    cocos2d::AudioEngine::setVolume(audioId, volume);
}

void Audio::resume(int audioId)
{
    cocos2d::AudioEngine::resume(audioId);
}
void Audio::stopAll()
{
    cocos2d::AudioEngine::stopAll();
}
void Audio::stop(int audioId)
{
    cocos2d::AudioEngine::stop(audioId);
}
void Audio::pause(int audioId)
{
    cocos2d::AudioEngine::pause(audioId);
}
void Audio::preload(std::string filePath)
{
    cocos2d::AudioEngine::preload(filePath);
}
AudioEngine::AudioState Audio::getState(int audioId)
{
    return cocos2d::AudioEngine::getState(audioId);
}

bool Audio::isPlaying(int audioId)
{
    return cocos2d::AudioEngine::getState(audioId) == cocos2d::AudioEngine::AudioState::PLAYING;
}
