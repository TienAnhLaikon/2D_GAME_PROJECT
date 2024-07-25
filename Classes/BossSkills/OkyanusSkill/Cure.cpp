#include "Cure.h"
#include "HelloWorldScene.h"
#include"Map/Map.h"
#include "Player/Player.h"
bool Cure::init() {

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/BossSkill/OkyanusSkill/cure/cure.plist", "skill/SkillSprite/BossSkill/OkyanusSkill/cure/cure.png");

	_rangeIndicator = Sprite::create("skill/AOERangeSprite/CircleRangeBoss.png");
	_rangeIndicator->setOpacity(130);
	_rangeIndicator->setAnchorPoint(Vec2(0.5, 0.5));
	_rangeIndicator->retain();

	_skillSprite = Sprite::createWithSpriteFrameName("cure (1).png");
	_skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
	_skillSprite->retain();

	_skillAnimate = Animate::create(Engine::createAnimation2("cure", 100, 0.015));
	_skillAnimate->retain();

	skillDamage = 100;

	return true;
}

void Cure::performSkill(Vec2 target) {
	if (!_skillSprite->getParent()) {
		auto scene = this->getParent()->getScene();
		_skillSprite->setPosition(target);
		scene->addChild(_skillSprite);
		_skillSprite->runAction(Sequence::create(_skillAnimate, RemoveSelf::create(), nullptr));
		_rangeIndicator->runAction(RemoveSelf::create());

		// Ly scene chnh t Director  kim tra c enemy no trong phm vi skill ko
		Scene* currentScene = Director::getInstance()->getRunningScene();
		if (currentScene) {
			Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
			if (game) {
				auto children = game->gameMap->getTiledMap()->getChildren();

				for (const auto& child : children) {
					auto player = dynamic_cast<Player*>(child);
					if (player) {
						//Kim tra xem enemy c nm trong vng nh hng ca skill hay khng
						if (target.distance(player->getPosition()) <= 80) {
							player->takeDamage(skillDamage);
							if (player->getCurrentHP() == 0) {

							}
						}
					}
				}
			}
		}


	}

}

void Cure::castRangeIndicator(Vec2 target) {
	auto scene = this->getParent()->getScene();
	_rangeIndicator->setPosition(target);
	scene->addChild(_rangeIndicator);

}