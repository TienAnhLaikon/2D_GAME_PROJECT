#include "BombRainbow.h"
#include "HelloWorldScene.h"
#include"Map/Map.h"
#include "Player/Player.h"
#include "Enemies/Boss/Boss.h"

bool BombRainbow::init() {

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
		auto skillSprite = Sprite::createWithSpriteFrameName("fire_combust (1).png");
		skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
		skillSprite->retain();
		_skillSprites.push_back(skillSprite);
	}

	for (int i = 0; i < 8; i++) {
		auto skillAnimate = Animate::create(Engine::createAnimation2("fire_combust", 60, 0.05));
		skillAnimate->retain();
		_animates.push_back(skillAnimate);
	}

	skillDamage = 70;

	return true;
}

void BombRainbow::performSkill(Vec2 target) {
	auto boss = dynamic_cast<Boss*>(this->getParent());
	auto scene = this->getParent()->getScene();
	//_skillSprite->setPosition(target);
	for (auto skillSprite : _skillSprites) {
		skillSprite->setPosition(boss->getPosition());
		scene->addChild(skillSprite);
	}

	for (int i = 0; i < 8; i++) {
		//falling trajectory
		float duration = 2.0f;
		float amplitude = 15.0f;
		float frequency = 1.0f;
		Vec2 startPosition1 = boss->getPosition();
		Vec2 endPosition1 = _rangeIndicators[i]->getPosition();

		int steps = 100;
		float stepDuration = duration / steps;

		Vector<FiniteTimeAction*> actions1;

		for (int i = 0; i <= steps; ++i) {
			float t = (float)i / steps;
			float x = startPosition1.x + t * (endPosition1.x - startPosition1.x);
			float y = startPosition1.y + t * (endPosition1.y - startPosition1.y) + amplitude * sinf(frequency * M_PI * t);

			actions1.pushBack(MoveTo::create(stepDuration, Vec2(x, y)));
		}
		auto sequence1 = Sequence::create(actions1);
		auto spawn = Spawn::create(sequence1, _animates[i], nullptr);
		auto resultSequence = Sequence::create(spawn,RemoveSelf::create(), nullptr);
		_skillSprites[i]->runAction(resultSequence);

	}

	for (auto rangeIndicator : _rangeIndicators) {
		rangeIndicator->runAction(RemoveSelf::create());
	}

	//skillSprite->runAction(Sequence::create(_skillAnimate, RemoveSelf::create(), nullptr));

	//// Lấy scene chính từ Director để kiểm tra có enemy nào trong phạm vi skill ko
	//Scene* currentScene = Director::getInstance()->getRunningScene();
	//if (currentScene) {
	//	Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
	//	if (game) {
	//		auto children = game->gameMap->getTiledMap()->getChildren();

	//		for (const auto& child : children) {
	//			auto player = dynamic_cast<Player*>(child);
	//			if (player) {
	//				//Kiểm tra xem enemy có nằm trong vùng ảnh hưởng của skill hay không
	//				if (target.distance(player->getPosition()) <= 80) {
	//					player->takeDamage(skillDamage);
	//					if (player->getCurrentHP() == 0) {

	//					}
	//				}
	//			}
	//		}
	//	}
	//}

}

void BombRainbow::castRangeIndicator(Vec2 target) {
	auto boss = dynamic_cast<Boss*>(this->getParent());
	auto scene = this->getParent()->getScene();
	//_rangeIndicator->setPosition(target);

	for (int i = 0; i < 8; i++) {
		Vec2 dir = Vec2(1, 0);
		if (i == 0) {
			dir = Vec2(1, 0);
		}
		else if (i == 1) {
			dir = Vec2(sqrt(2), sqrt(2));
		}
		else if (i == 2) {
			dir = Vec2(0, 1);
		}
		else if (i == 3) {
			dir = Vec2(-sqrt(2), sqrt(2));
		}
		else if (i == 4) {
			dir = Vec2(-1, 0);
		}
		else if (i == 5) {
			dir = Vec2(-sqrt(2), -sqrt(2));
		}
		else if (i == 6) {
			dir = Vec2(0, -1);
		}
		else if (i == 7) {
			dir = Vec2(sqrt(2), -sqrt(2));
		}
		dir.normalize();
		Vec2 tar = boss->getPosition() + dir * _rangeIndicators[i]->getBoundingBox().size.width;
		_rangeIndicators[i]->setPosition(tar);
		scene->addChild(_rangeIndicators[i]);

	}

}