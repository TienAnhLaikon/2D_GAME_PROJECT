#include "HamburgerMenu.h"

bool HamburgerMenu::init()
{
	if (!Node::init()) {
		return false;
	}
	//auto visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//hamburgerMenuList = Sprite::create("res/menu_bg.png");
	//hamburgerMenuList->setScale(0.27);
	//
 //   // To nt "RESUME"
 //   auto resumeButton = Sprite::create("res/button_orange.png");
 //   resumeButton->setPosition(Vec2(hamburgerMenuList->getContentSize().width / 2, hamburgerMenuList->getContentSize().height - 100)); // t v tr ca nt "RESUME"

 //   // To label hin th ch "RESUME"
 //   auto label = Label::createWithTTF("RESUME", "fonts/Diablo Light.ttf", 20);
 //   label->setPosition(Vec2(resumeButton->getContentSize().width / 2, resumeButton->getContentSize().height / 2));
 //   resumeButton->addChild(label); // Thm label vo nt "RESUME"

 //   // To listener cho nt "RESUME"
 //   auto touchListener = EventListenerTouchOneByOne::create();
 //   touchListener->setSwallowTouches(true);
 //   touchListener->onTouchBegan = [&](Touch* touch, Event* event) {
 //       if (resumeButton->getBoundingBox().containsPoint(touch->getLocation())) {
 //           unpauseGameCallBack(nullptr);
 //           return true;
 //       }
 //       return false;
 //       };
 //   _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, resumeButton);

 //   // Thm nt "RESUME" vo sprite hamburgerMenuList
 //   hamburgerMenuList->addChild(resumeButton);

 //   // Thm sprite hamburgerMenuList vo layer
 //   this->addChild(hamburgerMenuList);

 //   hamburgerMenuList->setVisible(false);
    return true;
}

bool HamburgerMenu::isShow()
{
	return hamburgerMenuList->isVisible();
}

void HamburgerMenu::show()
{
	hamburgerMenuList->setVisible(true);
	//Director::getInstance()->pause();

}

void HamburgerMenu::hide()
{
	hamburgerMenuList->setVisible(false);

}

void HamburgerMenu::unpauseGameCallBack(Ref* pSender)
{
	//Director::getInstance()->resume();
	CCLOG("RESUME");
	hide();

}

HamburgerMenu::~HamburgerMenu()
{
}
