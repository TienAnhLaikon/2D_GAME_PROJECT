#include "Summon.h"
#include "HelloWorldScene.h"
#include"Map/Map.h"
#include "Player/Player.h"
#include "Enemies/Boss/Boss.h"
#include "Enemies/NormalMonster/NormalMonsterFactory.h"
#include "Game/Game.h"

bool Summon::init() {

	for (int i = 0; i < 3; i++) {
		auto skillSprite = Sprite::createWithSpriteFrameName("summon (1).png");
		skillSprite->setOpacity(100);
		skillSprite->setAnchorPoint(Vec2(0, 0.5));
		skillSprite->setScale(0.60);
		skillSprite->setRotation(120 * i);
		skillSprite->retain();
		_skillSprites.push_back(skillSprite);
	}


	for (int i = 0; i < 3; i++) {
		auto skillAnimate = Animate::create(Engine::createAnimation2("summon", 61, 0.025));
		skillAnimate->retain();
		_animates.push_back(skillAnimate);
	}

	skillDamage = 70;

	return true;
}

void Summon::performSkill(Vec2 target) {
	auto boss = dynamic_cast<Boss*>(this->getParent());
		auto scene = this->getParent()->getScene();
		// Lấy scene chính từ Director
		Scene* currentScene = Director::getInstance()->getRunningScene();
		if (currentScene) {
			Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
			if (game) {
				auto children = game->gameMap->getTiledMap()->getChildren();
				for (const auto& child : children) {
					// Kiểm tra xem đối tượng con có phải là Player không
					auto player = dynamic_cast<Player*>(child);
					if (player) {
						//Show Enemies
						for (int i = 0; i < 3; i++) {
							auto monster = NormalMonsterFactory::createNormalMonster("Skeleton");
							monster->setPosition(_skillSprites[i]->getPosition());
							monster->setTarget(player);
							monster->setName("Skeleton");
							game->gameMap->getTiledMap()->addChild(monster, 9.5);
							monster->currentState = monster->walkState;
							monster->currentState->EnterState();
							game->listOfMonster.push_back(monster);
						}
					}
				}
			}
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

void Summon::castRangeIndicator(Vec2 target) {
	auto boss = dynamic_cast<Boss*>(this->getParent());
	auto scene = this->getParent()->getScene();
	for (int i = 0; i < 3; i++) {
		Vec2 dir = Vec2(1, 0);
		if (i == 0) {
			dir = Vec2(0, 1);
		}
		else if (i == 1) {
			dir = Vec2(sqrt(2)/2, -sqrt(2)/2);
		}
		else if (i == 2) {
			dir = Vec2(-sqrt(2) / 2, -sqrt(2) / 2);
		}
		dir.normalize();
		Vec2 tar = boss->getPosition() + dir * _skillSprites[i]->getBoundingBox().size.width;
		_skillSprites[i]->setPosition(tar);
		scene->addChild(_skillSprites[i]);
		auto sqe = Sequence::create(Repeat::create(_animates[i], 2), RemoveSelf::create(), nullptr);
		_skillSprites[i]->runAction(sqe);


	}
}