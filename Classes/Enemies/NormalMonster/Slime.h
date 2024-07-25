#ifndef __SLIME_H__
#define __SLIME_H__

#include "cocos2d.h"
#include "NormalMonster.h"
USING_NS_CC;

class Slime : public NormalMonster
{
public:
    virtual bool init();
    //Cac phuong thuc danh rieng cho NormalMonster
    void loadAnimate();

    CREATE_FUNC(Slime);
private:
};

#endif 
