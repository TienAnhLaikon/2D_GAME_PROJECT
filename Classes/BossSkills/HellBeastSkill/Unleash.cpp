#include "Unleash.h"
#include "HelloWorldScene.h"
#include"Map/Map.h"
#include "Player/Player.h"
#include "Enemies/Boss/Boss.h"

bool Unleash::init() {

	for (int i = 0; i < 8; i++) {
		auto rangeIndicator = Sprite::create("skill/AOERangeSprite/CircleRangeBoss.png");
		rangeIndicator->setOpacity(100);
		rangeIndicator->setAnchorPoint(Vec2(0, 0.5));
		rangeIndicator->setScale(0.60);
		rangeIndicator->setRotation(45 * i);
		rangeIndicator->retain();
		_rangeIndicators.push_back(rangeIndicator);
	}

	for (int i = 0; i < 8; i++) {
		auto skillSprite = Sprite::createWithSpriteFrameName("fire_conflagration (1).png");
		skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
		skillSprite->setRotation(45 * i);
		skillSprite->retain();
		_skillSprites.push_back(skillSprite);
	}

	for (int i = 0; i < 8; i++) {
		auto skillAnimate = Animate::create(Engine::createAnimation2("fire_conflagration", 90, 0.025));
		skillAnimate->retain();
		_animates.push_back(skillAnimate);
	}

	skillDamage = 70;

	return true;
}

void Unleash::performSkill(Vec2 target) {
	auto boss = dynamic_cast<Boss*>(this->getParent());
	auto scene = this->getParent()->getScene();
	//_skillSprite->setPosition(target);
	for (auto skillSprite : _skillSprites) {
		skillSprite->setPosition(boss->getPosition());
		scene->addChild(skillSprite);
	}
	for (int i = 0; i < 8; i++) {
		Vec2 dir = Vec2(1, 0);
		if (i == 0) {
			dir = Vec2(1, 0);
		}
		else if (i == 1) {
			dir = Vec2(sqrt(2) / 2, sqrt(2) / 2);
		}
		else if (i == 2) {
			dir = Vec2(0, 1);
		}
		else if (i == 3) {
			dir = Vec2(-sqrt(2) / 2, sqrt(2) / 2);
		}
		else if (i == 4) {
			dir = Vec2(-1, 0);
		}
		else if (i == 5) {
			dir = Vec2(-sqrt(2) / 2, -sqrt(2) / 2);
		}
		else if (i == 6) {
			dir = Vec2(0, -1);
		}
		else if (i == 7) {
			dir = Vec2(sqrt(2) / 2, -sqrt(2) / 2);
		}
		dir.normalize();
		Vec2 tar = _skillSprites[i]->getPosition() + dir * _rangeIndicators[i]->getBoundingBox().size.width;
		auto moveTo = MoveTo::create(1.0f, tar);
		auto spawn = Spawn::create(moveTo, _animates[i], nullptr);
		_skillSprites[i]->runAction(Sequence::create(spawn, RemoveSelf::create(), nullptr));
	}

	for (auto rangeIndicator : _rangeIndicators) {
		rangeIndicator->runAction(RemoveSelf::create());
	}


	// Lấy scene chính từ Director để kiểm tra có enemy nào trong phạm vi skill ko
	Scene* currentScene = Director::getInstance()->getRunningScene();
	if (currentScene) {
		Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
		if (game) {
			auto children = game->gameMap->getTiledMap()->getChildren();

			for (const auto& child : children) {
				auto player = dynamic_cast<Player*>(child);
				if (player) {
					//Kiểm tra xem enemy có nằm trong vùng ảnh hưởng của skill hay không
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

void Unleash::castRangeIndicator(Vec2 target) {
	auto boss = dynamic_cast<Boss*>(this->getParent());
	auto scene = this->getParent()->getScene();
	//_rangeIndicator->setPosition(target);

	for (int i = 0; i < 8; i++) {
		Vec2 dir = Vec2(1, 0);
		if (i == 0) {
			dir = Vec2(1, 0);
		}
		else if (i == 1) {
			dir = Vec2(sqrt(2) / 2, sqrt(2) / 2);
		}
		else if (i == 2) {
			dir = Vec2(0, 1);
		}
		else if (i == 3) {
			dir = Vec2(-sqrt(2) / 2, sqrt(2) / 2);
		}
		else if (i == 4) {
			dir = Vec2(-1, 0);
		}
		else if (i == 5) {
			dir = Vec2(-sqrt(2) / 2, -sqrt(2) / 2);
		}
		else if (i == 6) {
			dir = Vec2(0, -1);
		}
		else if (i == 7) {
			dir = Vec2(sqrt(2) / 2, -sqrt(2) / 2);
		}
		dir.normalize();
		Vec2 tar = _skillSprites[i]->getPosition() + dir * _rangeIndicators[i]->getBoundingBox().size.width;
		_rangeIndicators[i]->setPosition(tar);
		scene->addChild(_rangeIndicators[i]);
	}

}