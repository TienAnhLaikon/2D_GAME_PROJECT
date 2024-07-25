#ifndef __SELECT_MAP_BUTTON__
#define __SELECT_MAP_BUTTON__

#include "cocos2d.h"
USING_NS_CC;
class SelectMapLayer;
class SelectMapButton : public cocos2d::Node
{
public:
    static SelectMapButton* create(SelectMapLayer* selectMapLayer);
    virtual bool init(SelectMapLayer* selectMapLayer);
    bool onTouchBegan(Touch* touch, Event* event);
    Sprite* btnSprite;
    void showButton();
    void hideButton();
private: 
    SelectMapLayer* _selectMapLayer = nullptr;
    Sprite* buttonClose = nullptr;

};

#endif // __JOYSTICK_H__