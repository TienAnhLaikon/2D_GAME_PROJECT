#ifndef __TUTORIAL_SCENE__
#define __TUTORIAL_SCENE__

#include "cocos2d.h"

USING_NS_CC;
class Game;
class SettingsManager;
class TutorialScene : public cocos2d::Scene
{
public:
    static Scene *createScene();
    virtual bool init();
    void backToMainMenuCallback(Ref* pSender);
    int clickButtonSoundId;
    CREATE_FUNC(TutorialScene);
};

#endif // __SPLASH_H__