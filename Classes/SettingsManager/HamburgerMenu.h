#ifndef __HAMBURGER_MENU_H__
#define __HAMBURGER_MENU_H__
#include "cocos2d.h"
USING_NS_CC;
class HamburgerMenu : public cocos2d::Node
{
public:
    // Sprite *SettingsList;
    Sprite *hamburgerMenuList;
    bool init();
    bool isShow();
    void show();
    void hide();
    void unpauseGameCallBack(Ref* pSender);
    CREATE_FUNC(HamburgerMenu);
private:
    HamburgerMenu(){};

    ~HamburgerMenu();
};
#endif // __HAMBURGER_MENU_H__
