#include "UpgradeInventoryButton.h"
#include "../../Inventory/UpgradeInventory.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
UpgradeInventoryButton* UpgradeInventoryButton::create(UpgradeInventory* inventory) {
    auto node = new UpgradeInventoryButton();
    if (node && node->init(inventory)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool UpgradeInventoryButton::init(UpgradeInventory* inventory) {
    if (!Node::init()) {
        return false;
    }

    inven = inventory;
    btnSprite = Sprite::create("res/Crafting-ICON.png");
    btnSprite->setPosition(this->getPosition());
    btnSprite->setScale(6);
    this->addChild(btnSprite);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(UpgradeInventoryButton::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btnSprite);


    return true;
}

bool UpgradeInventoryButton::onTouchBegan(Touch* touch, Event* event) {
    if (isVisible()) {
        if (btnSprite->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            if (inven->isShow())
                inven->hideUpgradeInventory();
            else
                inven->showUpgradeInventory();
            return true;
        }
        return false;
    }
    return false;
}
