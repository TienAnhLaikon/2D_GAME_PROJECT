#include "LoadingScene.h"
#include "Game/Game.h"

LoadingScene* createLoading() {
    auto node = new LoadingScene();
    if (node && node->init()) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool LoadingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("res/background-loading.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
    this->addChild(background);

    this->setContentSize(Size(background->getContentSize().width * (visibleSize.width / background->getContentSize().width), background->getContentSize().height * (visibleSize.height / background->getContentSize().height)));

    auto logo = Sprite::create("res/soul-slayer-icon.png");
    logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    logo->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
    this->addChild(logo);

    auto loadingBarBorder = Sprite::create("res/loadingBar.png");
    loadingBarBorder->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + loadingBarBorder->getContentSize().height * 2));
    loadingBarBorder->setScale(0.5);
    this->addChild(loadingBarBorder);

    loadingBarFill = ui::LoadingBar::create("res/loadingBarFill.png");
    loadingBarFill->setPosition(loadingBarBorder->getPosition());
    loadingBarFill->setDirection(ui::LoadingBar::Direction::LEFT);
    loadingBarFill->setPercent(perc);
    loadingBarFill->setScale(0.5);
    this->addChild(loadingBarFill);


    this->schedule(CC_SCHEDULE_SELECTOR(LoadingScene::updateLoading), 2.0f / 100.0f, 100, 0.0f);


    auto delay = DelayTime::create(2.0f);
    auto remove = CallFunc::create([this]() {
        auto game = dynamic_cast<Game*>(this->getParent());
        if (game) {
            game->loadingSceneFirst = nullptr;
            game->loadingScene = nullptr;
        }
        this->removeFromParentAndCleanup(true);
        isDeleted = true;
        });
    auto sequence = Sequence::create(
        delay,
        remove,
        nullptr
    );
    sequence->setTag(123);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(LoadingScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    this->runAction(sequence);


    return true;
}

void LoadingScene::updateLoading(float dt) {
    int currentPercent = loadingBarFill->getPercent();
    if (currentPercent < 100) {
        loadingBarFill->setPercent(currentPercent + 1);
    }
    else {
        this->unschedule(CC_SCHEDULE_SELECTOR(LoadingScene::updateLoading));
    }
}

void LoadingScene::unscheduleUpdate() {
    if (this->getActionByTag(123)) this->stopActionByTag(123);
    this->unschedule(CC_SCHEDULE_SELECTOR(LoadingScene::updateLoading));
}