#ifndef __SETTINGS_MANAGER_H__
#define __SETTINGS_MANAGER_H__

#include "cocos2d.h"
#include "ui/UISlider.h"
#include "Scenes/PauseMenuScene.h"
USING_NS_CC;
class SettingsManagerInPauseMenu : public Node
{
public:

    int clickButtonSoundId;
    //Sprite *SettingsList;
    Sprite* menu;
    Sprite* close_menu;
    Sprite* stoperLeft;
        Sprite* stoperRight;
    Label* settingsTitle;
    Label* musicTitle;
    Label* soundTitle;
    int languageIndex = 0;
    ui::Slider *musicSlider;
    ui::Slider* soundSlider;
    Label* languageLabel;
    bool init();
    bool isMenuShow();
    void showMenuList();
    void hideMenuList();
    void updateLanguageLabel(Label* label, int index);

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);
    CREATE_FUNC(SettingsManagerInPauseMenu);
private:
    SettingsManagerInPauseMenu(){};
    ~SettingsManagerInPauseMenu();
};
#endif // __SETTINGS_MANAGER_H__
