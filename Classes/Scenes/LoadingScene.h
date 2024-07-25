#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class Game;
class LoadingScene : public cocos2d::Layer
{
public:
    static LoadingScene* createLoading();
    virtual bool init();
    void updateLoading(float dt);
    void unscheduleUpdate();
    float currentPercent = 0;
    bool onTouchBegan(Touch* touch, Event* event) { return true; }
    ui::LoadingBar* loadingBarFill;
    float perc = 0.0f;
    bool isDeleted = false;
    CREATE_FUNC(LoadingScene);
};

#endif // __SPLASH_H__