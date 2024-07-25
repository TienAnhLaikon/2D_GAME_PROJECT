#ifndef __SKELETON_H__
#define __SKELETON_H__

#include "cocos2d.h"
#include "NormalMonster.h"
USING_NS_CC;

class Skeleton : public NormalMonster
{
public:
    virtual bool init();
    //Cac phuong thuc danh rieng cho NormalMonster
    void loadAnimate();
    void setAttackSpeed(float attackSpeed);

    CREATE_FUNC(Skeleton);
private:
};

#endif 
