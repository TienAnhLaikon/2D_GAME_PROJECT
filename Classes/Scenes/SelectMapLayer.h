#ifndef __SELECT_MAP_LAYER__
#define __SELECT_MAP_LAYER__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class Game;
class SelectMapLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    Sprite* _background;
    void hide();
    void show();
    bool isShow();
    bool onTouchBegan(Touch* touch, Event* event);
    CREATE_FUNC(SelectMapLayer);
private:
    Game* game;
    bool _isShow;
    Sprite* _closeButton;
    Sprite* _currentMapArrow;
    Node* map0;
    Node* map1;
    Node* map2;
    Node* map3;
    std::vector<Node*> listOfMap;
};

#endif // __SELECT_MAP_LAYER__