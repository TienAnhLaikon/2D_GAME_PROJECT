#ifndef __PAUSE_MENU_SCENE__
#define __PAUSE_MENU_SCENE__

#include "cocos2d.h"

USING_NS_CC;
class Game;
class SettingsManagerInPauseMenu;
class PauseMenuScene : public cocos2d::Scene
{
public:
    static Scene *createScene();
    virtual bool init();
    void unpauseCallback(Ref *pSender);
    void saveGameCallback(Ref *pSender);
    void settingsCallback(Ref* pSender);
    void backToMainMenuCallback(Ref* pSender);
    MenuItemImage* resumeButton;
    MenuItemImage* saveGameButton;
    MenuItemImage* settingsButton;
    MenuItemImage* backToMainMenuButton;

    SettingsManagerInPauseMenu* settingsManager;
    int clickButtonSoundId;
    Game* game;

    CREATE_FUNC(PauseMenuScene);
};

#endif // __SPLASH_H__