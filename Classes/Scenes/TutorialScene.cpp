#include "TutorialScene.h"
#include "Scenes/ScenesManager.h"
#include "UserInterface/InGame/Scrollview.h"

Scene* TutorialScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TutorialScene::create();
    scene->addChild(layer);
    return scene;
}

bool TutorialScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("res/black-background.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
    this->addChild(background);

    auto backToMainMenuButton = MenuItemImage::create(
        "res/cc_char_options_arrow_left.png",
        "res/cc_char_options_arrow_left-HOVER.png",
        CC_CALLBACK_1(TutorialScene::backToMainMenuCallback, this));
        backToMainMenuButton->setScale(0.8 * Director::getInstance()->getContentScaleFactor());
        backToMainMenuButton->setPosition(Vec2(75, visibleSize.height-75));


    auto backToMenu = Menu::create(backToMainMenuButton, nullptr);
    backToMenu->setPosition(Vec2::ZERO);
    this->addChild(backToMenu, 2);

    auto scrollview = Scrollview::create();
    scrollview->setScrollViewContentSize(Size(visibleSize.width, visibleSize.height));
    scrollview->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    auto nodesToAdd = std::vector<Node*>();
    Node* tutorial1 = Node::create();
    Label* lb = Label::createWithTTF("tutorial1", "fonts/arial.ttf", 20);
    tutorial1->addChild(lb, 10);
    Sprite* tutorial1Sprite = Sprite::create("res/Tutorial Normal Attack, Use Skills, Use Items Equipment.png");
    tutorial1Sprite->setScale((visibleSize.width) / tutorial1Sprite->getContentSize().width, (visibleSize.height) / tutorial1Sprite->getContentSize().height);
    tutorial1->setContentSize(Size((visibleSize.width) / tutorial1Sprite->getContentSize().width * tutorial1Sprite->getContentSize().width, (visibleSize.height) / tutorial1Sprite->getContentSize().height * tutorial1Sprite->getContentSize().height));
    tutorial1->addChild(tutorial1Sprite);
    nodesToAdd.push_back(tutorial1);

    Node* tutorial2 = Node::create();
    Sprite* tutorial2Sprite = Sprite::create("res/Tutorial receive quest.png");
    tutorial2Sprite->setScale((visibleSize.width) / tutorial2Sprite->getContentSize().width, (visibleSize.height) / tutorial2Sprite->getContentSize().height);
    tutorial2->setContentSize(Size((visibleSize.width) / tutorial2Sprite->getContentSize().width * tutorial2Sprite->getContentSize().width, (visibleSize.height) / tutorial2Sprite->getContentSize().height * tutorial2Sprite->getContentSize().height));
    tutorial2->addChild(tutorial2Sprite);
    nodesToAdd.push_back(tutorial2);

    Node* tutorial3 = Node::create();
    Sprite* tutorial3Sprite = Sprite::create("res/Tutorial return quest to npc.png");
    tutorial3Sprite->setScale((visibleSize.width) / tutorial3Sprite->getContentSize().width, (visibleSize.height) / tutorial3Sprite->getContentSize().height);
    tutorial3->setContentSize(Size((visibleSize.width) / tutorial3Sprite->getContentSize().width * tutorial3Sprite->getContentSize().width, (visibleSize.height) / tutorial3Sprite->getContentSize().height * tutorial3Sprite->getContentSize().height));
    tutorial3->addChild(tutorial3Sprite);
    nodesToAdd.push_back(tutorial3);

    Node* tutorial4 = Node::create();
    Sprite* tutorial4Sprite = Sprite::create("res/Tutorial Unlock Skills.png");
    tutorial4Sprite->setScale((visibleSize.width) / tutorial4Sprite->getContentSize().width, (visibleSize.height) / tutorial4Sprite->getContentSize().height);
    tutorial4->setContentSize(Size((visibleSize.width) / tutorial4Sprite->getContentSize().width * tutorial4Sprite->getContentSize().width, (visibleSize.height) / tutorial4Sprite->getContentSize().height * tutorial4Sprite->getContentSize().height));
    tutorial4->addChild(tutorial4Sprite);
    nodesToAdd.push_back(tutorial4);

    Node* tutorial5 = Node::create();
    Sprite* tutorial5Sprite = Sprite::create("res/Tutorial Equip, Upgrade, Equip Slot Skills.png");
    tutorial5Sprite->setScale((visibleSize.width) / tutorial5Sprite->getContentSize().width, (visibleSize.height) / tutorial5Sprite->getContentSize().height);
    tutorial5->setContentSize(Size((visibleSize.width) / tutorial5Sprite->getContentSize().width * tutorial5Sprite->getContentSize().width, (visibleSize.height) / tutorial5Sprite->getContentSize().height * tutorial5Sprite->getContentSize().height));
    tutorial5->addChild(tutorial5Sprite);
    nodesToAdd.push_back(tutorial5);

    Node* tutorial6 = Node::create();
    Sprite* tutorial6Sprite = Sprite::create("res/Tutorial Equip,Drop Armor.png");
    tutorial6Sprite->setScale((visibleSize.width) / tutorial6Sprite->getContentSize().width, (visibleSize.height) / tutorial6Sprite->getContentSize().height);
    tutorial6->setContentSize(Size((visibleSize.width) / tutorial6Sprite->getContentSize().width * tutorial6Sprite->getContentSize().width, (visibleSize.height) / tutorial6Sprite->getContentSize().height * tutorial6Sprite->getContentSize().height));
    tutorial6->addChild(tutorial6Sprite);
    nodesToAdd.push_back(tutorial6);

    Node* tutorial7 = Node::create();
    Sprite* tutorial7Sprite = Sprite::create("res/Tutorial Unequip Weapon.png");
    tutorial7Sprite->setScale((visibleSize.width) / tutorial7Sprite->getContentSize().width, (visibleSize.height) / tutorial7Sprite->getContentSize().height);
    tutorial7->setContentSize(Size((visibleSize.width) / tutorial7Sprite->getContentSize().width * tutorial7Sprite->getContentSize().width, (visibleSize.height) / tutorial7Sprite->getContentSize().height * tutorial7Sprite->getContentSize().height));
    tutorial7->addChild(tutorial7Sprite);
    nodesToAdd.push_back(tutorial7);

    Node* tutorial8 = Node::create();
    Sprite* tutorial8Sprite = Sprite::create("res/Tutorial Use Item.png");
    tutorial8Sprite->setScale((visibleSize.width) / tutorial8Sprite->getContentSize().width, (visibleSize.height) / tutorial8Sprite->getContentSize().height);
    tutorial8->setContentSize(Size((visibleSize.width) / tutorial8Sprite->getContentSize().width * tutorial8Sprite->getContentSize().width, (visibleSize.height) / tutorial8Sprite->getContentSize().height * tutorial8Sprite->getContentSize().height));
    tutorial8->addChild(tutorial8Sprite);
    nodesToAdd.push_back(tutorial8);

    Node* tutorial9 = Node::create();
    Sprite* tutorial9Sprite = Sprite::create("res/Tutorial Upgrade Weapon.png");
    tutorial9Sprite->setScale((visibleSize.width) / tutorial9Sprite->getContentSize().width, (visibleSize.height) / tutorial9Sprite->getContentSize().height);
    tutorial9->setContentSize(Size((visibleSize.width) / tutorial9Sprite->getContentSize().width * tutorial9Sprite->getContentSize().width, (visibleSize.height) / tutorial9Sprite->getContentSize().height * tutorial9Sprite->getContentSize().height));
    tutorial9->addChild(tutorial9Sprite);
    nodesToAdd.push_back(tutorial9);



    float totalHeight = 0;
    for (auto node : nodesToAdd) {
        totalHeight += node->getContentSize().height;
    }
    scrollview->getScrollView()->setInnerContainerSize(Size(scrollview->getScrollView()->getContentSize().width, totalHeight));

    float posY = totalHeight;
    for (auto node : nodesToAdd) {
        node->setPosition(Vec2(node->getContentSize().width / 2, posY - node->getContentSize().height / 2));
        scrollview->addToScrollView(node);
        posY -= node->getContentSize().height;
    }


    this->addChild(scrollview, 1);

    return true;
}

void TutorialScene::backToMainMenuCallback(Ref* pSender)
{
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    SceneManager::getInstance()->runMainMenuScene();
}
