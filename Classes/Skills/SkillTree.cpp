#include "SkillTree.h"
#include "Player/Player.h"
#include "TutorialManager/TutorialManager.h"
#include "HelloWorldScene.h"
#include "SkillFactory.h"


#include "UserInterface/InGame/InGameUI.h"
#include "UserInterface/InGame/HealthBar.h"
#include "UserInterface//InGame/HamburgerMenuButton.h"
#include "UserInterface/InGame/QuestButton.h"
#include "UserInterface/InGame/InventoryButton.h"
#include "UserInterface/InGame/UpgradeInventoryButton.h"
#include "UserInterface/InGame/TalentButton.h"
#include "Quest/QuestManager.h"

SkillTree* SkillTree::createSkillTree(Player* player, SkillBase::SkillTalent* skillRoot)
{
    SkillTree* skill = new (std::nothrow) SkillTree();
    if (skill && skill->init(player, skillRoot))
    {
        skill->autorelease();
        return skill;
    }
    else
    {
        delete skill;
        skill = nullptr;
        return nullptr;
    }
}

bool SkillTree::init(Player* player, SkillBase::SkillTalent* skillRoot)
{
    if (!Node::init())
    {
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();
    drawTree(player, skillRoot, Vec2(0, 0), 200, 200, nullptr);

    skillDetail = Sprite::create("res/c_container.png");
    skillDetail->setName("SkillDetail");
    this->addChild(skillDetail, 4);
    skillDetail->setVisible(false);
    if (_overlay) _overlay->setVisible(false);

    moneyLabel = Label::createWithTTF("Skill Point: " + std::to_string(player->getSkillPoint()), "fonts/Diablo Light.ttf", 22);
    moneyLabel->setPositionX(200);
    moneyLabel->setString(StringUtils::format("Skill Point: %d", player->getSkillPoint()));
    this->addChild(moneyLabel);
    moneyLabel->setVisible(false);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, player](cocos2d::Touch* touch, cocos2d::Event* event)
        {

            if (this->isVisible())
            {
                auto touchPoint = this->convertToNodeSpace(touch->getLocation());

                if (!skillRects.empty())
                {
                    for (auto parentNode : skillRects)
                    {
                        auto parentPosition = std::get<1>(parentNode)->getPosition();
                        auto nodeSize = std::get<1>(parentNode)->getContentSize();
                        auto parentBoundingBox = cocos2d::Rect(parentPosition.x - nodeSize.width / 2, parentPosition.y - nodeSize.height / 2, nodeSize.width, nodeSize.height);
                        
                        if (isVisible() && parentBoundingBox.containsPoint(touchPoint) && this->getParent()->isVisible() && std::get<1>(parentNode)->getParent()->isVisible() && std::get<1>(parentNode)->isVisible())
                        {
                            if (std::get<0>(parentNode)->skillName.find(this->treeName) != std::string::npos) {
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                                auto skillNode = SkillFactory::createSkill(std::get<0>(parentNode)->skillName);
                                showSkillDetail(skillNode, player);
                            }
                        }
                    }
                }
            }
            return false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


void SkillTree::drawTree(Player* player, SkillBase::SkillTalent* node, Vec2 position, float xSpacing, float ySpacing, SkillBase::SkillTalent* parent)
{
    bool isUnlock = false;
    for (auto skill : player->getPlayerSkills()) {
        if (skill->getSkillName() == node->skillName) {
            isUnlock = true;
            break;
        }
    }
    Node* parentNode = Node::create();
    parentNode->setName(node->skillName + " Node");
    auto skillSpr = Sprite::create(node->iconPath);
    skillSpr->setScale(0.00025f * visibleSize.width);
    skillSpr->setName(node->skillName + " Sprite");

    auto blackNode = Sprite::create("res/minimap_frame.png");
    blackNode->setOpacity(180);
    blackNode->setPosition(Vec2(skillSpr->getContentSize().width / 2, skillSpr->getContentSize().height / 2));
    blackNode->setName("Black Node");
    blackNode->setContentSize(skillSpr->getContentSize());
    if (!isUnlock)skillSpr->addChild(blackNode, 10);
    this->setContentSize(Size(this->getContentSize().width * (0.00025f * visibleSize.width), this->getContentSize().height * (0.00025f * visibleSize.width)));
    if (parent)
        node->parent = parent;
    else
        node->parent = nullptr;

    treeDrawed.push_back(skillSpr);
    listOfSkills.push_back(node);

    parentNode->addChild(skillSpr, 5);
    parentNode->setPosition(position);
    if (!isUnlock)
    {
        auto scoreToUnlock = Label::createWithTTF("", "fonts/Diablo Light.ttf", 25);
        scoreToUnlock->setName(node->skillName + " LevelSkill");
        scoreToUnlock->setPosition(skillSpr->getContentSize().width * 0.2 / 2, skillSpr->getContentSize().height * 0.2 / 2);
        parentNode->addChild(scoreToUnlock, 20);
        hideLabels.push_back(scoreToUnlock);
    }
    else {
        int currentLevel = 0;
        for (auto skill : player->getPlayerSkills()) {
            if (skill->getSkillName() == node->skillName) {
                currentLevel = skill->getCurrentLevelSkill();
                break;
            }
        }
        
        auto scoreToUnlock = Label::createWithTTF("Lv"+std::to_string(currentLevel), "fonts/Diablo Light.ttf", 25);
        if(currentLevel == 5) {
            scoreToUnlock->setBMFontSize(25);
            scoreToUnlock->setString("Max");
        }
        else {
            scoreToUnlock->setString("Lv" + std::to_string(currentLevel));
        }
        scoreToUnlock->setName(node->skillName + " LevelSkill");
        scoreToUnlock->setPosition(skillSpr->getContentSize().width * 0.2 / 2, skillSpr->getContentSize().height * 0.2 / 2);
        parentNode->addChild(scoreToUnlock, 20);
        hideLabels.push_back(scoreToUnlock);
    }
    this->addChild(parentNode, 2);
    parentNode->setContentSize(skillSpr->getContentSize() * (0.00025f * visibleSize.width));
    skillRects.push_back(std::make_tuple(node, parentNode));

    Vec2 childPosition = position - Vec2(0, ySpacing);
    if (static_cast<int>(node->children.size()) == 3)
    {
        childPosition = Vec2(-xSpacing, 0) + position - Vec2(0, ySpacing);
    }
    else if (static_cast<int>(node->children.size()) == 4)
    {
        childPosition = Vec2(-xSpacing * 2 + parentNode->getContentSize().width * (0.0002f * visibleSize.width), 0) + position - Vec2(0, ySpacing);
    }
    if (static_cast<int>(node->children.size()) > 0)
    {
        childrenQuan++;
    }
    else
    {
        treeHeight = skillSpr->getContentSize().height * 0.0002f * visibleSize.width + (childrenQuan * ySpacing) + (skillSpr->getContentSize().height * 0.0002f * visibleSize.width * childrenQuan);
    }
    // V ng thng t Skill ti Skill con ca n
    for (auto child : node->children)
    {
        auto drawNode = DrawNode::create();
        drawNode->drawLine(position, childPosition, Color4F(106, 90, 205,1));
        this->addChild(drawNode, 1);
        treeDrawedNode.push_back(drawNode);
        drawTree(player, child, childPosition, xSpacing, ySpacing, node);
        childPosition.x += xSpacing;
    }
}

void SkillTree::showSkillDetail(SkillBase* skill, Player* player)
{

    if (_overlay) _overlay->setVisible(true);
    // TUTORIAL
    int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
    if (currentTutorial == 5) {
        TutorialManager::getInstance()->nextTutorial();
    }

    float scaleFactor = Director::getInstance()->getContentScaleFactor();
    float scale = visibleSize.width / visibleSize.height * 2;
    // Check skill exist in player's Skills
    bool isSkillUnlocked = false;

    std::string skillname = skill->getSkillTalent()->skillName;

    for (auto skillplayer : player->getPlayerSkills())
    {
        if (skillplayer->getSkillTalent()->skillName == skill->getSkillTalent()->skillName)
        {
            isSkillUnlocked = true;
            skill = skillplayer;
            break;
        }
    }
    // Create Skill detail
    if (this->getChildByName("SkillDetail"))
    {
        auto child = this->getChildByName("SkillDetail");
        child->removeAllChildren();
        child->removeFromParent();
    }
    skillDetail = Sprite::create("res/c_full_header_1.png");
    skillDetail->setRotation(180);
    skillDetail->setOpacity(0);
    skillDetail->setName("SkillDetail");
    skillDetail->setPosition(Vec2(1450 / scaleFactor, -500 / scaleFactor));
    skillDetail->setScale(2 * scaleFactor);
    this->addChild(skillDetail, 4);
    listSkillDetails.push_back(skillDetail);

    // SetUp Detail
    // IconSprite
    auto icon = Sprite::create(skill->getSkillTalent()->iconPath);
    icon->setRotation(180);
    icon->setScale(0.08 * scaleFactor);
    icon->setPosition(Vec2((skillDetail->getBoundingBox().size.width / (scaleFactor * 2.5)), 75 / scaleFactor));
    skillDetail->addChild(icon, 5);
    // LABEL
    // Description Label

    auto descriptionLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (22 / scaleFactor));
    descriptionLable->setColor(Color3B::WHITE);
    descriptionLable->setRotation(180);
    descriptionLable->setScale(0.35 * scaleFactor);
    descriptionLable->setPosition(Vec2(skillDetail->getBoundingBox().size.width / (12 / scaleFactor), 150 / scaleFactor));
    skillDetail->addChild(descriptionLable);
    descriptionLable->setString(StringUtils::format("%s", splitLines(skill->getSkillTalent()->description).c_str()));
    // Cooldown
    int cooldown = skill->getSkillCooldown();
    int cooldownReduct = skill->getSkillCooldownReduct();
    auto cooldownLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (20 / scaleFactor));
    cooldownLable->setColor(Color3B::WHITE);
    cooldownLable->setRotation(180);
    cooldownLable->setScale(0.35 * scaleFactor);
    cooldownLable->setPosition(Vec2(skillDetail->getBoundingBox().size.width / (40 / scaleFactor), 47.4 / scaleFactor));
    skillDetail->addChild(cooldownLable);
    cooldownLable->setString(StringUtils::format("Cooldown: %d", cooldown));
    // Cost 
    int manaCost = skill->getManaCost();
    auto manaCostLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (20 / scaleFactor));
    manaCostLable->setColor(Color3B::WHITE);
    manaCostLable->setRotation(180);
    manaCostLable->setScale(0.35 * scaleFactor);
    manaCostLable->setPosition(Vec2(skillDetail->getBoundingBox().size.width / (40 / scaleFactor), 60.7 / scaleFactor));
    skillDetail->addChild(manaCostLable);
    manaCostLable->setString(StringUtils::format("Mana Cost: %d", manaCost));
    // Level
    int currentLevel = 0;
    if (isSkillUnlocked)
        currentLevel = skill->getCurrentLevelSkill();
    int maxLevel = skill->getMaxLevelSkill();
    auto levelLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (20 / scaleFactor));
    levelLable->setColor(Color3B::WHITE);
    levelLable->setRotation(180);
    levelLable->setScale(0.35 * scaleFactor);
    levelLable->setPosition(Vec2(skillDetail->getBoundingBox().size.width / (40 / scaleFactor), 74 / scaleFactor));
    skillDetail->addChild(levelLable);
    levelLable->setString(StringUtils::format("Level: %d/%d", currentLevel, maxLevel));

    int damageSkill = skill->getSkillDamage();
    // Damage
    if (damageSkill != 0)
    {
        auto damageLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (20 / scaleFactor));
        damageLable->setColor(Color3B::WHITE);
        damageLable->setRotation(180);
        damageLable->setScale(0.35 * scaleFactor);
        damageLable->setPosition(Vec2(skillDetail->getBoundingBox().size.width / (40 / scaleFactor), 87.23 / scaleFactor));
        skillDetail->addChild(damageLable);
        damageLable->setString(StringUtils::format("Damage: %d", damageSkill));
    }
    else
    {
        // Buff
        int hpExtra = skill->getHPExtra();
        int mpExtra = skill->getMPExtra();
        int armorExtra = skill->getArmorExtra();
        int dmgExtra = skill->getDamageExtra();
        auto buffLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (20 / scaleFactor));
        buffLable->setColor(Color3B::WHITE);
        buffLable->setRotation(180);
        buffLable->setScale(0.3 * scaleFactor);
        buffLable->setPosition(Vec2(skillDetail->getBoundingBox().size.width / (3.41 * scaleFactor) - 70, 83.4367 / scaleFactor + 25));
        skillDetail->addChild(buffLable);
        buffLable->setString(StringUtils::format("Increase: %d HP, %d MP, %d Armor, %d Damage", hpExtra, mpExtra, armorExtra, dmgExtra));
    }
    // Unlock Points Required
    auto unlockPointRequiredLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (20 / scaleFactor));
    unlockPointRequiredLable->setColor(Color3B::WHITE);
    unlockPointRequiredLable->setRotation(180);
    unlockPointRequiredLable->setScale(0.4 * scaleFactor);
    unlockPointRequiredLable->setPosition(Vec2(220.6 / scaleFactor, 59.73 / scaleFactor));
    skillDetail->addChild(unlockPointRequiredLable);
    unlockPointRequiredLable->setString(StringUtils::format("Exp required: %d", skill->getSkillTalent()->unlockPoint));
    this->unlockPointRequired = unlockPointRequiredLable;
    // Level Required
    auto levelRequiredLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (20 / scaleFactor));
    levelRequiredLable->setColor(Color3B::WHITE);
    levelRequiredLable->setRotation(180);
    levelRequiredLable->setScale(0.4 * scaleFactor);
    levelRequiredLable->setPosition(Vec2(220.6 / scaleFactor, 78.73 / scaleFactor));
    skillDetail->addChild(levelRequiredLable);
    levelRequiredLable->setString(StringUtils::format("Level required: %d", skill->getSkillTalent()->levelRequired));
    // Upgrade Points Required
    auto upgradePointRequiredLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", (20 / scaleFactor));
    upgradePointRequiredLable->setColor(Color3B::WHITE);
    upgradePointRequiredLable->setRotation(180);
    upgradePointRequiredLable->setScale(0.4 * scaleFactor);
    upgradePointRequiredLable->setPosition(Vec2(220.6 / scaleFactor, 59.73 / scaleFactor));
    skillDetail->addChild(upgradePointRequiredLable);
    upgradePointRequiredLable->setString(StringUtils::format("Upgrade Point: %d", skill->getSkillTalent()->upgradePoint));
    this->upgradePointRequired = upgradePointRequiredLable;
    // Implement sprite

    // Nt Equip
    auto equip = Sprite::create("res/papper_container2.png");
    equip->setName("Equip");
    equip->setScale(0.08 * scaleFactor);
    equip->setRotation(180);
    equip->setPosition(Vec2(95 / scaleFactor, 233.33 / scaleFactor));
    skillDetail->addChild(equip);
    this->equipButton = equip;

    auto equipLabel = Label::createWithTTF("Equip", "fonts/Diablo Light.ttf", 35);
    equipLabel->setColor(Color3B::GRAY);
    // equipLabel->setRotation(180);
    equipLabel->setScale(1 * Director::getInstance()->getContentScaleFactor());
    equipLabel->setPosition(Vec2(equip->getContentSize().width / 2, equip->getContentSize().height / 2));
    equip->addChild(equipLabel);

    //
    Size size = Director::getInstance()->getVisibleSize();
    //float scale = size.width / size.height * 2;
    auto sizeWD4 = size.width / 4;
    auto sizeHD4 = size.height / 4;

    //Cancel Equip Button
    auto cancelEquip = Sprite::create("res/button_tab2.png");
    cancelEquip->setScale(0.3 * Director::getInstance()->getContentScaleFactor());
    cancelEquip->setName("CancelEquip");
    cancelEquip->setPosition(Vec2::ZERO);
    InGameUI::getInstance(player)->addChild(cancelEquip, 100);

    //Pos
    auto offsetW5 = cancelEquip->getContentSize().width / 2;
    auto offsetH5 = cancelEquip->getContentSize().height / 2;
    cancelEquip->setPosition(Vec2((sizeWD4 + 3 * offsetW5) / scale, (-sizeHD4 + 3 * offsetH5) / scale) + Vec2(-100, 50));

    // SLOT
    auto slot4 = Sprite::create("res/minimap_frame.png");
    slot4->setScale(0.1 * Director::getInstance()->getContentScaleFactor());
    slot4->setOpacity(50);
    slot4->setName("Slot4");
    InGameUI::getInstance(player)->addChild(slot4, 100);
    //Pos
    auto offsetW4 = slot4->getContentSize().width / 2;
    auto offsetH4 = slot4->getContentSize().height / 2;
    slot4->setPosition(Vec2((sizeWD4 + 3 * offsetW4) / scale, (-sizeHD4 + 3 * offsetH4) / scale) + Vec2(118, -34 - 10));

    auto slot3 = Sprite::create("res/minimap_frame.png");
    slot3->setScale(0.1 * Director::getInstance()->getContentScaleFactor());
    slot3->setOpacity(50);
    slot3->setName("Slot3");
    InGameUI::getInstance(player)->addChild(slot3, 100);
    //Pos
    auto offsetW3 = slot3->getContentSize().width / 2;
    auto offsetH3 = slot3->getContentSize().height / 2;
    slot3->setPosition(Vec2((sizeWD4 + 2 * offsetW3) / scale - 9, (-sizeHD4 + 2 * offsetH3) / scale + 10) + Vec2(87, -30));

    auto slot2 = Sprite::create("res/minimap_frame.png");
    slot2->setScale(0.1 * Director::getInstance()->getContentScaleFactor());
    slot2->setOpacity(50);
    slot2->setName("Slot2");
    InGameUI::getInstance(player)->addChild(slot2, 100);
    //Pos
    auto offsetW2 = slot2->getContentSize().width / 2;
    auto offsetH2 = slot2->getContentSize().height / 2;
    slot2->setPosition(Vec2((sizeWD4 + offsetW2) / scale - 7, (-sizeHD4 + offsetH2) / scale + 8) + Vec2(60 + 10, -40));

    auto slot1 = Sprite::create("res/minimap_frame.png");
    slot1->setScale(0.1 * Director::getInstance()->getContentScaleFactor());
    slot1->setOpacity(50);
    slot1->setName("Slot1");
    InGameUI::getInstance(player)->addChild(slot1, 100);
    //Pos
    auto offsetW1 = slot1->getContentSize().width / 2;
    auto offsetH1 = slot1->getContentSize().height / 2;
    slot1->setPosition(Vec2(sizeWD4 / scale, -sizeHD4 / scale) + Vec2(80 + 10, -70)); 
    //
    this->slot1Button = slot1;

    //Attack Button
    auto attackButton = Sprite::create("res/attack_button.png");
    attackButton->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
    attackButton->setName("AttackButton");
    InGameUI::getInstance(player)->addChild(attackButton, 100);
    //Pos
    attackButton->setPosition(Vec2(sizeWD4 / scale, -sizeHD4 / scale) + Vec2(168, -30));

    listSlots.push_back(slot1);
    listSlots.push_back(slot2);
    listSlots.push_back(slot3);
    listSlots.push_back(slot4);

    //// Skill Icon In Slot
    //Sprite* skillSlot1 = nullptr;
    //Sprite* skillSlot2 = nullptr;
    //Sprite* skillSlot3 = nullptr;
    //Sprite* skillSlot4 = nullptr;

    //for (auto playerSkill : player->getPlayerSkills())
    //{
    //    if (playerSkill->getSkillIndex() == 1)
    //        skillSlot1 = Sprite::create(playerSkill->getSkillTalent()->iconPath);
    //    if (playerSkill->getSkillIndex() == 2)
    //        skillSlot2 = Sprite::create(playerSkill->getSkillTalent()->iconPath);
    //    if (playerSkill->getSkillIndex() == 3)
    //        skillSlot3 = Sprite::create(playerSkill->getSkillTalent()->iconPath);
    //    if (playerSkill->getSkillIndex() == 4)
    //        skillSlot4 = Sprite::create(playerSkill->getSkillTalent()->iconPath);
    //}
    //if (skillSlot1)
    //{
    //    //skillSlot1->setScale(0.1 * Director::getInstance()->getContentScaleFactor());
    //    slot1->addChild(skillSlot1);
    //}
    //if (skillSlot2)
    //{
    //    //skillSlot2->setScale(0.1 * Director::getInstance()->getContentScaleFactor());
    //    slot2->addChild(skillSlot2);
    //}
    //if (skillSlot3)
    //{
    //    //skillSlot3->setScale(0.1 * Director::getInstance()->getContentScaleFactor());
    //    slot3->addChild(skillSlot3);
    //}
    //if (skillSlot4)
    //{
    //    //skillSlot4->setScale(0.1 * Director::getInstance()->getContentScaleFactor());
    //    slot4->addChild(skillSlot4);
    //}

    auto cancelEquipLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 20);
    cancelEquipLabel->setColor(Color3B::WHITE);
    cancelEquipLabel->setName("CancelEquipLabel");
    //slot4Lable->setRotation(180);
    //cancelEquipLabel->setScale(0.05 * Director::getInstance()->getContentScaleFactor());
    cancelEquipLabel->setPosition(Vec2(cancelEquip->getContentSize().width / 2, cancelEquip->getContentSize().height / 2));
    cancelEquipLabel->setString(StringUtils::format("%s", "Cancel Equip"));
    cancelEquip->addChild(cancelEquipLabel, 10);

    auto slot4Lable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 20);
    slot4Lable->setColor(Color3B::GRAY);
    slot4Lable->setName("Slot4Label");
    //slot4Lable->setRotation(180);
    slot4Lable->setScale(0.05 * Director::getInstance()->getContentScaleFactor());
    slot4Lable->setPosition(Vec2(slot4->getContentSize().width / 2, slot4->getContentSize().height / 2));
    slot4Lable->setString(StringUtils::format("%s", "Slot 4"));
    slot4->addChild(slot4Lable, 10);

    auto slot3Lable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 20);
    slot3Lable->setColor(Color3B::GRAY);
    slot3Lable->setName("Slot3Label");
    //slot3Lable->setRotation(180);
    slot3Lable->setScale(0.05 * Director::getInstance()->getContentScaleFactor());
    slot3Lable->setPosition(Vec2(slot3->getContentSize().width / 2, slot3->getContentSize().height / 2));
    slot3Lable->setString(StringUtils::format("%s", "Slot 3"));
    slot3->addChild(slot3Lable, 10);

    auto slot2Lable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 20);
    slot2Lable->setColor(Color3B::GRAY);
    slot2Lable->setName("Slot2Label");
    //slot2Lable->setRotation(180);
    slot2Lable->setScale(0.05 * Director::getInstance()->getContentScaleFactor());
    slot2Lable->setPosition(Vec2(slot2->getContentSize().width / 2, slot2->getContentSize().height / 2));
    slot2Lable->setString(StringUtils::format("%s", "Slot 2"));
    slot2->addChild(slot2Lable, 10);

    auto slot1Lable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 20);
    slot1Lable->setColor(Color3B::GRAY);
    slot1Lable->setName("Slot1Label");
    //slot1Lable->setRotation(180);
    slot1Lable->setScale(0.05 * Director::getInstance()->getContentScaleFactor());
    slot1Lable->setPosition(Vec2(slot1->getContentSize().width / 2, slot1->getContentSize().height / 2));
    slot1Lable->setString(StringUtils::format("%s", "Slot 1"));
    slot1->addChild(slot1Lable, 10);

    hideLabels.push_back(slot1Lable);
    hideLabels.push_back(slot2Lable);
    hideLabels.push_back(slot3Lable);
    hideLabels.push_back(slot4Lable);

    // UNLOCK and LEVELUP
    auto unlock = Sprite::create("res/papper_container2.png");
    unlock->setName("Unlock");
    unlock->setScale(0.08 * scaleFactor);
    unlock->setRotation(180);
    unlock->setPosition(Vec2(95 / scaleFactor, 233.33 / scaleFactor));
    skillDetail->addChild(unlock);
    this->unlockButton = unlock;

    //Check for show unlock or not
    bool isShowUnlock = false;

    //Assign skill parent to a skill in list Skill
    for (auto i : listOfSkills)
    {
        if (i->skillName == skillname)
        {
            skill->getSkillTalent()->parent = i->parent;
            break;
        }
    }
    //Check player has been unlocked a parent skill of this skill
    for (auto i : player->getPlayerSkills())
    {
        if (skill->getSkillTalent()->parent == nullptr)
            break;
        if (i->getSkillName() == skill->getSkillTalent()->parent->skillName)
        {
            skill->getSkillTalent()->parent->isLock = false;
            break;
        }
    }
    //
    if (player->getExpToConsume() >= skill->getSkillTalent()->unlockPoint && player->getLevel() >= skill->getSkillTalent()->levelRequired
        && !player->isSkillBeUnlock(skillname) && ((skill->getSkillTalent()->parent
            && !skill->getSkillTalent()->parent->isLock) || skill->getSkillTalent()->parent == nullptr))isShowUnlock = true;


    /////////////////////////////

    auto upgrade = Sprite::create("res/papper_container2.png");
    upgrade->setName("Upgrade");
    upgrade->setScale(0.08 * scaleFactor);
    upgrade->setRotation(180);
    upgrade->setPosition(Vec2(335 / scaleFactor, 233.33 / scaleFactor));
    skillDetail->addChild(upgrade);
    this->upgradeButton = upgrade;

    bool isShowUpgrade = false;
    if (player->getExpToConsume() >= skill->getSkillTalent()->upgradePoint && player->isSkillBeUnlock(skillname))
    {

        for (auto skillPlayer : player->getPlayerSkills()) {
            if (skillPlayer->getSkillName() == skillname && skillPlayer->getCurrentLevelSkill() < 5) {
                isShowUpgrade = true;
                break;
            }
        }

    }

    auto unlockLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 35);
    unlockLable->setColor(Color3B::WHITE);
    // unlockLable->setRotation(180);
    unlockLable->setScale(1 * Director::getInstance()->getContentScaleFactor());
    unlockLable->setPosition(Vec2(unlock->getContentSize().width / 2, unlock->getContentSize().height / 2));
    unlockLable->setString(StringUtils::format("%s", "Unlock"));
    unlock->addChild(unlockLable);

    auto upgradeLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 35);
    upgradeLable->setColor(Color3B::GRAY);
    // upgradeLable->setRotation(180);
    upgradeLable->setScale(1 * Director::getInstance()->getContentScaleFactor());
    upgradeLable->setPosition(Vec2(upgrade->getContentSize().width / 2, upgrade->getContentSize().height / 2));
    upgradeLable->setString(StringUtils::format("%s", "Upgrade"));
    upgrade->addChild(upgradeLable);


    // Set visible va su kien cham cho cac nut theo tung truong hop
    if (!isSkillUnlocked)
    {
        slot1->setVisible(false);
        slot1Lable->setVisible(false);
        slot2->setVisible(false);
        slot2Lable->setVisible(false);
        slot3->setVisible(false);
        slot3Lable->setVisible(false);
        slot4->setVisible(false);
        slot4Lable->setVisible(false);

        attackButton->setVisible(false);
        cancelEquip->setVisible(false);

        upgrade->setVisible(false);
        upgradeLable->setVisible(false);
        //if (isShowUnlock) {
        unlock->setVisible(true);
        unlockLable->setVisible(true);
        //}
        //else {
        //    unlock->setVisible(false);
        //    unlockLable->setVisible(false);
        //}
        upgradePointRequiredLable->setVisible(false);

        if (isShowUnlock) {
            unlock->setVisible(true);
            unlockLable->setVisible(true);
        }
        else {
            unlock->setVisible(false);
            unlockLable->setVisible(false);
        }
        //if (skillSlot1)
        //{
        //    skillSlot1->setVisible(false);
        //}
        //if (skillSlot2)
        //{
        //    skillSlot2->setVisible(false);
        //}
        //if (skillSlot3)
        //{
        //    skillSlot3->setVisible(false);
        //}
        //if (skillSlot4)
        //{
        //    skillSlot4->setVisible(false);
        //}
    }
    if (isSkillUnlocked)
    {
        currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
        if (currentTutorial == 6) {
            TutorialManager::getInstance()->nextTutorial();
        }
        slot1->setVisible(false);
        slot1Lable->setVisible(false);
        slot2->setVisible(false);
        slot2Lable->setVisible(false);
        slot3->setVisible(false);
        slot3Lable->setVisible(false);
        slot4->setVisible(false);
        slot4Lable->setVisible(false);
        attackButton->setVisible(false);
        unlock->setVisible(false);
        unlockLable->setVisible(false);
        unlockPointRequiredLable->setVisible(false);
        cancelEquip->setVisible(false);
        if (isShowUpgrade) {
            upgrade->setVisible(true);
            upgradeLable->setVisible(true);
        }
        else {
            upgrade->setVisible(false);
            upgradeLable->setVisible(false);
        }
        //if (skillSlot1)
        //{
        //    skillSlot1->setVisible(false);
        //}
        //if (skillSlot2)
        //{
        //    skillSlot2->setVisible(false);
        //}
        //if (skillSlot3)
        //{
        //    skillSlot3->setVisible(false);
        //}
        //if (skillSlot4)
        //{
        //    skillSlot4->setVisible(false);
        //}
    }

    // S kin chm vo nt Equip (Tutorials Listener)
    touchListenerEquip = EventListenerTouchOneByOne::create();
    touchListenerEquip->setSwallowTouches(true);
    touchListenerEquip->onTouchBegan = [this, player, scale, slot1, slot2, slot3, slot4, cancelEquip, attackButton/*, skillSlot1, skillSlot2, skillSlot3, skillSlot4*/](Touch* touch, Event* event)
        {
            if (isVisible() && skillDetail->isVisible() && getParent()->isVisible())
            {
                auto target = static_cast<Sprite*>(event->getCurrentTarget());
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                Size s = target->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);
                if (rect.containsPoint(locationInNode))
                {

                    InGameUI::getInstance(player)->hamburgerButton->setVisible(false);
                    InGameUI::getInstance(player)->hamburgerButton->btnSprite->setVisible(false);

                    InGameUI::getInstance(player)->questButton->setVisible(false);
                    InGameUI::getInstance(player)->questButton->btnSprite->setVisible(false);

                    InGameUI::getInstance(player)->questManager->currentQuest->setVisible(false);
                    InGameUI::getInstance(player)->questManager->noQuestLabel->setVisible(false);
                    InGameUI::getInstance(player)->questManager->questManagerUI->setVisible(false);

                    InGameUI::getInstance(player)->talentButton->setVisible(false);
                    InGameUI::getInstance(player)->talentButton->btnSprite->setVisible(false);

                    InGameUI::getInstance(player)->inventoryButton->setVisible(false);
                    InGameUI::getInstance(player)->inventoryButton->btnSprite->setVisible(false);
                    player->setVisibleSkillButton(false);

                    InGameUI::getInstance(player)->healthBar->setVisible(false);
                    InGameUI::getInstance(player)->expBar->setVisible(false);
                    InGameUI::getInstance(player)->subInventory->setVisible(false);

                    InGameUI::getInstance(player)->getMovementJoystick()->isCanPress = false;
                    this->getParent()->setVisible(false);
                    int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
                    if (currentTutorial == 7) {
                        TutorialManager::getInstance()->nextTutorial();
                    }
                    SkillTree::hideComponents();

                    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

                    cancelEquip->setVisible(true);
                    attackButton->setVisible(true);

                    if (slot1)
                    {

                        slot1->setVisible(true);
                        //slot1->setPosition(Vec2(scale , 3 * scale));
                        if (skillDetail->getChildByName("Slot1Label"))
                            skillDetail->removeChildByName("Slot1Label");
                        auto slot1Label = Label::createWithTTF("Slot 1", "fonts/Diablo Light.ttf", 20);
                        slot1Label->setColor(Color3B::WHITE);
                        // slot1Label->setRotation(180);
                        slot1Label->setName("Slot1Label");
                        slot1Label->setScale(1 * Director::getInstance()->getContentScaleFactor());
                        slot1Label->setPosition(Vec2(slot1->getContentSize().width / 2, slot1->getContentSize().height / 2));
                        slot1->addChild(slot1Label);
                        slot1Label->setVisible(true);
                        /*if (skillSlot1)
                        {
                            skillSlot1->setVisible(true);
                            skillSlot1->setPosition(Vec2(slot1->getContentSize().width / 2, slot1->getContentSize().height / 2));
                        }*/
                    }
                    if (slot2)
                    {
                        slot2->setVisible(true);
                        //slot2->setPosition(Vec2(scale, 13 * scale));
                        if (skillDetail->getChildByName("Slot2Label"))
                            skillDetail->removeChildByName("Slot2Label");
                        auto slot2Label = Label::createWithTTF("Slot 2", "fonts/Diablo Light.ttf", 20);
                        slot2Label->setColor(Color3B::WHITE);
                        //slot2Label->setRotation(180);
                        slot2Label->setName("Slot2Label");
                        slot2Label->setScale(1 * Director::getInstance()->getContentScaleFactor());
                        slot2Label->setPosition(Vec2(slot2->getContentSize().width / 2, slot2->getContentSize().height / 2));
                        slot2->addChild(slot2Label);
                        slot2Label->setVisible(true);
                        /*if (skillSlot2)
                        {
                            skillSlot2->setVisible(true);
                            skillSlot2->setPosition(Vec2(slot2->getContentSize().width / 2, slot2->getContentSize().height / 2));
                        }*/
                    }
                    if (slot3)
                    {
                        slot3->setVisible(true);
                        //slot3->setPosition(Vec2(scale , 23 * scale));
                        if (skillDetail->getChildByName("Slot3Label"))
                            skillDetail->removeChildByName("Slot3Label");
                        auto slot3Label = Label::createWithTTF("Slot 3", "fonts/Diablo Light.ttf", 20);
                        slot3Label->setColor(Color3B::WHITE);
                        //slot3Label->setRotation(180);
                        slot3Label->setName("Slot3Label");
                        slot3Label->setScale(1 * Director::getInstance()->getContentScaleFactor());
                        slot3Label->setPosition(Vec2(slot3->getContentSize().width / 2, slot3->getContentSize().height / 2));
                        slot3->addChild(slot3Label);
                        slot3Label->setVisible(true);
                        /*if (skillSlot3)
                        {
                            skillSlot3->setVisible(true);
                            skillSlot3->setPosition(Vec2(slot3->getContentSize().width / 2, slot3->getContentSize().height / 2));
                        }*/
                    }
                    if (slot4)
                    {
                        slot4->setVisible(true);
                        //slot4->setPosition(Vec2(scale , 33 * scale));
                        if (skillDetail->getChildByName("Slot4Label"))
                            skillDetail->removeChildByName("Slot4Label");
                        auto slot4Label = Label::createWithTTF("Slot 4", "fonts/Diablo Light.ttf", 20);
                        slot4Label->setColor(Color3B::WHITE);
                        //slot4Label->setRotation(180);
                        slot4Label->setName("Slot4Label");
                        slot4Label->setScale(1 * Director::getInstance()->getContentScaleFactor());
                        slot4Label->setPosition(Vec2(slot4->getContentSize().width / 2, slot4->getContentSize().height / 2));
                        slot4->addChild(slot4Label);
                        slot4Label->setVisible(true);
                        /*if (skillSlot4)
                        {
                            skillSlot4->setVisible(true);
                            skillSlot4->setPosition(Vec2(slot4->getContentSize().width / 2, slot4->getContentSize().height / 2));
                        }*/
                    }

                    return true;
                }
                return false;
            }
            return false;
        };

    // Thm s kin cho nt Equip
    if (isSkillUnlocked)
    {
        equip->setVisible(true);
        equipLabel->setVisible(true);
        equip->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListenerEquip, equip);
    }
    else
    {
        equip->setVisible(false);
        equipLabel->setVisible(false);
    }

    touchListenerCancelEquip = EventListenerTouchOneByOne::create();
    touchListenerCancelEquip->setSwallowTouches(true);
    touchListenerCancelEquip->onTouchBegan = [player, skillname, this, slot1, slot2, slot3, slot4, cancelEquip, attackButton](Touch* touch, Event* event)
        {
            if (cancelEquip->isVisible())
            {
                Vec2 touchLocationInNode = InGameUI::getInstance(player)->convertToNodeSpace(touch->getLocation());
                if (cancelEquip->getBoundingBox().containsPoint(touchLocationInNode))
                {
                    auto skill = SkillFactory::createSkill(skillname);
                    if (cancelEquip->getName() == "CancelEquip")
                    {
                        CCLOG("Touch to CancelEquip");
                        SkillTree::showComponents();
                        cancelEquip->setVisible(false);
                        attackButton->setVisible(false);
                        slot1->setVisible(false);
                        slot2->setVisible(false);
                        slot3->setVisible(false);
                        slot4->setVisible(false);
                        this->getParent()->setVisible(true);

                        InGameUI::getInstance(player)->hamburgerButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->questManager->currentQuest->setVisible(true);
                        InGameUI::getInstance(player)->questManager->noQuestLabel->setVisible(true);
                        InGameUI::getInstance(player)->questManager->questManagerUI->setVisible(true);
                        InGameUI::getInstance(player)->questManager->generalDisplay();
                        InGameUI::getInstance(player)->talentButton->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->setVisible(true);

                        //
                        InGameUI::getInstance(player)->hamburgerButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->btnSprite->setVisible(true);

                        InGameUI::getInstance(player)->getMovementJoystick()->isCanPress = true;
                    }
                    return true; //  x l s kin
                }
            }
            return false;
        };
    if (isSkillUnlocked)
        cancelEquip->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListenerCancelEquip, cancelEquip);
    // Slot1 (Tutorials Listener)
    touchListenerAttack = EventListenerTouchOneByOne::create();
    touchListenerAttack->setSwallowTouches(true);
    touchListenerAttack->onTouchBegan = [player, skillname, this, slot1, slot2, slot3, slot4, cancelEquip, attackButton](Touch* touch, Event* event)
        {
            if (cancelEquip->isVisible())
            {
                Vec2 touchLocationInNode = InGameUI::getInstance(player)->convertToNodeSpace(touch->getLocation());
                if (attackButton->getBoundingBox().containsPoint(touchLocationInNode))
                {
                    if (attackButton->getName() == "AttackButton")
                    {
                        
                    }
                    return true; //  x l s kin
                }
            }
            return false;
        };
    if (isSkillUnlocked)
        attackButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListenerAttack, attackButton);

    // Slot1 (Tutorials Listener)
    touchListener1 = EventListenerTouchOneByOne::create();
    touchListener1->setSwallowTouches(true);
    touchListener1->onTouchBegan = [player, skillname, this, slot1, slot2, slot3, slot4, cancelEquip, attackButton](Touch* touch, Event* event)
        {
            if (slot1->isVisible())
            {
                Vec2 touchLocationInNode = InGameUI::getInstance(player)->convertToNodeSpace(touch->getLocation());
                if (slot1->getBoundingBox().containsPoint(touchLocationInNode))
                {
                    auto skill = SkillFactory::createSkill(skillname);
                    if (slot1->getName() == "Slot1")
                    {
                        int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
                        if (currentTutorial == 8) {
                            TutorialManager::getInstance()->hideTutorial();
                        }
                        CCLOG("Touch to Slol1");

                        SkillTree::showComponents();
                        // Lay ra vi tri cua skill dang thao tac
                        int currentPos = 0;
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillTalent()->skillName == skillname)
                            {
                                currentPos = skill->getSkillIndex();
                                break;
                            }
                        }
                        // Dat skill da thay the vao vi tri cu cua skill dang thao tac
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillIndex() == 1)
                            {
                                skill->setSkillIndex(currentPos);
                                if (currentPos == 0) {
                                    if (this->getChildByName("SkillButton1")) {
                                        this->getChildByName("SkillButton1")->removeFromParentAndCleanup(true);
                                        this->removeChildByName("SkillButton1");
                                    }
                                }
                                break;
                            }
                        }
                        // Dat skill dang thao tac vao vi tri 1
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillTalent()->skillName == skillname)
                            {
                                skill->setSkillIndex(1);
                                break;
                            }
                        }
                        //Alert
                        showAlert("Equiped " + skillname + " to Slot1", true);


                        player->resetSkillSlotOrder();
                        
                        cancelEquip->setVisible(false);
                        attackButton->setVisible(false);
                        slot1->setVisible(false);
                        slot2->setVisible(false);
                        slot3->setVisible(false);
                        slot4->setVisible(false);
                        this->getParent()->setVisible(true);

                        InGameUI::getInstance(player)->hamburgerButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->setVisible(true);
                        InGameUI::getInstance(player)->questManager->currentQuest->setVisible(true);
                        InGameUI::getInstance(player)->questManager->noQuestLabel->setVisible(true);
                        InGameUI::getInstance(player)->questManager->questManagerUI->setVisible(true);
                        InGameUI::getInstance(player)->questManager->generalDisplay();

                        InGameUI::getInstance(player)->healthBar->setVisible(true);
                        InGameUI::getInstance(player)->expBar->setVisible(true);
                        InGameUI::getInstance(player)->subInventory->setVisible(true);

                        //
                        InGameUI::getInstance(player)->hamburgerButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->btnSprite->setVisible(true);

                        InGameUI::getInstance(player)->getMovementJoystick()->isCanPress = true;
                    }
                    return true; //  x l s kin
                }
            }
            return false;
        };
    if (isSkillUnlocked)
        slot1->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener1, slot1);
    _touchListenerSlot1 = touchListener1;

    touchListener2 = EventListenerTouchOneByOne::create();
    touchListener2->setSwallowTouches(true);
    touchListener2->onTouchBegan = [player, skillname, this, slot1, slot2, slot3, slot4, cancelEquip, attackButton](Touch* touch, Event* event)
        {
            if (slot2->isVisible())
            {

                Vec2 touchLocationInNode = InGameUI::getInstance(player)->convertToNodeSpace(touch->getLocation());
                if (slot2->getBoundingBox().containsPoint(touchLocationInNode))
                {
                    auto skill = SkillFactory::createSkill(skillname);
                    if (slot2->getName() == "Slot2")
                    {
                        int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
                        if (currentTutorial == 8) {
                            TutorialManager::getInstance()->hideTutorial();
                        }
                        CCLOG("Touch to Slot2");
                        SkillTree::showComponents();
                        // Lay ra vi tri cua skill dang thao tac
                        int currentPos = 0;
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillTalent()->skillName == skillname)
                            {
                                currentPos = skill->getSkillIndex();
                                break;
                            }
                        }
                        // Dat skill da thay the vao vi tri cu cua skill dang thao tac
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillIndex() == 2)
                            {
                                skill->setSkillIndex(currentPos);
                                if (currentPos == 0) {

                                    if (this->getChildByName("SkillButton2")) {
                                        this->getChildByName("SkillButton2")->removeFromParentAndCleanup(true);
                                        this->removeChildByName("SkillButton2");
                                    }
                                }
                                break;
                            }
                        }
                        // Dat skill dang thao tac vao vi tri 1
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillTalent()->skillName == skillname)
                            {
                                skill->setSkillIndex(2);
                                break;
                            }
                        }
                        //Alert
                        showAlert("Equiped " + skillname + " to Slot2", true);
                        player->resetSkillSlotOrder();
                        cancelEquip->setVisible(false);
                        attackButton->setVisible(false);
                        slot1->setVisible(false);
                        slot2->setVisible(false);
                        slot3->setVisible(false);
                        slot4->setVisible(false);
                        this->getParent()->setVisible(true);

                        InGameUI::getInstance(player)->hamburgerButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->setVisible(true);
                        InGameUI::getInstance(player)->questManager->currentQuest->setVisible(true);
                        InGameUI::getInstance(player)->questManager->noQuestLabel->setVisible(true);
                        InGameUI::getInstance(player)->questManager->questManagerUI->setVisible(true);
                        InGameUI::getInstance(player)->questManager->generalDisplay();

                        InGameUI::getInstance(player)->healthBar->setVisible(true);
                        InGameUI::getInstance(player)->expBar->setVisible(true);
                        InGameUI::getInstance(player)->subInventory->setVisible(true);

                        //
                        InGameUI::getInstance(player)->hamburgerButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->btnSprite->setVisible(true);

                        InGameUI::getInstance(player)->getMovementJoystick()->isCanPress = true;
                    }
                    return true; //  x l s kin
                }
            }
            return false; // Cha x l s kin
        };
    if (isSkillUnlocked)
        slot2->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener2, slot2);
    _touchListenerSlot2 = touchListener2;

    touchListener3 = EventListenerTouchOneByOne::create();
    touchListener3->setSwallowTouches(true);
    touchListener3->onTouchBegan = [player, skillname, this, slot1, slot2, slot3, slot4, cancelEquip, attackButton](Touch* touch, Event* event)
        {
            if (slot3->isVisible())
            {
                Vec2 touchLocationInNode = InGameUI::getInstance(player)->convertToNodeSpace(touch->getLocation());
                if (slot3->getBoundingBox().containsPoint(touchLocationInNode))
                {
                    if (slot3->getName() == "Slot3")
                    {
                        int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
                        if (currentTutorial == 8) {
                            TutorialManager::getInstance()->hideTutorial();
                        }
                        auto skill = SkillFactory::createSkill(skillname);
                        CCLOG("Touch to Slot3");
                        SkillTree::showComponents();
                        // Lay ra vi tri cua skill dang thao tac
                        int currentPos = 0;
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillTalent()->skillName == skillname)
                            {
                                currentPos = skill->getSkillIndex();
                                break;
                            }
                        }
                        // Dat skill da thay the vao vi tri cu cua skill dang thao tac
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillIndex() == 3)
                            {
                                skill->setSkillIndex(currentPos);
                                if (currentPos == 0) {
                                    if (this->getChildByName("SkillButton3")) {
                                        this->removeChildByName("SkillButton3");
                                        this->getChildByName("SkillButton3")->removeFromParentAndCleanup(true);
                                    }

                                }
                                break;
                            }
                        }
                        // Dat skill dang thao tac vao vi tri 3
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillTalent()->skillName == skillname)
                            {
                                skill->setSkillIndex(3);
                                break;
                            }
                        }
                        //Alert
                        showAlert("Equiped " + skillname + " to Slot3", true);
                        player->resetSkillSlotOrder();
                        cancelEquip->setVisible(false);
                        attackButton->setVisible(false);
                        slot1->setVisible(false);
                        slot2->setVisible(false);
                        slot3->setVisible(false);
                        slot4->setVisible(false);
                        this->getParent()->setVisible(true);

                        InGameUI::getInstance(player)->hamburgerButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->setVisible(true);
                        InGameUI::getInstance(player)->questManager->currentQuest->setVisible(true);
                        InGameUI::getInstance(player)->questManager->noQuestLabel->setVisible(true);
                        InGameUI::getInstance(player)->questManager->questManagerUI->setVisible(true);
                        InGameUI::getInstance(player)->questManager->generalDisplay();

                        InGameUI::getInstance(player)->healthBar->setVisible(true);
                        InGameUI::getInstance(player)->expBar->setVisible(true);
                        InGameUI::getInstance(player)->subInventory->setVisible(true);

                        //
                        InGameUI::getInstance(player)->hamburgerButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->btnSprite->setVisible(true);

                        InGameUI::getInstance(player)->getMovementJoystick()->isCanPress = true;
                    }
                    return true; //  x l s kin
                }
            }
            return false; // Cha x l s kin
        };
    if (isSkillUnlocked)
        slot3->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener3, slot3);
    _touchListenerSlot3 = touchListener3;

    touchListener4 = EventListenerTouchOneByOne::create();
    touchListener4->setSwallowTouches(true);
    touchListener4->onTouchBegan = [player, skillname, this, slot1, slot2, slot3, slot4, cancelEquip, attackButton](Touch* touch, Event* event)
        {
            if (slot4->isVisible())
            {
                auto target = static_cast<Sprite*>(event->getCurrentTarget());
                // Ly v tr ca chm tng i vi sprite
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                // Ly kch thc ca sprite
                Size s = target->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);
                // Kim tra xem chm c nm trong sprite khng
                if (rect.containsPoint(locationInNode))
                {
                    auto skill = SkillFactory::createSkill(skillname);
                    if (target->getName() == "Slot4")
                    {
                        int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
                        if (currentTutorial == 8) {
                            TutorialManager::getInstance()->hideTutorial();
                        }
                        CCLOG("Touch to Slot4");
                        SkillTree::showComponents();
                        // Lay ra vi tri cua skill dang thao tac
                        int currentPos = 0;
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillTalent()->skillName == skillname)
                            {
                                currentPos = skill->getSkillIndex();
                                break;
                            }
                        }
                        // Dat skill da thay the vao vi tri cu cua skill dang thao tac
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillIndex() == 4)
                            {
                                skill->setSkillIndex(currentPos);
                                if (currentPos == 0) {
                                    if (this->getChildByName("SkillButton4")) {
                                        this->getChildByName("SkillButton4")->removeFromParentAndCleanup(true);
                                        this->removeChildByName("SkillButton4");
                                    }
                                }
                                break;
                            }
                        }
                        // Dat skill dang thao tac vao vi tri 1
                        for (auto skill : player->getPlayerSkills())
                        {
                            if (skill->getSkillTalent()->skillName == skillname)
                            {
                                skill->setSkillIndex(4);
                                break;
                            }
                        }
                        //Alert
                        showAlert("Equiped " + skillname + " to Slot4", true);
                        player->resetSkillSlotOrder();
                        cancelEquip->setVisible(false);
                        attackButton->setVisible(false);
                        slot1->setVisible(false);
                        slot2->setVisible(false);
                        slot3->setVisible(false);
                        slot4->setVisible(false);
                        this->getParent()->setVisible(true);

                        InGameUI::getInstance(player)->hamburgerButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->setVisible(true);
                        InGameUI::getInstance(player)->questButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->setVisible(true);
                        InGameUI::getInstance(player)->questManager->currentQuest->setVisible(true);
                        InGameUI::getInstance(player)->questManager->noQuestLabel->setVisible(true);
                        InGameUI::getInstance(player)->questManager->questManagerUI->setVisible(true);
                        InGameUI::getInstance(player)->questManager->generalDisplay();


                        InGameUI::getInstance(player)->healthBar->setVisible(true);
                        InGameUI::getInstance(player)->expBar->setVisible(true);
                        InGameUI::getInstance(player)->subInventory->setVisible(true);

                        //
                        InGameUI::getInstance(player)->hamburgerButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->talentButton->btnSprite->setVisible(true);
                        InGameUI::getInstance(player)->inventoryButton->btnSprite->setVisible(true);

                        InGameUI::getInstance(player)->getMovementJoystick()->isCanPress = true;
                    }
                    return true; //  x l s kin
                }
            }
            return false; // Cha x l s kin
        };
    if (isSkillUnlocked)
        slot4->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener4, slot4);
    _touchListenerSlot4 = touchListener4;

    //Unlock (Tutorials Listener)
    touchListener5 = EventListenerTouchOneByOne::create();
    touchListener5->setSwallowTouches(true);
    touchListener5->onTouchBegan = [skillname, unlock, this, player](Touch* touch, Event* event)
        {
            if (this->isVisible() && skillDetail->isVisible() && unlock->isVisible())
            {

                auto target = static_cast<Sprite*>(event->getCurrentTarget());

                // Ly v tr ca chm tng i vi sprite
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                // Ly kch thc ca sprite
                Size s = target->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);

                // Kim tra xem chm c nm trong sprite khng
                if (rect.containsPoint(locationInNode))
                {
                    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                    auto skill = SkillFactory::createSkill(skillname);
                    for (auto i : listOfSkills)
                    {
                        if (i->skillName == skillname)
                        {
                            skill->getSkillTalent()->parent = i->parent;
                            break;
                        }
                    }
                    for (auto i : player->getPlayerSkills())
                    {
                        if (skill->getSkillTalent()->parent == nullptr)
                            break;
                        if (i->getSkillName() == skill->getSkillTalent()->parent->skillName)
                        {
                            skill->getSkillTalent()->parent->isLock = false;
                            break;
                        }
                    }
                    if (target->getName() == "Unlock")
                    {
                        CCLOG("Touch to Unlock");
                        int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
                        if (currentTutorial == 6) {
                            TutorialManager::getInstance()->nextTutorial();
                        }
                        if (player->getExpToConsume() >= skill->getSkillTalent()->unlockPoint && player->getLevel() >= skill->getSkillTalent()->levelRequired && !player->isSkillBeUnlock(skillname) && ((skill->getSkillTalent()->parent && !skill->getSkillTalent()->parent->isLock) || skill->getSkillTalent()->parent == nullptr))
                        {
                            ////Co yes/no
                            //ko yes/No
                            player->decreaseExpToConsume(skill->getSkillTalent()->unlockPoint);
                            player->addSkill(skillname);
                            player->resetSkillSlotOrder();
                            this->showAlert("Unlock completed!", true);
                            _progressBar->updateProgress(player->getExpToConsume(), player->getAllExp());
                            this->getChildByName(skillname + " Node")->getChildByName(skillname + " Sprite")->removeChildByName("Black Node");
                            auto levelLabel = this->getChildByName(skillname + " Node")->getChildByName(skillname + " LevelSkill");
                            if (levelLabel)
                            {
                                auto label = dynamic_cast<Label*>(levelLabel);
                                if (label)
                                {
                                    label->setString("Lv1");
                                }
                            }
                            this->showSkillDetail(skill, player);

                        }
                        else
                        {
                            this->showAlert("Unlock failed!", false);
                        }
                    }
                    return true; //  x l s kin
                }
            }
            return false; // Cha x l s kin
        };
    if (!isSkillUnlocked)
        unlock->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener5, unlock);
    else
        unlock->getEventDispatcher()->removeEventListener(touchListener5);
    touchListener6 = EventListenerTouchOneByOne::create();
    touchListener6->setSwallowTouches(true);
    touchListener6->onTouchBegan = [skillname, this, player, upgrade](Touch* touch, Event* event)
        {
            if (isVisible() && skillDetail->isVisible() && upgrade->isVisible() && getParent()->isVisible())
            {
                auto target = static_cast<Sprite*>(event->getCurrentTarget());
                // Ly v tr ca chm tng i vi sprite
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                // Ly kch thc ca sprite
                Size s = target->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);

                SkillBase* skillOfPlayer = nullptr;
                for (auto skillPlayer : player->getPlayerSkills()) {
                    if (skillPlayer->getSkillName() == skillname) {
                        skillOfPlayer = skillPlayer;
                        break;
                    }
                }
                // Kim tra xem chm c nm trong sprite khng
                if (rect.containsPoint(locationInNode))
                {
                    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                    if (target->getName() == "Upgrade")
                    {
                        auto skill = SkillFactory::createSkill(skillname);

                        if (player->getExpToConsume() >= skill->getSkillTalent()->upgradePoint && player->isSkillBeUnlock(skillname))
                        {
                            ////co yes/no
                            //else this->showAlert("This Skill has reached the maximum level!", false);
                            //ko yes no
                            if (skillOfPlayer->getCurrentLevelSkill() < 5) {
                                for (auto skill : player->getPlayerSkills())
                                {
                                    if (skill->getSkillTalent()->skillName == skillname)
                                    {
                                        bool isLevelUp = skill->levelUpSkill();
                                        if (isLevelUp)
                                            player->decreaseExpToConsume(skill->getSkillTalent()->upgradePoint);
                                        moneyLabel->setString(StringUtils::format("Skill Point: %d", player->getSkillPoint()));
                                        _progressBar->updateProgress(player->getExpToConsume(), player->getAllExp());
                                        this->showAlert("Upgrade completed!", true);

                                        auto levelLabel = this->getChildByName(skillname + " Node")->getChildByName(skillname + " LevelSkill");
                                        if (levelLabel)
                                        {
                                            
                                            auto label = dynamic_cast<Label*>(levelLabel);
                                            if (label)
                                            {
                                                if (skill->getCurrentLevelSkill() == 5) {
                                                    label->setBMFontSize(25);
                                                    label->setString("Max");
                                                }
                                                else
                                                label->setString("Lv"+std::to_string(skill->getCurrentLevelSkill()));
                                            }
                                        }
                                        this->showSkillDetail(skill, player);
                                    }
                                }
                            }
                            else this->showAlert("This Skill has reached the maximum level!", false);

                        }
                        else
                        {
                            this->showAlert("Upgrade failed!", false);
                        }
                    }
                    return true; //  x l s kin
                }
            }
            return false; // Cha x l s kin
        };
    if (isSkillUnlocked)
        upgrade->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener6, upgrade);

    //outsideSkillDetailListener = EventListenerTouchOneByOne::create();
    //outsideSkillDetailListener->setSwallowTouches(true);
    //outsideSkillDetailListener->onTouchBegan = [this](Touch* touch, Event* event)
    //    {
    //        if (this->skillDetail->isVisible() && this->isVisible())
    //        {
    //            int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
    //            if (UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) > 5 && UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) < 8) {
    //                TutorialManager::getInstance()->showTutorial(5);
    //            }
    //            auto target = static_cast<Sprite*>(event->getCurrentTarget());
    //            // Ly v tr ca chm tng i vi sprite
    //            Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
    //            // Ly kch thc ca sprite
    //            Size s = target->getContentSize();
    //            Rect rect = Rect(0, 0, s.width, s.height);

    //            // Kim tra xem chm c nm trong sprite khng
    //            if (rect.containsPoint(locationInNode))
    //            {
    //                if (target->getName() == "SkillDetail")
    //                {

    //                }
    //            }
    //            else
    //            {
    //                this->skillDetail->setVisible(false);
    //                if (_overlay) _overlay->setVisible(false);
    //                TutorialManager::getInstance()->hideTutorial();
    //            }
    //            return true; //  x l s kin
    //        }
    //        return false; // Cha x l s kin
    //    };
    //skillDetail->getEventDispatcher()->addEventListenerWithSceneGraphPriority(outsideSkillDetailListener, skillDetail);
}

void SkillTree::hideComponents() {
    _talentCarousel->hideComponents();
    for (auto sprite : listSkillDetails) {
        sprite->setVisible(false);
    }
    for (auto sprite : treeDrawed) {
        sprite->setVisible(false);
    }
    for (auto sprite : treeDrawedNode) {
        sprite->setVisible(false);
    }
    for (auto sprite : hideLabels) {
        sprite->setVisible(false);
    }
    /*for (auto sprite : listSlots) {
        sprite->setVisible(true);
    }*/
}
void SkillTree::showComponents() {
    _talentCarousel->showComponents();
    for (auto sprite : listSkillDetails) {
        sprite->setVisible(true);
    }
    for (auto sprite : treeDrawed) {
        sprite->setVisible(true);
    }
    for (auto sprite : treeDrawedNode) {
        sprite->setVisible(true);
    }
    for (auto sprite : hideLabels) {
        sprite->setVisible(true);
    }
    /*for (auto sprite : listSlots) {
        sprite->setVisible(false);
    }*/
}
void SkillTree::showAlert(std::string alert, bool isSuccess)
{
    if (isSuccess)
        NotificationManager::getInstance()->showMessageNotification(alert, Vec2::ZERO, Color3B(161, 251, 142), 17);
    else
        NotificationManager::getInstance()->showMessageNotification(alert, Vec2::ZERO, Color3B(251, 67, 53), 17);
}

SkillTree::~SkillTree()
{
}