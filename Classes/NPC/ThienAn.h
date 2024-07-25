#ifndef  _THIENAN_H_
#define _THIENAN_H_
#include "cocos2d.h"
#include "NPC.h"
USING_NS_CC;
class ThienAn : public NPC {
public:
	virtual bool init();
	void loadAnimate();

	CREATE_FUNC(ThienAn);
};
#endif // ! _THIENAN_H_
