#include "NotificationManager/NotificationManager.h"
#include "HelloWorldScene.h"
#include "Game/Game.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include "UserInterface/InGame/InGameUI.h"
NotificationManager* NotificationManager::instance = nullptr;

void NotificationManager::showMessageNotification(const std::string& message, Vec2 positionApply, Color3B colorApply, int fontSize)
{
	Player* _player;
	// thm label v sprite vo scene
	Scene* currentScene = Director::getInstance()->getRunningScene();
	if (currentScene)
	{
		Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
		if (game)
		{
			auto children = game->gameMap->getTiledMap()->getChildren();
			for (const auto& child : children)
			{
				// Kim tra xem i tng con c phi l Player khng
				auto player = dynamic_cast<Player*>(child);
				if (player)
				{
					_player = player;
				}
			}
		}
	}

	auto notificationLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", fontSize);
	notificationLabel->setColor(colorApply);
	notificationLabel->setString(message);
	auto backgroundSprite = Sprite::create("res/black-background.png");
	backgroundSprite->setTextureRect(Rect(0, 0, notificationLabel->getContentSize().width, notificationLabel->getContentSize().height));
	backgroundSprite->setName("Notify");
	backgroundSprite->addChild(notificationLabel, 1);
	notificationLabel->setPosition(Vec2(backgroundSprite->getBoundingBox().size.width / 2, backgroundSprite->getBoundingBox().size.height / 2));

	backgroundSprite->setPosition(positionApply);
/*	if(!InGameUI::getInstance(_player)->getChildByName("Notify"))*/InGameUI::getInstance(_player)->addChild(backgroundSprite, 100);
	//scene->addChild(backgroundSprite, 1000);

	//Action
	auto fadeOut = FadeOut::create(1.0f);
	notificationLabel->runAction(fadeOut);
	backgroundSprite->runAction(Sequence::create(fadeOut, RemoveSelf::create(), nullptr));
}
void NotificationManager::showSkillEffectivePeriod(const std::string& spritePath, float time) {
	//Player* _player;
	//// thm label v sprite vo scene
	//Scene* currentScene = Director::getInstance()->getRunningScene();
	//if (currentScene)
	//{
	//	Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
	//	if (game)
	//	{
	//		auto children = game->gameMap->getTiledMap()->getChildren();
	//		for (const auto& child : children)
	//		{
	//			// Kim tra xem i tng con c phi l Player khng
	//			auto player = dynamic_cast<Player*>(child);
	//			if (player)
	//			{
	//				_player = player;
	//			}
	//		}
	//	}
	//}
	//auto notificationLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 17);
	//notificationLabel->setColor(Color3B::WHITE);
	//notificationLabel->setString("");
	//auto skill = Sprite::create(spritePath);
	//skill->setTextureRect(Rect(0, 0, notificationLabel->getContentSize().width, notificationLabel->getContentSize().height));
	//skill->addChild(notificationLabel, 1);
	//notificationLabel->setPosition(Vec2(skill->getBoundingBox().size.width / 2, skill->getBoundingBox().size.height / 2));



	//// To mt hm lambda  cp nht label
	//auto updateLabelLambda = [notificationLabel](float dt) {
	//	// Set the label's string to something new
	//	notificationLabel->setString();
	//	};

	//// Schedule the lambda function to run every 1 second
	//this->schedule([updateLabelLambda](float dt) {
	//	updateLabelLambda(dt);
	//	}, 1.0f, "label_update");
}

void NotificationManager::hideMessageNotification()
{

}

NotificationManager::NotificationManager()
{
}

NotificationManager::~NotificationManager()
{
}
