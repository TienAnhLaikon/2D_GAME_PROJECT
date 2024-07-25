#include "TalentProgressBar.h"


// Constructor
TalentProgressBar::TalentProgressBar() : talentProgressBar(nullptr) {}

// Destructor
TalentProgressBar::~TalentProgressBar() {}

// Kh?i t?o l?p
bool TalentProgressBar::init() {
    if (!Node::init()) {
        return false;
    }
    //
    auto size = Director::getInstance()->getVisibleSize();
    scale = size.width / size.height;
    auto emptyBar = Sprite::create("res/exp-border.png");
    emptyBar->setScale(1 / (scale * 2.1f), 1 / (scale * 4.0f));
    this->addChild(emptyBar, 0);

    talentProgressBar = ui::LoadingBar::create("res/exp-inline.png", 0);
    if (!talentProgressBar) {
        return false;
    }
    


    talentProgressBar->setDirection(ui::LoadingBar::Direction::LEFT);
    talentProgressBar->setScale(1 / (scale * 2.0f), 1 / (scale * 4.0f));
    talentProgressBar->setPosition(Vec2(0, 0));
    this->addChild(talentProgressBar, 1);

    //maxLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 8);
    //maxLabel->setTextColor(Color4B::GREEN);
    //maxLabel->setPosition(talentProgressBar->getContentSize().width * 1 / (scale * 2.0f) / 2 + maxLabel->getContentSize().width / 2 + 5, 0);
    //this->addChild(maxLabel);

    currentProgressSprite = Sprite::create("res/cc_small_window_bot_arrow.png");

    curLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 15);
    curLabel->setScale(0.3);
    curLabel->setTextColor(Color4B::GREEN);
    curLabel->setPosition(-(talentProgressBar->getContentSize().width * 1 / (scale * 2.0f) / 2) - curLabel->getContentSize().width / 2, curLabel->getContentSize().height / 2 + 1 * Director::getInstance()->getContentScaleFactor());
    this->addChild(curLabel);

    currentProgressSprite->setPosition(-(talentProgressBar->getContentSize().width * 1 / (scale * 2.0f) / 2) - curLabel->getContentSize().width / 2, curLabel->getContentSize().height / 2 - currentProgressSprite->getContentSize().height * 0.05 * Director::getInstance()->getContentScaleFactor() / 2);
    currentProgressSprite->setScale(0.05 * Director::getInstance()->getContentScaleFactor());
    this->addChild(currentProgressSprite);
    return true;
}

TalentProgressBar* TalentProgressBar::create() {
    TalentProgressBar* talentProgressBar = new TalentProgressBar();
    if (talentProgressBar && talentProgressBar->init()) {
        talentProgressBar->autorelease();
        return talentProgressBar;
    }
    CC_SAFE_DELETE(talentProgressBar);
    return nullptr;
}

void TalentProgressBar::updateProgress(float currentHealth, float maxHealth) {
    if (!talentProgressBar) return;
    float healthPercentage = currentHealth / maxHealth;
    talentProgressBar->setPercent(healthPercentage * 100);
    curLabel->setPositionX(-(talentProgressBar->getContentSize().width * 1 / (scale * 2.0f) / 2) + (talentProgressBar->getContentSize().width * 1 / (scale * 2.0f) * healthPercentage));
    currentProgressSprite->setPositionX(-(talentProgressBar->getContentSize().width * 1 / (scale * 2.0f) / 2) + (talentProgressBar->getContentSize().width * 1 / (scale * 2.0f) * healthPercentage));
    curLabel->setString(std::to_string(static_cast<int>(currentHealth)));
    //maxLabel->setString(std::to_string(static_cast<int>(maxHealth)));
}
