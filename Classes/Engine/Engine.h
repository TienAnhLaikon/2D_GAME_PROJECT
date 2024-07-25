#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "cocos2d.h"
USING_NS_CC;

class Engine
{
public:
    static Engine *getInstance();
    static cocos2d::Animation* createAnimation1(std::string prefixName, int pFrameOrder, float delay);
    static cocos2d::Animation* createAnimation2(std::string prefixName, int pFrameOrder, float delay);
    int randomNumber(int min, int max) { return cocos2d::RandomHelper::random_int(min, max); };
private:
    Engine();
    virtual ~Engine();
    static Engine* instance;
};

#endif // __ENGINE_H__