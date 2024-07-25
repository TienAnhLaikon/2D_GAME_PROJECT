#include "TalentCarousel.h"
#include "Player/Player.h"
//#include "Skills/SkillTree.h"
#include "Skills/SkillFactory.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
#include "TutorialManager/TutorialManager.h"
#include "Game/Game.h"
#include "TalentProgressBar.h"
#include "CoinManager/CoinManager.h"
//int TalentCarousel::currentTree = 0;

TalentCarousel* TalentCarousel::create(Player* player) {
    auto node = new TalentCarousel();
    if (node && node->init(player)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool TalentCarousel::init(Player* player) {
    if (!Node::init()) {
        return false;
    }
    _player = player;
    this->setVisible(false);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //CCLOG("%s asdasdsdsda sdsadsad", ->skillName.c_str());

     touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(TalentCarousel::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(TalentCarousel::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(TalentCarousel::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    talentBorder = Sprite::create("res/background-img-4.png");
    //talentBorder->setOpacity(160);
    this->setContentSize(talentBorder->getContentSize());
    this->addChild(talentBorder);

    fireTreeTabButton = Sprite::create("res/button_tab.png");
    auto fireTreeLabel = Label::createWithTTF("Fire Skills", "fonts/Diablo Heavy.ttf", 13);
    fireTreeLabel->setName("label");
    fireTreeLabel->setScale(0.68);
    fireTreeLabel->setPosition(Vec2(fireTreeTabButton->getContentSize().width / 2, fireTreeTabButton->getContentSize().height / 2));
    fireTreeTabButton->setScale(0.50);
    fireTreeTabButton->addChild(fireTreeLabel);
    fireTreeTabButton->setPosition(Vec2(-fireTreeTabButton->getContentSize().width / 4, talentBorder->getContentSize().height / 2 - fireTreeTabButton->getContentSize().height * fireTreeTabButton->getScaleY() * 1.2));
    fireTreeTabButton->setOpacity(180);
    this->addChild(fireTreeTabButton);

    thunderTreeTabButton = Sprite::create("res/button_tab.png");
    auto thunderTreeLabel = Label::createWithTTF("Thunder Skills", "fonts/Diablo Heavy.ttf", 13);
    thunderTreeLabel->setName("label");
    thunderTreeLabel->setScale(0.68);
    thunderTreeLabel->setPosition(Vec2(thunderTreeTabButton->getContentSize().width / 2, thunderTreeTabButton->getContentSize().height / 2));
    thunderTreeTabButton->setScale(0.50);
    thunderTreeTabButton->addChild(thunderTreeLabel);
    thunderTreeTabButton->setPosition(Vec2(thunderTreeTabButton->getContentSize().width / 4, talentBorder->getContentSize().height / 2 - thunderTreeTabButton->getContentSize().height * thunderTreeTabButton->getScaleY() * 1.2));
    thunderTreeTabButton->setOpacity(180);
    this->addChild(thunderTreeTabButton);

    buttonClose = Sprite::create("res/c_header_close.png");
    buttonClose->setScale(0.6 * Director::getInstance()->getContentScaleFactor());
    buttonClose->setPosition(Vec2((talentBorder->getContentSize().width / 2 - buttonClose->getContentSize().width * buttonClose->getScale() * 1.5), (talentBorder->getContentSize().height / 2 - buttonClose->getContentSize().height * buttonClose->getScale() * 1.5)));
    this->addChild(buttonClose, 6);

    //goldLabel = Label::createWithTTF("0", "fonts/Diablo Heavy.ttf", 13);
    //goldLabel->setString(std::to_string(CoinManager::getInstance()->getCoins()));
    //goldLabel->setPosition(Vec2((talentBorder->getContentSize().width / 2 - buttonClose->getContentSize().width * buttonClose->getScale() * 1.5) - 250, (talentBorder->getContentSize().height / 2 - buttonClose->getContentSize().height * buttonClose->getScale() * 1.5 -15)));
    //this->addChild(goldLabel, 6);

    //gold = Sprite::create("res/coin.png");
    //gold->setScale(0.1);
    //gold->setPosition(Vec2(0, goldLabel->getContentSize().height+ 10));
    //goldLabel->addChild(gold);

    //resetSkillButton = Sprite::create("res/reset_skills_button.png");
    //resetSkillButton->setScale(0.3 * Director::getInstance()->getContentScaleFactor());
    //resetSkillButton->setPosition(Vec2((talentBorder->getContentSize().width / 2 - buttonClose->getContentSize().width * buttonClose->getScale() * 1.46) +15, -(talentBorder->getContentSize().height / 2 - buttonClose->getContentSize().height * buttonClose->getScale() * 1.5)));
    //this->addChild(resetSkillButton, 6);

    skillTreeAoe = SkillTree::createSkillTree(player, SkillFactory::createSkill("FireBall")->getSkillTalent());
    skillTreeAoe->treeName = "Fire";
    skillTreeBuff = SkillTree::createSkillTree(player, SkillFactory::createSkill("ThunderCrackle")->getSkillTalent());
    skillTreeBuff->treeName = "Thunder";
    listOfTrees.push_back(skillTreeAoe);
    listOfTrees.push_back(skillTreeBuff);
    skillTreeAoe->setParentNode(this);
    skillTreeBuff->setParentNode(this);

    progressBar = TalentProgressBar::create();
    progressBar->setScale(Director::getInstance()->getContentScaleFactor());
    progressBar->setPosition(Vec2(0, -talentBorder->getContentSize().height / 2 + progressBar->getContentSize().height * progressBar->getScaleY() + 6 * Director::getInstance()->getContentScaleFactor()));
    this->addChild(progressBar, 3);
    progressBar->updateProgress(player->getExpToConsume(), player->getAllExp());
    
    auto overlay = LayerColor::create(Color4B(0, 0, 0, 128)); // Màu đen, 128 là độ trong suốt
    overlay->setPosition(Vec2(-overlay->getContentSize().width / 2, -overlay->getContentSize().height / 2));
    this->addChild(overlay, 2);
    overlay->setVisible(false);
    for (int i = 0; i < listOfTrees.size(); i++)
    {
        listOfTrees[i]->setScale(0.15);
        listOfTrees[i]->setPosition(Vec2(-talentBorder->getContentSize().width / 2 + 39 * Director::getInstance()->getContentScaleFactor(), 22 * Director::getInstance()->getContentScaleFactor()));
        listOfTrees[i]->setProgressBar(progressBar);
        this->addChild(listOfTrees[i], 4);
        listOfTrees[i]->setVisible(false);
    }


    fireTreeTabButton->setTexture("res/button_tab.png");
    thunderTreeTabButton->setTexture("res/button_tab_push.png");
    listOfTrees[0]->setVisible(true);
    listOfTrees[1]->setVisible(false);
    currentTree = 0;
    dynamic_cast<Label*>(thunderTreeTabButton->getChildByName("label"))->setColor(Color3B::BLACK);
    dynamic_cast<Label*>(fireTreeTabButton->getChildByName("label"))->setColor(Color3B::WHITE);

    /*auto touchListenerToResetSkill = EventListenerTouchOneByOne::create();
    touchListenerToResetSkill->setSwallowTouches(true);
    touchListenerToResetSkill->onTouchBegan = CC_CALLBACK_2(TalentCarousel::onTouchBeganToResetSkills, this);
    resetSkillButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListenerToResetSkill, resetSkillButton);*/

    return true;
}

bool TalentCarousel::onTouchBegan(Touch* touch, Event* event) {
    if (isVisible()) {
        if (fireTreeTabButton->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            skillTreeAoe->moneyLabel->setString(StringUtils::format("Skill Point: %d", _player->getSkillPoint()));
            skillTreeBuff->moneyLabel->setString(StringUtils::format("Skill Point: %d", _player->getSkillPoint()));
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            listOfTrees[0]->setVisible(true);
            listOfTrees[1]->setVisible(false);
            lastChoosedFirstTab = true;
            currentTree = 0;
            fireTreeTabButton->setTexture("res/button_tab_push.png");
            dynamic_cast<Label*>(fireTreeTabButton->getChildByName("label"))->setColor(Color3B::BLACK);
            thunderTreeTabButton->setTexture("res/button_tab.png");
            dynamic_cast<Label*>(thunderTreeTabButton->getChildByName("label"))->setColor(Color3B::WHITE);
        } 
        if (thunderTreeTabButton->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            skillTreeAoe->moneyLabel->setString(StringUtils::format("Skill Point: %d", _player->getSkillPoint()));
            skillTreeBuff->moneyLabel->setString(StringUtils::format("Skill Point: %d", _player->getSkillPoint()));
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            listOfTrees[0]->setVisible(false);
            listOfTrees[1]->setVisible(true);
            currentTree = 1;
            lastChoosedFirstTab = false;
            thunderTreeTabButton->setTexture("res/button_tab_push.png");
            dynamic_cast<Label*>(thunderTreeTabButton->getChildByName("label"))->setColor(Color3B::BLACK);
            fireTreeTabButton->setTexture("res/button_tab.png");
            dynamic_cast<Label*>(fireTreeTabButton->getChildByName("label"))->setColor(Color3B::WHITE);
        }
        auto startPoint = this->convertToNodeSpace(touch->getLocation());
        if (buttonClose->getBoundingBox().containsPoint(startPoint))
        {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

            resetUI();

            Scene* currentScene = Director::getInstance()->getRunningScene();
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            game->resumeGame();

            this->setVisible(false);
            InGameUI::getInstance(_player)->showUI();
            listOfTrees[currentTree]->setVisible(false);

            int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
            if (currentTutorial >= 5 && currentTutorial < 8) {
                TutorialManager::getInstance()->showTutorial(4);
            }


            if (currentTutorial == 8) {
                TutorialManager::getInstance()->nextTutorial();
            }
            /*if (listOfTrees[currentTree]->skillDetail->getChildByName("Slot1")) {
                listOfTrees[currentTree]->_touchListenerSlot1->setEnabled(false);
                listOfTrees[currentTree]->_touchListenerSlot2->setEnabled(false);
                listOfTrees[currentTree]->_touchListenerSlot3->setEnabled(false);
                listOfTrees[currentTree]->_touchListenerSlot4->setEnabled(false);
            }*/
        }
        return true;
    }
    return false;
}

void TalentCarousel::resetUI() {
    currentTree == 1 ? listOfTrees[1]->setVisible(true) : listOfTrees[1]->setVisible(false);
    currentTree == 0 ? listOfTrees[0]->setVisible(true) : listOfTrees[0]->setVisible(false);
    currentTree == 1 == true ? thunderTreeTabButton->setTexture("res/button_tab_push.png") : thunderTreeTabButton->setTexture("res/button_tab.png");
    currentTree == 0 ? fireTreeTabButton->setTexture("res/button_tab_push.png") : fireTreeTabButton->setTexture("res/button_tab.png");
    currentTree == 1 == true ? dynamic_cast<Label*>(thunderTreeTabButton->getChildByName("label"))->setColor(Color3B::BLACK) : dynamic_cast<Label*>(thunderTreeTabButton->getChildByName("label"))->setColor(Color3B::WHITE);
    currentTree == 0 ? dynamic_cast<Label*>(fireTreeTabButton->getChildByName("label"))->setColor(Color3B::BLACK) : dynamic_cast<Label*>(fireTreeTabButton->getChildByName("label"))->setColor(Color3B::WHITE);
}

bool TalentCarousel::onTouchMoved(Touch* touch, Event* event)
{
    return false;
}

bool TalentCarousel::onTouchEnded(Touch* touch, Event* event)
{
    return false;
}

void TalentCarousel::showTalentCarousel() {
    this->setVisible(true);
    InGameUI::getInstance(_player)->hideUI();
    resetUI();
   // listOfTrees[currentTree]->setVisible(true);
    progressBar->updateProgress(_player->getExpToConsume(), _player->getAllExp());
}

void TalentCarousel::hideComponents() {
    fireTreeTabButton->setVisible(false);
    thunderTreeTabButton->setVisible(false);
    buttonClose->setVisible(false);
    talentBorder->setVisible(false);
    progressBar->setVisible(false);
}

void TalentCarousel::showComponents() {
    fireTreeTabButton->setVisible(true);
    fireTreeTabButton->setTexture("res/button_tab_push.png");
    thunderTreeTabButton->setTexture("res/button_tab.png");
    dynamic_cast<Label*>(fireTreeTabButton->getChildByName("label"))->setColor(Color3B::BLACK);
    dynamic_cast<Label*>(thunderTreeTabButton->getChildByName("label"))->setColor(Color3B::WHITE);
    thunderTreeTabButton->setVisible(true);
    buttonClose->setVisible(true);
    talentBorder->setVisible(true);
    progressBar->setVisible(true);
}

void TalentCarousel::resetSkill(Player* player) {
    
}

//bool TalentCarousel::onTouchBeganToResetSkills(Touch* touch, Event* event) {
//    if (isVisible()) {
//        auto target = static_cast<Sprite*>(event->getCurrentTarget());
//        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
//        Size s = target->getContentSize();
//        Rect rect = Rect(0, 0, s.width, s.height);
//
//        if (rect.containsPoint(locationInNode)) {
///*            bool result = */yesOrNo(CoinManager::getInstance());
//            //if (result) {
//            //    resetSkill(_player);
//            //    NotificationManager::getInstance()->showMessageNotification("Reset skills Completed!", Vec2::ZERO, Color3B::GREEN, 10);
//            //}
//            //else NotificationManager::getInstance()->showMessageNotification("Reset skills Failed!", Vec2::ZERO, Color3B::RED, 10);
//            return true;
//        }
//    }
//    return false;
//}

//bool TalentCarousel::yesOrNo(CoinManager* coinManager) {
//    auto goldManager = CoinManager::getInstance();
//    Player* player = _player;
//    auto spriteBg = Sprite::create("res/papper_container2.png");
//    spriteBg->setName("YesOrNo");
//    this->addChild(spriteBg, 10);
//    /*spriteBg->setPosition(Vec2((talentBorder->getContentSize().width / 2), (talentBorder->getContentSize().height / 2 )));*/
//    spriteBg->setScale(0.4);
//    auto askLabel = Label::createWithTTF("Do you want to Reset All Skills ?", "fonts/Diablo Heavy.ttf", 13);
//    askLabel->setPosition(Vec2(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height / 2 + 50));
//    spriteBg->addChild(askLabel);
//
//    auto goldX = Sprite::create("res/coin.png");
//    goldX->setScale(0.3);
//    goldX->setPosition(Vec2(askLabel->getContentSize().width - 15, askLabel->getContentSize().height));
//    askLabel->addChild(goldX);
//
//    auto yesButton = Sprite::create("res/quest_list_row_checkbox.png");
//    yesButton->setScale(2);
//    yesButton->setPosition(Vec2(spriteBg->getContentSize().width/2 - 40, spriteBg->getContentSize().height/2));
//    spriteBg->addChild(yesButton);
//
//    auto yesLabel = Label::createWithTTF("Yes", "fonts/Diablo Heavy.ttf", 10);
//    yesLabel->setPosition(Vec2(yesButton->getContentSize().width / 2, yesButton->getContentSize().height / 2));
//    yesButton->addChild(yesLabel);
//
//    auto noButton = Sprite::create("res/quest_list_row_checkbox.png");
//    noButton->setScale(2);
//    noButton->setPosition(Vec2(spriteBg->getContentSize().width / 2 + 40, spriteBg->getContentSize().height / 2));
//    spriteBg->addChild(noButton);
//
//    auto noLabel = Label::createWithTTF("No", "fonts/Diablo Heavy.ttf", 10);
//    noLabel->setPosition(Vec2(noButton->getContentSize().width / 2, noButton->getContentSize().height / 2));
//    noButton->addChild(noLabel);
//
//    auto closeButton = Sprite::create("res/close_icon.png");
//    closeButton->setScale(2);
//    closeButton->setPosition(Vec2(spriteBg->getContentSize().width - 10, spriteBg->getContentSize().height -10 ));
//    spriteBg->addChild(closeButton);
//
//    // Thêm sự kiện nhấn nút Yes
//    auto yesListener = EventListenerTouchOneByOne::create();
//    yesListener->setSwallowTouches(true);
//    yesListener->onTouchBegan = [this, yesButton, spriteBg, player, coinManager](Touch* touch, Event* event) {
//        auto target = static_cast<Sprite*>(event->getCurrentTarget());
//        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
//        Size s = target->getContentSize();
//        Rect rect = Rect(0, 0, s.width, s.height);
//
//        if (rect.containsPoint(locationInNode)) {
//            /*if (CoinManager::getInstance()->getCoins() >= 1200) {*/
//                this->getChildByName("YesOrNo")->removeFromParent();
//                int totalExp = 0;
//                for (auto skill : player->getPlayerSkills()) {
//                    int totalPoint = (skill->getCurrentLevelSkill() - 1) * skill->getSkillTalent()->upgradePoint + skill->getSkillTalent()->unlockPoint;
//                    totalExp += totalPoint;
//                }
//                player->setExpToConsume(player->getExpToConsume() + totalExp);
//                this->skillTreeAoe->_progressBar->updateProgress(player->getExpToConsume(), player->getAllExp());
//                this->skillTreeBuff->_progressBar->updateProgress(player->getExpToConsume(), player->getAllExp());
//                for (auto skill : player->getPlayerSkills()) {
//                    if (skill->getSkillIndex() != 5)
//                        skill->setSkillIndex(0);
//                }
//                player->resetSkillSlotOrder();
//
//                if (this->skillTreeAoe->getChildByName("FireBall Node"))this->skillTreeAoe->getChildByName("FireBall Node")->setVisible(false);
//                if (this->skillTreeAoe->getChildByName("FireBall Node"))this->skillTreeAoe->getChildByName("FireBall Node")->setName("false");
//                if (this->skillTreeAoe->getChildByName("FireCastA Node"))this->skillTreeAoe->getChildByName("FireCastA Node")->setVisible(false);
//                if (this->skillTreeAoe->getChildByName("FireCastA Node"))this->skillTreeAoe->getChildByName("FireCastA Node")->setName("false");
//                if (this->skillTreeAoe->getChildByName("FireCastB Node"))this->skillTreeAoe->getChildByName("FireCastB Node")->setVisible(false);
//                if (this->skillTreeAoe->getChildByName("FireCastB Node"))this->skillTreeAoe->getChildByName("FireCastB Node")->setName("false");
//                if (this->skillTreeAoe->getChildByName("FireCombust Node"))this->skillTreeAoe->getChildByName("FireCombust Node")->setVisible(false);
//                if (this->skillTreeAoe->getChildByName("FireCombust Node"))this->skillTreeAoe->getChildByName("FireCombust Node")->setName("false");
//                if (this->skillTreeAoe->getChildByName("FireConflagration Node"))this->skillTreeAoe->getChildByName("FireConflagration Node")->setVisible(false);
//                if (this->skillTreeAoe->getChildByName("FireConflagration Node"))this->skillTreeAoe->getChildByName("FireConflagration Node")->setName("false");
//                if (this->skillTreeAoe->getChildByName("FireFlare Node"))this->skillTreeAoe->getChildByName("FireFlare Node")->setVisible(false);
//                if (this->skillTreeAoe->getChildByName("FireFlare Node"))this->skillTreeAoe->getChildByName("FireFlare Node")->setName("false");
//                if (this->skillTreeAoe->getChildByName("FireScorch Node"))this->skillTreeAoe->getChildByName("FireScorch Node")->setVisible(false);
//                if (this->skillTreeAoe->getChildByName("FireScorch Node"))this->skillTreeAoe->getChildByName("FireScorch Node")->setName("false");
//
//
//                //this->skillTreeBuff->removeAllChildren();
//                if (this->skillTreeBuff->getChildByName("ThunderCrackle Node"))this->skillTreeBuff->getChildByName("ThunderCrackle Node")->setVisible(false);
//                if (this->skillTreeBuff->getChildByName("ThunderCrackle Node"))this->skillTreeBuff->getChildByName("ThunderCrackle Node")->setName("false");
//                if (this->skillTreeBuff->getChildByName("ThunderCastA Node"))this->skillTreeBuff->getChildByName("ThunderCastA Node")->setVisible(false);
//                if (this->skillTreeBuff->getChildByName("ThunderCastA Node"))this->skillTreeBuff->getChildByName("ThunderCastA Node")->setName("false");
//                if (this->skillTreeBuff->getChildByName("ThunderCastB Node"))this->skillTreeBuff->getChildByName("ThunderCastB Node")->setVisible(false);
//                if (this->skillTreeBuff->getChildByName("ThunderCastB Node"))this->skillTreeBuff->getChildByName("ThunderCastB Node")->setName("false");
//                if (this->skillTreeBuff->getChildByName("ThunderShock Node"))this->skillTreeBuff->getChildByName("ThunderShock Node")->setVisible(false);
//                if (this->skillTreeBuff->getChildByName("ThunderShock Node"))this->skillTreeBuff->getChildByName("ThunderShock Node")->setName("false");
//                if (this->skillTreeBuff->getChildByName("ThunderCurrent Node"))this->skillTreeBuff->getChildByName("ThunderCurrent Node")->setVisible(false);
//                if (this->skillTreeBuff->getChildByName("ThunderCurrent Node"))this->skillTreeBuff->getChildByName("ThunderCurrent Node")->setName("false");
//                if (this->skillTreeBuff->getChildByName("ThunderDynamo Node"))this->skillTreeBuff->getChildByName("ThunderDynamo Node")->setVisible(false);
//                if (this->skillTreeBuff->getChildByName("ThunderDynamo Node"))this->skillTreeBuff->getChildByName("ThunderDynamo Node")->setName("false");
//                if (this->skillTreeBuff->getChildByName("ThunderVoltage Node"))this->skillTreeBuff->getChildByName("ThunderVoltage Node")->setVisible(false);
//                if (this->skillTreeBuff->getChildByName("ThunderVoltage Node"))this->skillTreeBuff->getChildByName("ThunderVoltage Node")->setName("false");
//                if (this->skillTreeBuff->getChildByName("ThunderBolt Node"))this->skillTreeBuff->getChildByName("ThunderBolt Node")->setVisible(false);
//                if (this->skillTreeBuff->getChildByName("ThunderBolt Node"))this->skillTreeBuff->getChildByName("ThunderBolt Node")->setName("false");
//
//                if (this->skillTreeBuff->getChildByName("SkillDetail"))
//                {
//                    auto child = this->skillTreeBuff->getChildByName("SkillDetail");
//                    child->removeAllChildren();
//                    child->removeFromParent();
//                }
//                if (this->skillTreeAoe->getChildByName("SkillDetail"))
//                {
//                    auto child = this->skillTreeAoe->getChildByName("SkillDetail");
//                    child->removeAllChildren();
//                    child->removeFromParent();
//                }
//                ////Clear skills
//                player->skills.clear();
//                skillTreeAoe->drawTree(player, SkillFactory::createSkill("FireBall")->getSkillTalent(), Vec2(0, 0), 200, 200, nullptr);
//                skillTreeBuff->drawTree(player, SkillFactory::createSkill("ThunderCrackle")->getSkillTalent(), Vec2(0, 0), 200, 200, nullptr);
//
//                if (totalExp != 0) {
//                    NotificationManager::getInstance()->showMessageNotification("Reset Skills Completed!", Vec2::ZERO, Color3B(161, 251, 142), 17);
//                    /*coinManager->decreaseCoin(1200);
//                    this->gold->setPosition(Vec2(0, goldLabel->getContentSize().height + 10));
//                    this->goldLabel->setString(std::to_string(coinManager->getCoins()));*/
//                }
//                else NotificationManager::getInstance()->showMessageNotification("Skills got reset already!!", Vec2::ZERO, Color3B(251, 67, 53), 17);
//            /*}
//            else NotificationManager::getInstance()->showMessageNotification("Not enough Coin to Reset", Vec2::ZERO, Color3B(251, 67, 53), 17);*/
//            return true;
//        }
//        return false;
//        };
//    yesButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(yesListener, yesButton);
//
//    // Thêm sự kiện nhấn nút No
//    auto noListener = EventListenerTouchOneByOne::create();
//    noListener->setSwallowTouches(true);
//    noListener->onTouchBegan = [this, noButton, spriteBg](Touch* touch, Event* event) {
//        auto target = static_cast<Sprite*>(event->getCurrentTarget());
//        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
//        Size s = target->getContentSize();
//        Rect rect = Rect(0, 0, s.width, s.height);
//
//        if (rect.containsPoint(locationInNode)) {
//            this->getChildByName("YesOrNo")->removeFromParent();
//            return true;
//        }
//        return false;
//        };
//    noButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(noListener, noButton);
//
//    // Thêm sự kiện nhấn nút X
//    auto closeListener = EventListenerTouchOneByOne::create();
//    closeListener->setSwallowTouches(true);
//    closeListener->onTouchBegan = [this, closeButton, spriteBg](Touch* touch, Event* event) {
//        auto target = static_cast<Sprite*>(event->getCurrentTarget());
//        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
//        Size s = target->getContentSize();
//        Rect rect = Rect(0, 0, s.width, s.height);
//
//        if (rect.containsPoint(locationInNode)) {
//            this->getChildByName("YesOrNo")->removeFromParent();
//            return true;
//        }
//        return false;
//        };
//    closeButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(closeListener, closeButton);
//    //// Sử dụng std::future để đợi lựa chọn của người chơi
//    //std::future<bool> future = promise.get_future();
//    //return future.get(); // Đợi cho đến khi người chơi chọn Yes hoặc No
//    return true;
//}
//
//void TalentCarousel::onYesClicked() {
//    // Xử lý khi người chơi nhấn Yes
//    CCLOG("Yes clicked!");
//    promise.set_value(true); // Đặt giá trị trả về là true
//}
//
//void TalentCarousel::onNoClicked() {
//    // Xử lý khi người chơi nhấn No
//    CCLOG("No clicked!");
//    promise.set_value(false); // Đặt giá trị trả về là false
//}
//
//void TalentCarousel::onCloseClicked() {
//    // Xử lý khi người chơi nhấn No
//    CCLOG("Close clicked!");
//    promise.set_value(false); // Đặt giá trị trả về là false
//}