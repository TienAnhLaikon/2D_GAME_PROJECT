#include "UpgradeProgressBar.h"


// Constructor
UpgradeProgressBar::UpgradeProgressBar() : upgradeProgressBar(nullptr) {}

// Destructor
UpgradeProgressBar::~UpgradeProgressBar() {}

// Kh?i t?o l?p
bool UpgradeProgressBar::init() {
    if (!Node::init()) {
        return false;
    }
    //
    auto size = Director::getInstance()->getVisibleSize();
    float scale = size.width / size.height;
    auto emptyBar = Sprite::create("res/empty_bar.png");
    emptyBar->setScale(1 / (scale * 2.1f), 1 / (scale * 4.0f));
    this->addChild(emptyBar, 0);

    upgradeProgressBar = ui::LoadingBar::create("res/fill.png", 0);
    if (!upgradeProgressBar) {
        return false;
    }
    
    upgradeProgressBar->setDirection(ui::LoadingBar::Direction::LEFT);
    upgradeProgressBar->setScale(1 / (scale * 2.0f), 1 / (scale * 4.0f));
    upgradeProgressBar->setPosition(Vec2(0, 0));
    this->addChild(upgradeProgressBar, 1);


    return true;
}

UpgradeProgressBar* UpgradeProgressBar::create() {
    UpgradeProgressBar* upgradeProgressBar = new UpgradeProgressBar();
    if (upgradeProgressBar && upgradeProgressBar->init()) {
        upgradeProgressBar->autorelease();
        return upgradeProgressBar;
    }
    CC_SAFE_DELETE(upgradeProgressBar);
    return nullptr;
}

void UpgradeProgressBar::updateProgress(float currentHealth, float maxHealth) {
    if (!upgradeProgressBar) return;
    float healthPercentage = currentHealth / maxHealth;
    upgradeProgressBar->setPercent(healthPercentage * 100);
}
