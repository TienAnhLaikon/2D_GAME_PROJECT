#include "../Engine/Engine.h"

Engine* Engine::instance = nullptr;

Engine* Engine::getInstance()
{
    if (!instance)
    {
        instance = new Engine();
    }
    return instance;
}

Engine::Engine()
{
}

Engine::~Engine()
{
}

cocos2d::Animation* Engine::createAnimation1(std::string prefixName, int pFrameOrder, float delay)
{
    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < pFrameOrder; i++)
    {
        std::string str = prefixName + StringUtils::toString(i) + ".png";
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame == nullptr)
            continue;
        animFrames.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(animFrames, delay);
    return animation;
}

cocos2d::Animation* Engine::createAnimation2(std::string prefixName, int pFrameOrder, float delay)
{
    Vector<SpriteFrame*> animFrames;
    for (int i = 1; i <= pFrameOrder; i++) {
        char buffer[20] = { 0 };
        sprintf(buffer, " (%d).png", i);
        std::string str = prefixName + buffer;
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(animFrames, delay);


    return animation;
}