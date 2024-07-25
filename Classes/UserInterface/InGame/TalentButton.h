#ifndef __TALENT_BUTTON__
#define __TALENT_BUTTON__

#include "cocos2d.h"
USING_NS_CC;
class TalentCarousel;
class TalentButton : public cocos2d::Node
{
public:
    static TalentButton* create(TalentCarousel* talentCarousel);
    virtual bool init(TalentCarousel* talentCarousel);
    bool onTouchBegan(Touch* touch, Event* event);
    Size getSpriteSize() const { return btnSprite->getContentSize(); };
    Sprite* btnSprite;
private: 


    TalentCarousel* talent;
};

#endif // __JOYSTICK_H__