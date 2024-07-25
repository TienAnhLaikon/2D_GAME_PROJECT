#include "PlayerDead.h"
#include "Player/Player.h"
#include "AudioEngine.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
#include "Game/Game.h"
void PlayerDead::EnterState() {
	//CCLOG("Hello From Dead State");
	Animate* animateResult;
	UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/player_death_sound.mp3", false, SettingsData::getInstance()->getSoundSlider()));
	if (player->getCurrentDirection() == 1) {
		animateResult = ((player->specialDeath_E));
	}

	else if (player->getCurrentDirection() == 2) {
		animateResult = ((player->specialDeath_NE));
	}

	else if (player->getCurrentDirection() == 3) {
		animateResult = ((player->specialDeath_N));
	}

	else if (player->getCurrentDirection() == 4) {
		animateResult = ((player->specialDeath_NW));
	}

	else if (player->getCurrentDirection() == 5) {
		animateResult = ((player->specialDeath_W));
	}

	else if (player->getCurrentDirection() == 6) {
		animateResult = ((player->specialDeath_SW));
	}

	else if (player->getCurrentDirection() == 7) {
		animateResult = ((player->specialDeath_S));
	}

	else if (player->getCurrentDirection() == 8) {
		animateResult = ((player->specialDeath_SE));
	}

	player->getCurrentSprite()->runAction(Sequence::create(animateResult, 
		cocos2d::CallFunc::create([this]() {
			Scene* currentScene = Director::getInstance()->getRunningScene();
			Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
			game->showDiedLayer();
			//game
			//game->pauseGame();
				}), nullptr));

	//player->getCurrentSprite()->runAction(
	//	cocos2d::CallFunc::create([this]() {
	//		player->runAction(RemoveSelf::create());
	//		}));
}

void PlayerDead::ExitState() {
	if (player->getNumberOfRunningActions() > 0)player->getCurrentSprite()->stopAllActions();
}

void PlayerDead::UpdateState() {


}

