#ifndef __BABY_SPIDER_H__
#define __BABY_SPIDER_H__

#include "cocos2d.h"
#include "NormalMonster.h"
USING_NS_CC;

class BabySpider : public NormalMonster
{
public:
    virtual bool init();
    // Cac phuong thuc danh rieng cho NormalMonster
    void loadAnimate();
    void setAttackSpeed(float attackSpeed);

    CREATE_FUNC(BabySpider);

private:
};

#endif
