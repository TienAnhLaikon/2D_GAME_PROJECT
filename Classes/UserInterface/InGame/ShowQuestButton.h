#ifndef __SHOW_QUEST_BUTTON__
#define __SHOW_QUEST_BUTTON__

#include "cocos2d.h"
USING_NS_CC;
class QuestBtn : public cocos2d::Node
{
public:
    static QuestBtn* create();
    virtual bool init();
    bool onTouchBegan(Touch* touch, Event* event);
    Size getSpriteSize() const { return btnSprite->getContentSize(); };
    Sprite* btnSprite;
private: 

};

#endif // __JOYSTICK_H__