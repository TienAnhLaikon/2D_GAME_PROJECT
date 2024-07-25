#ifndef __NORMAL_MONSTER_H__
#define __NORMAL_MONSTER_H__

#include "cocos2d.h"
#include "Enemies/Enemy.h"
#include "Player/Player.h"
#include "State/NormalMonsterState/NormalMonsterStateBase.h"
#include "State/NormalMonsterState/NormalMonsterAttack.h"
#include "State/NormalMonsterState/NormalMonsterDead.h"
#include "State/NormalMonsterState/NormalMonsterIdle.h"
#include "State/NormalMonsterState/NormalMonsterWalk.h"

USING_NS_CC;
class HelloWorld;
class Game;
class NormalMonster : public Enemy
{
public:
    int player_hit_soundId = -1;
    std::string getEnemyName() const { return enemyName; };
    bool isCanMove;
    HelloWorld* helloWorld;
    Game* game;
    NormalMonsterStateBase *currentState;
    NormalMonsterStateBase *attackState;
    NormalMonsterStateBase *walkState;
    NormalMonsterStateBase *idleState;
    NormalMonsterStateBase *deadState;
    // default Attack
    Animate *defaultAttack_E;  // 1
    Animate *defaultAttack_NE; // 3
    Animate *defaultAttack_N;  // 5
    Animate *defaultAttack_NW; // 7
    Animate *defaultAttack_W;  // 9
    Animate *defaultAttack_SW; // 11
    Animate *defaultAttack_S;  // 13
    Animate *defaultAttack_SE; // 15

    // Default Walk
    Animate *defaultWalk_E;  // 1
    Animate *defaultWalk_NE; // 3
    Animate *defaultWalk_N;  // 5
    Animate *defaultWalk_NW; // 7
    Animate *defaultWalk_W;  // 9
    Animate *defaultWalk_SW; // 11
    Animate *defaultWalk_S;  // 13
    Animate *defaultWalk_SE; // 15

    // Special Dead
    Animate *specialDeath_E;  // 1
    Animate *specialDeath_NE; // 3
    Animate *specialDeath_N;  // 5
    Animate *specialDeath_NW; // 7
    Animate *specialDeath_W;  // 9
    Animate *specialDeath_SW; // 11
    Animate *specialDeath_S;  // 13
    Animate *specialDeath_SE; // 15

    // Default Idle
    Animate *defaultIdle_E;  // 1
    Animate *defaultIdle_NE; // 3
    Animate *defaultIdle_N;  // 5
    Animate *defaultIdle_NW; // 7
    Animate *defaultIdle_W;  // 9
    Animate *defaultIdle_SW; // 11
    Animate *defaultIdle_S;  // 13
    Animate *defaultIdle_SE; // 15

    virtual bool init();

    void Initalize(NormalMonsterStateBase *startingState)
    {
        currentState = startingState;
        currentState->EnterState();
    }

    void SwitchState(NormalMonsterStateBase *newState)
    {
        /*if (currentState == newState) {
            CCLOG("Not change to new State!");
            return;
        }*/
        currentState->ExitState();
        currentState = newState;
        currentState->EnterState();
    }
    bool isTargetInAttackRange;

    void detectPlayer();

    void chase();

    void stopSprite();

    void stopChase();

    float calculateAngle(const Vec2 &vectorA, const Vec2 &vectorB);

    void attack();

    void die();

    void updateEnemy(float dt);

    void takeDamage(float amountDamage) {
        float damageActuallyTake = amountDamage * (100 / (100 + this->baseArmor));
        if (this->currentHP <= damageActuallyTake) {
            currentHP = 0;
        }
        else {
            currentHP -= damageActuallyTake;
        }
        if (player_hit_soundId == -1 || AudioEngine::getState(player_hit_soundId) != AudioEngine::AudioState::PLAYING) {
            player_hit_soundId = Audio::getInstance()->play2d("sound/sounds effect/player_hit_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f);
            UserDefault::getInstance()->setIntegerForKey("sound_effect", player_hit_soundId);

            // Đặt callback khi âm thanh kết thúc để reset ID
            AudioEngine::setFinishCallback(player_hit_soundId, [this](int id, const std::string& filePath) {
                if (player_hit_soundId == id) {
                    player_hit_soundId = -1; // Đặt lại ID khi âm thanh kết thúc
                }
                });
        }
        healthBar->setPercent((currentHP / HP) * 100);
        //Lable
        auto dmgTakeLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 10);
        dmgTakeLable->setColor(Color3B::RED);
        this->addChild(dmgTakeLable);
        int dmgToInt = std::floor(damageActuallyTake);
        dmgTakeLable->setString(StringUtils::format("-%d", dmgToInt));
        Vec2 pos = Vec2(0, characterCurrentSprite->getBoundingBox().size.height / 2);
        auto moveTo = MoveTo::create(1.0f, pos);
        auto fadeOut = FadeOut::create(1.0f);
        auto spawn = Spawn::create(fadeOut, moveTo, nullptr);
        auto sqe = Sequence::create(spawn, RemoveSelf::create(), nullptr);
        dmgTakeLable->runAction(sqe);
    }

    void setTarget(Player *newTarget)
    {
        target = newTarget;
    }

    Player *getCurrentTarget()
    {
        return this->target;
    }


    CREATE_FUNC(NormalMonster);

protected:
    float detectionRadius;
    bool rayCast(const Vec2& start, const Vec2& end);
    Player *target;
    std::string enemyName;
};

#endif
