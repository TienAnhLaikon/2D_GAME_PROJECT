#ifndef __MAIN_MENU_SCENE__
#define __MAIN_MENU_SCENE__

#include "cocos2d.h"

USING_NS_CC;
class SettingsManager;
class MainMenuScene : public cocos2d::Scene
{
public:
    int mainMenuMusicId;
    MenuItemImage* playButton;
    MenuItemImage* tutorialButton;
    MenuItemImage* settingsButton;
    MenuItemImage* exitButton;
    Label* playLabel;
    Label* tutorialLabel;
    Label* settingsLabel;
    Label* exitLabel;
    int clickButtonSoundId;
    static Scene* createScene();
    virtual bool init();
    void menuPlayCallback(Ref* pSender);
    void menuCloseCallback(Ref* pSender);
    void tutorialCallback(Ref* pSender);
    void settingsCallback(Ref* pSender);
    SettingsManager* settingsManager;
    CREATE_FUNC(MainMenuScene);
};

#endif // __SPLASH_H__