#ifndef __MINIMAP__
#define __MINIMAP__

#include "cocos2d.h"

USING_NS_CC;
class Player;
class Compass : public Node
{
public:
    static Compass* create();
    bool init();
    const int PLAYER_DOT_TAG = 1001;
    void setTarget(Vec2 playerPosition, Vec2 targetPosition);
    void resetTarget() { _target = Vec2::ZERO; };

    Sprite* compassBorder;
    ~Compass();
private:
    Vec2 _target = Vec2::ZERO;
    Sprite* compassNeedle;

};

#endif // __MINIMAP__