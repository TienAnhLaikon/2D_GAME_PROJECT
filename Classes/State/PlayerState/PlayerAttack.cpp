#include "PlayerAttack.h"
#include "Player/Player.h"
#include "AudioEngine.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
void PlayerAttack::EnterState() {
    // CCLOG("Hello From Attack State");
    Animate* animateResult;
    UserDefault::getInstance()->setIntegerForKey("slash_sound", Audio::getInstance()->play2d("sound/sounds effect/slash_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

    if (player->getCurrentDirection() == 1) {
        animateResult = player->armAttack_E;
    }
    else if (player->getCurrentDirection() == 2) {
        animateResult = player->armAttack_NE;
    }
    else if (player->getCurrentDirection() == 3) {
        animateResult = player->armAttack_N;
    }
    else if (player->getCurrentDirection() == 4) {
        animateResult = player->armAttack_NW;
    }
    else if (player->getCurrentDirection() == 5) {
        animateResult = player->armAttack_W;
    }
    else if (player->getCurrentDirection() == 6) {
        animateResult = player->armAttack_SW;
    }
    else if (player->getCurrentDirection() == 7) {
        animateResult = player->armAttack_S;
    }

    else if (player->getCurrentDirection() == 8) {
        animateResult = player->armAttack_SE;
    }
    lastDir = player->getCurrentDirection();
    player->getCurrentSprite()->runAction(cocos2d::Sequence::create(
        animateResult,
        cocos2d::CallFunc::create([this]() {
            //Running
            if (player->isMoving())player->SwitchState(player->walkState);
            //Idle
            if (!player->isMoving())player->SwitchState(player->idleState);
            }),
        nullptr
    ));

}

void PlayerAttack::ExitState() {
    if (player->getNumberOfRunningActions() > 0)player->getCurrentSprite()->stopAllActions();
}

void PlayerAttack::UpdateState() {


}

