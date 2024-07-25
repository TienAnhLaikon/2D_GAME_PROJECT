#include "TeleportEffect.h"
#include "Engine/Engine.h"

bool TeleportEffect::init() {
    if (!Sprite::init()) return false;
    Animation* animation = Engine::getInstance()->createAnimation2("tele_effect", 10, 0.05f);
    createWithSpriteFrameName("tele_effect (1).png");
    Animate* animate = Animate::create(animation);
    this->runAction(RepeatForever::create(animate));
    this->setScale(0.6);
    this->setOpacity(199);
    this->retain();
    return true;
}
