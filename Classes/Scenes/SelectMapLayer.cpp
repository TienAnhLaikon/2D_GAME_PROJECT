#include "SelectMapLayer.h"
#include "Game/Game.h"
#include "Map/Map.h"
#include "UserInterface/InGame/TeleportEffect.h"
bool SelectMapLayer::init() {
	if (!Layer::create()) return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_background = Sprite::create("res/background-img-2.png");
	this->addChild(_background);
	_closeButton = Sprite::create("res/c_header_close.png");
	_closeButton->setScale(0.6 * Director::getInstance()->getContentScaleFactor());
	_closeButton->setPosition(Vec2(_background->getContentSize().width / 2 - _closeButton->getContentSize().width, _background->getContentSize().height / 2 - _closeButton->getContentSize().width));
	this->addChild(_closeButton, 50);

	auto title = Label::createWithTTF("Chapter 1", "fonts/Diablo Light.ttf", 13);
	title->setPosition(-title->getContentSize().width / 2, _background->getContentSize().width / 2 - title->getContentSize().height / 2);
	this->addChild(title);

	_currentMapArrow = Sprite::create("res/current-level-map.png");
	this->addChild(_currentMapArrow, 20);

	map0 = Node::create();
	auto map0Label = Label::createWithTTF("map 0", "fonts/Diablo Heavy.ttf", 10);
	map0Label->setColor(Color3B::BLACK);
	map0Label->setPosition(0, map0Label->getContentSize().height * 1.5);
	map0->addChild(map0Label);
	listOfMap.push_back(map0);

	auto map0Sprite = Sprite::create("res/pngwing1.png");
	map0->addChild(map0Sprite);
	map0->setContentSize(map0Sprite->getContentSize());

	map0->setPosition(-_background->getContentSize().width / 2 + 30 * Director::getInstance()->getContentScaleFactor(), -_background->getContentSize().height / 2 + _background->getContentSize().height / 3);
	this->addChild(map0, 50);

	map1 = Node::create();
	auto map1Label = Label::createWithTTF("map 1", "fonts/Diablo Heavy.ttf", 10);
	map1Label->setColor(Color3B::BLACK);
	map1Label->setPosition(0, map1Label->getContentSize().height * 1.5);
	map1->addChild(map1Label);
	listOfMap.push_back(map1);

	auto map1Sprite = Sprite::create("res/pngwing2.png");
	map1->addChild(map1Sprite);
	map1->setContentSize(map1Sprite->getContentSize() * 2);

	map1->setPosition(-_background->getContentSize().width / 2 + _background->getContentSize().width * 0.25, _background->getContentSize().height / 4);

	for (Vec2 i = map0->getPosition(); i != map1->getPosition();) {
		auto dashed = Sprite::create("res/dashed.png");
		dashed->setPosition(i);
		this->addChild(dashed, 80);
		Vec2 direction = map1->getPosition() - map0->getPosition();
		direction.normalize();
		if (i == map0->getPosition()) {
			i = i + direction * 18 * Director::getInstance()->getContentScaleFactor();
			dashed->setVisible(false);
			continue;
		}
		float angle = atan2(direction.y, direction.x) * 180 / M_PI;
		dashed->setRotation(-angle);
		int distance = 10;
		i = i + direction * distance;
		if (i.distance(map1->getPosition()) < 10 * Director::getInstance()->getContentScaleFactor()) break;
	}

	this->addChild(map1, 50);


	map2 = Node::create();
	auto map2Label = Label::createWithTTF("map 2", "fonts/Diablo Heavy.ttf", 10);
	map2Label->setColor(Color3B::BLACK);
	map2Label->setPosition(0, map2Label->getContentSize().height * 1.5);
	map2->addChild(map2Label);
	listOfMap.push_back(map2);

	auto map2Sprite = Sprite::create("res/pngwing3.png");
	map2->addChild(map2Sprite);
	map2->setContentSize(map2Sprite->getContentSize() * 2);

	map2->setPosition(-_background->getContentSize().width / 2 + _background->getContentSize().width * 0.5, -_background->getContentSize().height / 2 + _background->getContentSize().height / 2.5);
	this->addChild(map2, 50);

	for (Vec2 i = map1->getPosition(); i != map2->getPosition();) {
		auto dashed = Sprite::create("res/dashed.png");
		dashed->setPosition(i);
		this->addChild(dashed, 80);
		Vec2 direction = map2->getPosition() - map1->getPosition();
		direction.normalize();
		if (i == map1->getPosition()) {
			i = i + direction * 18 * Director::getInstance()->getContentScaleFactor();
			dashed->setVisible(false);
			continue;
		}
		float angle = atan2(direction.y, direction.x) * 180 / M_PI;
		dashed->setRotation(-angle);
		int distance = 10;
		i = i + direction * distance;
		if (i.distance(map2->getPosition()) < 10 * Director::getInstance()->getContentScaleFactor()) break;
	}

	map3 = Node::create();
	auto map3Label = Label::createWithTTF("map 3", "fonts/Diablo Heavy.ttf", 10);
	map3Label->setColor(Color3B::BLACK);
	map3Label->setPosition(0, map3Label->getContentSize().height * 1.5);
	map3->addChild(map3Label);
	listOfMap.push_back(map3);

	auto map3Sprite = Sprite::create("res/pngwing4.png");
	map3->addChild(map3Sprite);
	map3->setContentSize(map3Sprite->getContentSize() * 2);

	map3->setPosition(-_background->getContentSize().width / 2 + _background->getContentSize().width - 36 * Director::getInstance()->getContentScaleFactor(), -_background->getContentSize().height / 2 + _background->getContentSize().height / 2.3);
	this->addChild(map3, 50);

	for (Vec2 i = map2->getPosition(); i != map3->getPosition();) {
		auto dashed = Sprite::create("res/dashed.png");
		dashed->setPosition(i);
		this->addChild(dashed, 80);
		Vec2 direction = map3->getPosition() - map2->getPosition();
		direction.normalize();
		if (i == map2->getPosition()) {
			i = i + direction * 18 * Director::getInstance()->getContentScaleFactor();
			dashed->setVisible(false);
			continue;
		}
		float angle = atan2(direction.y, direction.x) * 180 / M_PI;
		dashed->setRotation(-angle);
		int distance = 10;
		i = i + direction * distance;
		if (i.distance(map3->getPosition()) < 10 * Director::getInstance()->getContentScaleFactor()) break;
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(SelectMapLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	hide();
	return true;
}

bool SelectMapLayer::onTouchBegan(Touch* touch, Event* event) {
	if (isShow()) {
		auto startPoint = this->convertToNodeSpace(touch->getLocation());

		if (_closeButton->getBoundingBox().containsPoint(startPoint)) {
			hide();
			Scene* currentScene = Director::getInstance()->getRunningScene();
			game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
			game->resumeGame();
			return true;
		}
		Rect map0Rect = Rect(map0->getPosition() - map0->getBoundingBox().size / 2, map0->getBoundingBox().size);
		Rect map1Rect = Rect(map1->getPosition() - map1->getBoundingBox().size / 2, map1->getBoundingBox().size);
		Rect map2Rect = Rect(map2->getPosition() - map2->getBoundingBox().size / 2, map2->getBoundingBox().size);
		Rect map3Rect = Rect(map3->getPosition() - map3->getBoundingBox().size / 2, map3->getBoundingBox().size);
		if(map0Rect.containsPoint(startPoint) || map1Rect.containsPoint(startPoint) || map2Rect.containsPoint(startPoint) || map3Rect.containsPoint(startPoint))
		{
			Scene* currentScene = Director::getInstance()->getRunningScene();
			game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
			int mapIndex = -1;
			if (map0Rect.containsPoint(startPoint)) {
				mapIndex = 0;
			}
			else if (map1Rect.containsPoint(startPoint)) {
				mapIndex = 1;
			}
			else if (map2Rect.containsPoint(startPoint)) {
				mapIndex = 2;
			}
			else if (map3Rect.containsPoint(startPoint)) {
				mapIndex = 3;
			}
			else return false;
			if (game->gameMap->currentMap != mapIndex && mapIndex != -1) {
				auto hideLayer = CallFunc::create([this]() {
					if (game) {
						game->save();
						hide();
						game->inGameUI->setVisible(false);
						game->compass->setVisible(false);
					}
					
					//game->pauseGame();
					});
				
				auto hideOvlay = CallFunc::create([this]() {
					if (game) {
						Node* overlay = LayerColor::create(Color4B::BLACK);
						overlay->setName("ovlay");
						game->gameMap->getTiledMap()->addChild(overlay, 1000);
						overlay->setVisible(true);
						overlay->setOpacity(0);
						overlay->runAction(FadeIn::create(0.4));
					}
					
					});
				auto loadMapAction = CallFunc::create([this, mapIndex]() {
					if (game) {
						game->resumeGame();
						if (game->compass)
							game->compass->setVisible(true);
						if (game->inGameUI)
							game->inGameUI->setVisible(true);

						game->loadMap(mapIndex);
						game->save();
						NotificationManager::getInstance()->showMessageNotification("Saved!", Vec2::ZERO, Color3B::GREEN, 16);
						

						if (mapIndex == 0 && game->gameMap != nullptr && game->getPlayer())
							game->getPlayer()->setPosition(game->gameMap->getEndPoint().getMidX(), game->gameMap->getEndPoint().getMidY());
					}
					});
					
				auto sequence = Sequence::create(
					hideLayer,
					hideOvlay,
					DelayTime::create(0.5),
					loadMapAction,
					nullptr
				);
				this->runAction(sequence);
			}
			else {
				NotificationManager::getInstance()->showMessageNotification("You are in this map!", Vec2::ZERO, Color3B::RED, 16);
			}
			return true;
		}
	}
	return false;
}
void SelectMapLayer::hide() {
	this->setVisible(false);
}
void SelectMapLayer::show() {
	this->setVisible(true);
	if (!game) {
		Scene* currentScene = Director::getInstance()->getRunningScene();
		game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
	}
	_currentMapArrow->setPosition(listOfMap[game->gameMap->currentMap]->getPosition() + Vec2(0, _currentMapArrow->getContentSize().height * 2.5));
	if (_currentMapArrow->getActionByTag(101)) {
		_currentMapArrow->stopActionByTag(101);
	}
	auto moveAction = RepeatForever::create(Sequence::create(MoveTo::create(1.2f, _currentMapArrow->getPosition() + Vec2(0, _currentMapArrow->getContentSize().height * 0.35)), MoveTo::create(1.2f, _currentMapArrow->getPosition() - Vec2(0, _currentMapArrow->getContentSize().height * 0.35)), nullptr));
	moveAction->setTag(101);
	_currentMapArrow->runAction(moveAction);
}

bool SelectMapLayer::isShow() {
	return this->isVisible();
}