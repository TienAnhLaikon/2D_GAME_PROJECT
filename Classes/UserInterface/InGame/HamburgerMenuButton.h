#ifndef __HAMBURGER_MENU_BUTTON__
#define __HAMBURGER_MENU_BUTTON__
#include "cocos2d.h"
USING_NS_CC;
class HamburgerMenu;
class Game;
class HamburgerMenuButton : public cocos2d::Node
{
public:
    static HamburgerMenuButton *create(HamburgerMenu* hamburgerMenu);
    virtual bool init(HamburgerMenu* hamburgerMenu);
    bool onTouchBegan(Touch *touch, Event *event);
    Size getSpriteSize() const { return btnSprite->getContentSize(); };
    Sprite* btnSprite;

private:
    Game* game;
    HamburgerMenu* hamburgerMenu;

};

#endif // __HAMBURGER_MENU_BUTTON__