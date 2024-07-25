#include "./PotionManager.h"
#include "./ItemFactory.h"

bool PotionManager::init()
{
    if (!Node::init())
    {
        return false;
    }

    schedule(CC_SCHEDULE_SELECTOR(PotionManager::updateCooldown), 1.0f);
    potionNode = Node::create();
    this->addChild(potionNode);
    return true;
}


bool PotionManager::checkPotion(std::string potionPath) {
    for (auto i : listPotionNodes) {
        if (i && i->getChildByName(potionPath) && dynamic_cast<Sprite*>(i->getChildByName(potionPath))->getTexture()->getPath().find(potionPath) != std::string::npos) {
            return false;
        }
    }
    return true;
}

bool PotionManager::addPotion(std::string potionPath, int potionCooldown) {
    for (auto i : listPotionNodes) {
        if (i && i->getChildByName(potionPath) && dynamic_cast<Sprite*>(i->getChildByName(potionPath))->getTexture()->getPath().find(potionPath) != std::string::npos) {
            return false;
        }
    }
    Node* newPotionNode = Node::create();
    if (newPotionNode) {
        Sprite* potionSprite = Sprite::create(potionPath);
        if (potionSprite) {
            newPotionNode->setPosition(Vec2(0, -30 * (static_cast<int>(listPotionNodes.size()) + 1)));
            potionSprite->setScale(2 * Director::getInstance()->getContentScaleFactor());
            potionSprite->setName(potionPath);
            newPotionNode->setName(potionPath);
            newPotionNode->addChild(potionSprite);
        }
        Label* currentCooldown = Label::createWithTTF(std::to_string(potionCooldown), "fonts/Diablo Light.ttf", 15);
        if (currentCooldown) {
            currentCooldown->setName("label");
            newPotionNode->addChild(currentCooldown);
        }
        potionNode->addChild(newPotionNode);
        listPotionNodes.push_back(newPotionNode);
    }
    return true;
}

bool PotionManager::isActiveItem(std::string potionPath) {
    for (auto i : listPotionNodes) {
        if (i->getName() == potionPath) {
            return true;
        }
    }
    return false;
}

void PotionManager::updateCooldown(float dt) {
    if (!listPotionNodes.empty()) {
        for (auto i : listPotionNodes) {
            Label* label = static_cast<Label*>(i->getChildByName("label"));
            if (label != nullptr) {
                int timeCalced = std::stoi(label->getString()) - 1;
                if (timeCalced <= 0) {
                    i->setVisible(false);
                    listPotionNodes.erase(std::remove(listPotionNodes.begin(), listPotionNodes.end(), i), listPotionNodes.end());
                }
                else {
                    label->setString(std::to_string(timeCalced));
                }
            }
        }
    }
}

void PotionManager::removePotion(std::string potionPath)
{
}


