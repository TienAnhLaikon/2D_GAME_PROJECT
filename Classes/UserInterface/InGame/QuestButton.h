#ifndef __QUEST_BUTTON__
#define __QUEST_BUTTON__
#include "cocos2d.h"
#include "Quest/QuestManager.h"
#include "Game/Game.h"
USING_NS_CC;
class QuestButton : public cocos2d::Node
{
public:
    static QuestButton *create();
    virtual bool init();
    bool isShow() {
        return QuestManager::getInstance()->isShowDetail;
    }
    bool onTouchBegan(Touch *touch, Event *event);
    Size getSpriteSize() const { return btnSprite->getContentSize(); };
    Sprite* btnSprite;
private:


};

#endif // __QUEST_BUTTON__