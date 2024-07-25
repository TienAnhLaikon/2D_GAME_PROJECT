#include "PlayerWalk.h"
#include "Player/Player.h"

void PlayerWalk::EnterState() {
	//CCLOG("Hello From Walk State");

	if (player->getCurrentDirection() == 1) {
		player->getCurrentSprite()->runAction(RepeatForever::create(player->armWalk_E));
	}

	else if (player->getCurrentDirection() == 2) {
		player->getCurrentSprite()->runAction(RepeatForever::create(player->armWalk_NE));
	}

	else if (player->getCurrentDirection() == 3) {
		player->getCurrentSprite()->runAction(RepeatForever::create(player->armWalk_N));
	}

	else if (player->getCurrentDirection() == 4) {
		player->getCurrentSprite()->runAction(RepeatForever::create(player->armWalk_NW));
	}

	else if (player->getCurrentDirection() == 5) {
		player->getCurrentSprite()->runAction(RepeatForever::create(player->armWalk_W));
	}

	else if (player->getCurrentDirection() == 6) {
		player->getCurrentSprite()->runAction(RepeatForever::create(player->armWalk_SW));
	}

	else if (player->getCurrentDirection() == 7) {
		player->getCurrentSprite()->runAction(RepeatForever::create(player->armWalk_S));
	}

	else if (player->getCurrentDirection() == 8) {
		player->getCurrentSprite()->runAction(RepeatForever::create(player->armWalk_SE));
	}
	//float soundVolume = SettingsData::getInstance()->getSoundSlider() / 100.0f;
	//int footstep_soundId = Audio::getInstance()->play2d("sound/sounds effect/footstep_sound1.mp3", false, soundVolume);
	//UserDefault::getInstance()->setIntegerForKey("sound_effect", footstep_soundId);
	lastDir = player->getCurrentDirection();

}

void PlayerWalk::ExitState() {
	if (player->getCurrentSprite()->getNumberOfRunningActions() > 0)player->getCurrentSprite()->stopAllActions();
}

void PlayerWalk::UpdateState() {
	if (lastDir != player->getCurrentDirection()) {
		player->SwitchState(player->walkState);
	}
	if (!player->isMoving()) {
		player->SwitchState(player->idleState);
	}

}

