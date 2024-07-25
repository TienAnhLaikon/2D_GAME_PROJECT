#ifndef __LEVEL_UP_POPUP__
#define __LEVEL_UP_POPUP__

#include "cocos2d.h"
USING_NS_CC;
class LevelUpPopup : public cocos2d::Node
{
public:
    static LevelUpPopup* create(int level);
    virtual bool init(int level);
    void show(int level);
    Sprite* levelUpSprite = nullptr;
    Label* levelUpLabel = nullptr;
    Label* levelUpMsg = nullptr;
    //LevelUpPopup();

private:
    int _level = 0;
    
};

#endif // __JOYSTICK_H__