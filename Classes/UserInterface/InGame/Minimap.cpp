#include "Minimap.h"
#include "Player/Player.h"
#include "TutorialManager/TutorialManager.h"

Compass* Compass::create() {
    auto layer = new Compass();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool Compass::init() {
    if (!Node::init()) {
        return false;
    }
    compassNeedle = Sprite::create("res/compass.png");
    compassNeedle->setScale(0.7);
    this->addChild(compassNeedle, 2);

    compassBorder = Sprite::create("res/minimap_frame.png");
    this->addChild(compassBorder, 1);
    compassBorder->setVisible(false);
    return true;
}

void Compass::setTarget(Vec2 playerPosition, Vec2 targetPosition) {
    if (targetPosition != Vec2::ZERO && UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) >= 9) {
        if (targetPosition.distance(playerPosition) < 50) {
            if (compassNeedle->isVisible()) {
                compassNeedle->setVisible(false);
                int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
                if (currentTutorial == 9) {
                    TutorialManager::getInstance()->nextTutorial();
                }
            }
        } else if (compassNeedle->isVisible() == false) {
            compassNeedle->setVisible(true);
            int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
            if (currentTutorial == 10) {
                TutorialManager::getInstance()->prevTutorial();
            }
        }
        _target = targetPosition - playerPosition;

        float angleRadians = atan2(_target.y, _target.x);
        float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians); 
        compassNeedle->setRotation(angleDegrees + 90);
    }
    else {
        if (compassNeedle->isVisible()) {
            compassNeedle->setVisible(false);
        }
    }
}

Compass::~Compass() {
  
}