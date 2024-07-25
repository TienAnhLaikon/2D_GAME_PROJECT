#include "QuestManager.h"
#include "NPC/NPC.h"
#include "Game/Game.h"
#include "Map/Map.h"

QuestManager* QuestManager::s_instance = nullptr;

bool QuestManager::init(std::vector<NPC*> listNPC) {

    if (noQuestLabel != nullptr) noQuestLabel->removeFromParentAndCleanup(true);
        noQuestLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 12);
        noQuestLabel->setColor(Color3B::GRAY);
        noQuestLabel->setName("NoQuestLabel");
        noQuestLabel->setPosition(Vec2(0, 0));
        noQuestLabel->setString(StringUtils::format("%s", "No Quest Available"));
        this->addChild(noQuestLabel, 11);
    
    for (auto npc : listNPC) {
        if (npc->getName() == "Durin")durin = npc;
    }
    //Quest UI
    if(questManagerUI != nullptr) questManagerUI->removeFromParentAndCleanup(true);
    questManagerUI = Sprite::create("res/questDetails.png");
    questManagerUI->setName("QuestManagerUI");
    //questManagerUI->setOpacity(100);
    this->addChild(questManagerUI);
    questManagerUI->setScaleX(0.3 * Director::getInstance()->getContentScaleFactor());
    questManagerUI->setScaleY(0.25 * Director::getInstance()->getContentScaleFactor());


    // Ly kch thc ca QuestManager node
    auto contentSize = this->getContentSize();

    // Tnh ton v tr trung tm ca QuestManager
    Vec2 centerPosition = Vec2(contentSize.width / 2, contentSize.height / 2 - 35);

    // t v tr cho questManagerUI ti v tr trung tm ca QuestManager
    questManagerUI->setPosition(centerPosition);

    
    //Icon 
    if (questNotReceiveIcon != nullptr) {
        questNotReceiveIcon->release();

    }
    questNotReceiveIcon = Sprite::create("res/quest_not_received_icon.png");
    questNotReceiveIcon->setName("QuestNotReceiveIcon");
    questNotReceiveIcon->setScale(0.1);
    questNotReceiveIcon->setPosition(Vec2(0, 35));
    questNotReceiveIcon->retain();

    if (questProcessingIcon != nullptr) {
        questProcessingIcon->release();

    }
    questProcessingIcon = Sprite::create("res/doing_quest_icon.png");
    questProcessingIcon->setName("QuestProcessingIcon");
    questProcessingIcon->setScale(0.1);
    questProcessingIcon->setPosition(Vec2(0, 35));
    questProcessingIcon->retain();

    if (questPendingIcon != nullptr) {
        questPendingIcon->release();

    }
    questPendingIcon = Sprite::create("res/important_quest_icon.png");
    questPendingIcon->setName("QuestPendingIcon");
    questPendingIcon->setScale(0.1);
    questPendingIcon->setPosition(Vec2(0, 35));
    questPendingIcon->retain();

    Quest::QuestRewards rewards1;
    rewards1.gold = 100;
    rewards1.exp = 50;
    rewards1.itemName = "Legendary Sword";
    Quest* quest1 = Quest::create("Free the sinful souls",
        "The unfortunate people were burned physically and mentally by Terra. Only a few souls remain. But perhaps death is the way out? ",
        std::vector<Quest::Item*>(),
        { new Quest::Enemy("Skeleton", 0, 3) }, 0, rewards1, durin);
    quest1->questDialogue = {
        "Hmmm! You really surprised me.",
        "Looks like you haven't lost your reason yet.",
        "You should celebrate that because you won't be able to maintain that weak spirit much longer Hahahaa.",
        "Oh it seems I forgot to introduce myself.",
        "My name is Robert and I am also a Slayer like you.",
        "You may not believe it, but believe me",
        "Everyone at first maintained the foolish belief when they came here",
        "Slowly the soul and mind are broken and there is no one left who can hold on to what they believe.",
        "Maybe it's me hahaha.",
        "I've seen too many people like you.",
        "But perhaps there's nothing better than letting you experience them, right?",
        "Do you see those skeletons out there?",
        "They are victims of Terra, one of the Three Great Evil Gods",
        "Their curiosity and stupidity angered him and Terra burned both their bodies and souls.",
        "But he left behind a little soul to serve as a deterrent to others.",
        "I believe that death is their liberation.",
        "Including my old friends, but I couldn't kill them.",
        "Please help me free them."};

    quest1->processingQuestDialogue = {
        "What are you waiting for?",
        "A slayer must fight",
        "I have nothing more to say"
    };

    quest1->completedQuestDialogue = {
        "You really did it",
        "You have freed those sinful souls",
        "I suddenly felt a bit of relief.",
        "Maybe my old friend won't hate me, right?",
        "Thank you very much.",
        "Please accept this reward."
    };

    Quest::QuestRewards rewards2;
    rewards2.gold = 100;
    rewards2.exp = 50;
    rewards2.itemName = "Legendary Sword";
    Quest* quest2 = Quest::create("Collect crystals",
        "Collect crystal to upgrade weapons !",
        { new Quest::Item("crystal", 0, 5) },
        std::vector<Quest::Enemy*>(), 0, rewards2, durin);
    quest2->questDialogue = {
        "Hmmm! Your weapon are suck. ",
        "You cant do much with that weapon",
        "Just go out there and collect some crystal.",
        "I will upgrade your weapons Hahahaa.",
        "Just believe in me."};

    quest2->processingQuestDialogue = {
        "I need more crystals",
        "You just go and find it for me"
    };

    quest2->completedQuestDialogue = {
        "Oh! These crystal looks so good !",
        "Thank you !"
    };

    Quest::QuestRewards rewards3;
    rewards3.gold = 100;
    rewards3.exp = 50;
    rewards3.itemName = "Legendary Sword";
    Quest* quest3 = Quest::create("Kill BabySpiders",
        "The imperfect children of a mother who longs to have a child of her own. Okyanus' loyal guards will prevent anyone from touching their mother.",
        std::vector<Quest::Item*>(),
        { new Quest::Enemy("BabySpider", 0, 5) }, 0, rewards3, durin);
    quest3->questDialogue = {
        "Maybe you're not like the people I know.",
        "It's worth watching you fail hahaha.",
        "Always remember that if you can still keep your life, it is never too late to turn back..",
        "Since you have defeated Terra, you should also know about the remaining gods.",
        "About their loyal minions.",
        "They are called Baby Spiders.",

        "Imperfect children of the Outer God Okyanus.",

        "They only copy the shape of Okyanus and do not inherit anything else.",
        "But they are extremely loyal and numerous, making them extremely dangerous.",
        "Many arrogant people just because they despised Baby Spiders, they ripped them to pieces.",
        "But you must overcome them to find Okyanus.",
        "I also have a deep hatred for them.",
        "Please help me!" };

    quest3->processingQuestDialogue = {
        "What are you still doing here?",
        "Aren't you looking for Okyanus?",
        "First, you must defeat Baby Spider first"
    };

    quest3->completedQuestDialogue = {
        "You surprise me time and time again",
        "This is your reward and prepare for the next battle"
    };

    Quest::QuestRewards rewards4;
    rewards4.gold = 100;
    rewards4.exp = 50;
    rewards4.itemName = "Legendary Sword";
    Quest* quest4 = Quest::create("Defeat The Outer Terra",
        "One of the three gods who govern the world of Terra. He has ruled the world for too long. But there comes a time when humanity needs to find a way out for themselves without depending on the gods.",
        std::vector<Quest::Item*>(),
        { new Quest::Enemy("Terra", 0, 1) }, 0, rewards4, durin);
    quest4->questDialogue = {
    "There are many things you don't really know about your journey.",
    "World, Outer Gods or Beast Slayers.",
    "The god you are about to face is unlike anything you have ever known.",
    "Have you ever heard of the god representing the earth Terra?",
    "Terra is an ancient Outer God who came to earth a long time ago.",
    "People say that he is as big as a mountain and has dark gray skin.",
    "But gradually the earth gradually warmed up and humanity's stupidity angered Terra.",
    "He turned his anger into volcanic eruptions and the red color of fire.",
    "Not many people who face Terra come back alive, so be careful with your weak life hahaha."};

    quest4->processingQuestDialogue = {
        "You should be careful",
        "Terra is a very powerful and fierce ancient god.",
        "He is the oldest god.",
        "One mistake can be costly when facing him.",
        "Be careful young slayer"
    };

    quest4->completedQuestDialogue = {
    "You... You really did it.",
    "I cannot believe that a human like you could destroy an Outer Gods Terra.",
    "Tskk What are you that can do what I can't?",
    "But it doesn't matter anymore",
    "The remaining Outer Gods must have also known about this.",
    "Hope you won't die soon hahahaha"
    };

    Quest::QuestRewards rewards5;
    rewards5.gold = 100;
    rewards5.exp = 50;
    rewards5.itemName = "Legendary Sword";
    Quest* quest5 = Quest::create("Defeat the Outer Gods Okyanus",
        "A female Outer Gods is weakening. She was once a gentle mother and loved humans. But it is human ignorance that has ruined everything. Perhaps death is also a way out for Okyanus.",
        std::vector<Quest::Item*>(),
        { new Quest::Enemy("Okyanus", 0, 1) }, 0, rewards5, durin);
    quest5->questDialogue = {
    "Beware! There's an Outer God are waiting for you!",
    "I'm not sure but she got weakers",
    "Because The Baby Spider army are more stronger than before",
    "Thats weird!",
    "You should be careful!"
     };

    quest5->processingQuestDialogue = {
        "Okyanus will not be alone.",
        "She is always protected by an army of Baby Spiders",
        "You should be careful if you don't want to be overwhelmed by numbers"
    };

    quest5->completedQuestDialogue = {
    "Damn it!",
    "Why can a stubborn person like you do things that the Beast Slayers struggle to do?",
    "I bet that sooner or later you will collapse and go crazy hahaha.",
    "Take it and I will continue to wait for it."
    };



    quest1->retain();
    listQuest.push_back(quest1);
    quest2->retain();
    listQuest.push_back(quest2);
    quest3->retain();
    listQuest.push_back(quest3);
    quest4->retain();
    listQuest.push_back(quest4);
    quest5->retain();
    listQuest.push_back(quest5);
    currentQuestIndex = 0;
    currentQuest = listQuest[currentQuestIndex];
    currentQuest->setName("CurrentQuest");
    currentQuest->questStatus = currentQuest->NOT_RECEIVED;
    changeQuestIconOfNPC(currentQuest->NOT_RECEIVED);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(QuestManager::onTouchBegan, this);
    questManagerUI->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, questManagerUI);

    return true;
}
bool QuestManager::onTouchBegan(Touch* touch, Event* event) {
    if (QuestManager::getInstance()->isShowDetail) {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);

        if (rect.containsPoint(locationInNode)) {
            CCLOG("Touch to Quest UI");
        }
        else {
            QuestManager::getInstance()->generalDisplay();
            return true;
        }

        return false;
    }
    return false;
}
QuestManager::QuestManager() {

}
QuestManager::~QuestManager() {

}
bool QuestManager::isCurrentQuestComplete() {
    
    if (currentQuest->questStatus != currentQuest->COMPLETED) {
        for (auto enemy : currentQuest->enemies) {
            if (enemy->killed < enemy->required) {
                return false;
            }
        }
        for (auto item : currentQuest->items) {
            if (item->collected < item->required) {
                return false;
            }
        }
        increaseCurrentMap();
        currentQuest->questStatus = currentQuest->COMPLETED;
        changeQuestIconOfNPC(currentQuest->COMPLETED);
        NotificationManager::getInstance()->showMessageNotification("Completed", Vec2::ZERO, Color3B(161, 251, 142), 17);
    }
    return true;
}
void QuestManager::changeQuestIconOfNPC(int questStatus) {
    switch (questStatus)
    {
    case 0:
        if (currentQuest->npc->getChildByName("QuestNotReceiveIcon")) {
            currentQuest->npc->removeChildByName("QuestNotReceiveIcon");
        }
        else if (currentQuest->npc->getChildByName("QuestProcessingIcon")) {
            currentQuest->npc->removeChildByName("QuestProcessingIcon");
        }
        else if (currentQuest->npc->getChildByName("QuestPendingIcon")) {
            currentQuest->npc->removeChildByName("QuestPendingIcon");
        }
        currentQuest->npc->addChild(questNotReceiveIcon);
        break;
    case 1:
        if (currentQuest->npc->getChildByName("QuestNotReceiveIcon")) {
            currentQuest->npc->removeChildByName("QuestNotReceiveIcon");
        }
        else if (currentQuest->npc->getChildByName("QuestProcessingIcon")) {
            currentQuest->npc->removeChildByName("QuestProcessingIcon");
        }
        else if (currentQuest->npc->getChildByName("QuestPendingIcon")) {
            currentQuest->npc->removeChildByName("QuestPendingIcon");
        }
        currentQuest->npc->addChild(questProcessingIcon);
        break;
    case 2:
        if (currentQuest->npc->getChildByName("QuestNotReceiveIcon")) {
            currentQuest->npc->removeChildByName("QuestNotReceiveIcon");
        }
        else if (currentQuest->npc->getChildByName("QuestProcessingIcon")) {
            currentQuest->npc->removeChildByName("QuestProcessingIcon");
        }
        else if (currentQuest->npc->getChildByName("QuestPendingIcon")) {
            currentQuest->npc->removeChildByName("QuestPendingIcon");
        }
        currentQuest->npc->addChild(questPendingIcon);
        break;
    default:
        if (currentQuest->npc->getChildByName("QuestNotReceiveIcon")) {
            currentQuest->npc->removeChildByName("QuestNotReceiveIcon");
        }
        else if (currentQuest->npc->getChildByName("QuestProcessingIcon")) {
            currentQuest->npc->removeChildByName("QuestProcessingIcon");
        }
        else if (currentQuest->npc->getChildByName("QuestPendingIcon")) {
            currentQuest->npc->removeChildByName("QuestPendingIcon");
        }
        break;
    }
}
void QuestManager::setNPC(NPC* durin) {
    this->durin = durin;
}
void QuestManager::resetOrderOfListQuest() {
    currentQuestIndex++;
    if (currentQuestIndex <= listQuest.size() - 1) {
        currentQuest = listQuest[currentQuestIndex];

        currentQuest->setName("CurrentQuest");
        currentQuest->questStatus = currentQuest->NOT_RECEIVED;
        changeQuestIconOfNPC(0);
        showAndHideNoQuest(true);
    }
    else {
        QuestManager::getInstance()->changeQuestIconOfNPC(10);
        currentQuest = nullptr;
        showAndHideNoQuest(true);
    }

}
void QuestManager::showAndHideNoQuest(bool isShow) {
    noQuestLabel->setVisible(isShow);
}
void QuestManager::generalDisplay() {
    if (currentQuest != nullptr) {
        currentQuest->descriptionLabel->setVisible(false);
        currentQuest->objectiveLabel->setVisible(false);
        currentQuest->questInformationLabel->setVisible(false);
        currentQuest->nameLabel->setPosition(Vec2(currentQuest->getContentSize().width * currentQuest->getScaleX() / 2, currentQuest->getContentSize().height));

        showAndHideNoQuest(true);
        this->questManagerUI->setVisible(false);
        //this->setScale(0.3);
        //currentQuest->nameLabel->setScale(5);
        this->setPosition(Vec2(210, 68)); // quest name

        // vi tri
        if (currentQuest->enemies.size() > 0) {
            int rowEnemies = 0;
            for (auto progressLabel : currentQuest->progressLabelList) {
                progressLabel->setPosition(Vec2(-(currentQuest->getContentSize().width * currentQuest->getScaleX() / 2 + 10 * Director::getInstance()->getContentScaleFactor()), rowEnemies - 10 * Director::getInstance()->getContentScaleFactor())); // set pos
                rowEnemies++;
            }
            if(currentQuest->questStatus != currentQuest->NOT_RECEIVED)
            showAndHideNoQuest(false);
        }
        if (currentQuest->items.size() > 0) {
            int rowItems = 0;
            for (auto progressLabel : currentQuest->progressLabelList) {
                progressLabel->setPosition(Vec2(-(currentQuest->getContentSize().width * currentQuest->getScaleX() / 2 + 10 * Director::getInstance()->getContentScaleFactor()), rowItems - 10 * Director::getInstance()->getContentScaleFactor())); // set pos
                rowItems++;
            }
            if (currentQuest->questStatus != currentQuest->NOT_RECEIVED)
            showAndHideNoQuest(false);
        }
    }
    isShowDetail = false;
}
void QuestManager::detailDisplay() {
    if (currentQuest != nullptr) {
        currentQuest->descriptionLabel->setVisible(true);
        currentQuest->objectiveLabel->setVisible(true);
        currentQuest->questInformationLabel->setVisible(true);
        currentQuest->nameLabel->setPosition(Vec2(currentQuest->getContentSize().width * currentQuest->getScaleX() / 2, currentQuest->getContentSize().height + 50));

        this->questManagerUI->setVisible(true);

        //this->setScale(1);
        //currentQuest->nameLabel->setScale(1);
        this->setPosition(Vec2(0, 30));

        showAndHideNoQuest(true);
        
        if (currentQuest->enemies.size() > 0) {
            int rowEnemies = 0;
            for (auto progressLabel : currentQuest->progressLabelList) {
                progressLabel->setPosition(Vec2(currentQuest->getContentSize().width * currentQuest->getScaleX() / 2 + 50 * Director::getInstance()->getContentScaleFactor(), rowEnemies - 60 * Director::getInstance()->getContentScaleFactor()));
                rowEnemies++;
            }
            if (currentQuest->questStatus != currentQuest->NOT_RECEIVED)
            showAndHideNoQuest(false);
        }
        if (currentQuest->items.size() > 0) {
            int rowItems = 0;
            for (auto progressLabel : currentQuest->progressLabelList) {
                progressLabel->setPosition(Vec2(currentQuest->getContentSize().width * currentQuest->getScaleX() / 2 - 50 * Director::getInstance()->getContentScaleFactor(), rowItems - 60 * Director::getInstance()->getContentScaleFactor()));
                rowItems++;
            }
            if (currentQuest->questStatus != currentQuest->NOT_RECEIVED)
            showAndHideNoQuest(false);
        }
    }
    isShowDetail = true;
}