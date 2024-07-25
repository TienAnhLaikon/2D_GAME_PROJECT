#include "ExpBar.h"


// Constructor
ExpBar::ExpBar() : expBar(nullptr) {}

// Destructor
ExpBar::~ExpBar() {}

// Kh?i t?o l?p
bool ExpBar::init() {
    if (!Node::init()) {
        return false;
    }

    // T?o thanh mu
    expBar = ui::LoadingBar::create("res/exp-inline-lighter.png", 0);
    if (!expBar) {
        return false;
    }
    
    expBar->setDirection(ui::LoadingBar::Direction::LEFT); 
    auto size = Director::getInstance()->getVisibleSize();
    this->setContentSize(Size(size.width, expBar->getContentSize().height * 0.2));
    float scale = size.width / size.height * 2;
    expBar->setScale(expBar->getContentSize().width / size.width, 0.2);
    this->addChild(expBar, 0);
    return true;
}

ExpBar* ExpBar::create() {
    ExpBar* expBar = new ExpBar();
    if (expBar && expBar->init()) {
        expBar->autorelease();
        return expBar;
    }
    CC_SAFE_DELETE(expBar);
    return nullptr;
}

void ExpBar::updateExpBar(float currentHealth, float maxHealth) {
    if (!expBar) return;
    float healthPercentage = currentHealth / maxHealth;
    expBar->setPercent(healthPercentage * 100);
}

