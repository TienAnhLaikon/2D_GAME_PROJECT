#include "LevelUpPopup.h"

LevelUpPopup* LevelUpPopup::create(int level) {
	LevelUpPopup* healthBar = new LevelUpPopup();
	if (healthBar && healthBar->init(level)) {
		healthBar->autorelease();
		return healthBar;
	}
	CC_SAFE_DELETE(healthBar);
	return nullptr;
}

bool LevelUpPopup::init(int level) {
	if (!Node::init()) {
		return false;
	}

	levelUpSprite = Sprite::create("res/level-up-popup.png");
	levelUpSprite->setScale(0.12 * Director::getInstance()->getContentScaleFactor());
	levelUpMsg = Label::createWithTTF("Well done on reaching a new level!" + std::to_string(level), "fonts/Diablo Light.ttf", 10);
	levelUpMsg->setTextColor(Color4B::GREEN);
	levelUpMsg->setPosition(Vec2(0, -levelUpSprite->getContentSize().height * levelUpSprite->getScaleY() + 12));
	levelUpLabel = Label::createWithTTF("LV " + std::to_string(level), "fonts/Diablo Light.ttf", 16);
	this->addChild(levelUpSprite);
	this->addChild(levelUpMsg);
	this->addChild(levelUpLabel);
	this->setVisible(false);
	
	return true;
}

void LevelUpPopup::show(int level) {
	levelUpLabel->setString("" + std::to_string(level));
	this->setVisible(true);
	auto sqe = Sequence::create(FadeIn::create(0.8), DelayTime::create(3.5), FadeOut::create(1.5), DelayTime::create(0.5), RemoveSelf::create(), nullptr);
	this->runAction(sqe);
}