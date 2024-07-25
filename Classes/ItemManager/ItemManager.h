#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "cocos2d.h"
#include "Item/ItemFactory.h"
#include "Equipment/EquipmentFactory.h"
#include "Map/Map.h"
#include "Game/Game.h"
#include "CoinManager/CoinManager.h"
class NormalMonster;
class Boss;
USING_NS_CC;

class ItemManager
{
public:
    static void dropItem(NormalMonster* monster, const std::string& itemName) {
        float dropRate = 2.0f;
        if (monster->getName() == "BabySpider" || monster->getName() == "Skeleton") {
            BaseItem* crystal1 = ItemFactory::createItem("crystal");
            crystal1->setPosition(monster->getPosition());
            crystal1->showLootIndicator();

            BaseItem* gold1 = ItemFactory::createItem("Gold");
            gold1->setPosition(monster->getPosition() + Vec2(10, 0));
            gold1->showLootIndicator();

            auto game = dynamic_cast<Game*>(Director::getInstance()->getRunningScene()->getChildByName("GameInstance"));
            if (game) {
                game->listOfItems.push_back(crystal1);
                game->listOfItems.push_back(gold1);
                game->gameMap->getTiledMap()->addChild(crystal1, 9.5);
                game->gameMap->getTiledMap()->addChild(gold1, 9.5);

                //falling trajectory
                float duration = 1.0f;
                float amplitude = 50.0f;
                float frequency = 1.0f;
                Vec2 startPosition1 = monster->getPosition();
                Vec2 endPosition1 = monster->getPosition() + Vec2(30, 0);

                Vec2 startPosition2 = monster->getPosition();
                Vec2 endPosition2 = monster->getPosition() + Vec2(-30, -20);

                int steps = 100;
                float stepDuration = duration / steps;

                Vector<FiniteTimeAction*> actions1;

                for (int i = 0; i <= steps; ++i) {
                    float t = (float)i / steps;
                    float x = startPosition1.x + t * (endPosition1.x - startPosition1.x);
                    float y = startPosition1.y + t * (endPosition1.y - startPosition1.y) + amplitude * sinf(frequency * M_PI * t);

                    actions1.pushBack(MoveTo::create(stepDuration, Vec2(x, y)));
                }

                Vector<FiniteTimeAction*> actions2;

                for (int i = 0; i <= steps; ++i) {
                    float t = (float)i / steps;
                    float x = startPosition2.x + t * (endPosition2.x - startPosition2.x);
                    float y = startPosition2.y + t * (endPosition2.y - startPosition2.y) + amplitude * sinf(frequency * M_PI * t);

                    actions2.pushBack(MoveTo::create(stepDuration, Vec2(x, y)));
                }

                auto sequence1 = Sequence::create(actions1);
                crystal1->runAction(sequence1);

                auto sequence2 = Sequence::create(actions2);
                gold1->runAction(sequence2);
            }
        }
    }
    static void dropEquipment(NormalMonster* monster, const std::string& itemName) {
        std::srand(std::time(0));
        // T?o s? ng?u nhiên t? 0 ??n 1
        float randomValue = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        if (randomValue < 0.30f) {
            if (monster->getName() == "BabySpider" || monster->getName() == "Skeleton") {
                BaseEquipment* equipmentResult = nullptr;
                std::string equipment = getEquipmentDrop(0.10f, 0.13f, 0.18f, 0.23f, 0.28f, 0.08f);
                int equipmentResultInt = 0;
                if (equipment == "ARMOR") {
                    equipmentResultInt = 1;
                }
                else if (equipment == "HELMET") {
                    equipmentResultInt = 2;
                }
                else if (equipment == "RING") {
                    equipmentResultInt = 3;
                }
                else if (equipment == "SHIELD") {
                    equipmentResultInt = 4;
                }
                else if (equipment == "SHOE") {
                    equipmentResultInt = 5;
                }
                else if (equipment == "WEAPON") {
                    equipmentResultInt = 6;
                }

                std::string equipmentType = getEquipmentTypeDrop(0.50f, 0.30f, 0.15f, 0.05f);
                if (equipmentType == "STANDARD") {
                    if (equipmentResultInt == 1) {
                        equipmentResult = EquipmentFactory::createEquipment("Chipped Armor", 1);
                    }
                    if (equipmentResultInt == 2) {
                        equipmentResult = EquipmentFactory::createEquipment("Chipped Helmet", 1);
                    }
                    if (equipmentResultInt == 3) {
                        equipmentResult = EquipmentFactory::createEquipment("Chipped Ring", 1);
                    }
                    if (equipmentResultInt == 4) {
                        equipmentResult = EquipmentFactory::createEquipment("Chipped Shield", 1);
                    }
                    if (equipmentResultInt == 5) {
                        equipmentResult = EquipmentFactory::createEquipment("Chipped Shoe", 1);
                    }
                    if (equipmentResultInt == 6) {
                        equipmentResult = EquipmentFactory::createEquipment("Chipped Sword", 1);
                    }
                }
                else if (equipmentType == "ENHANCED") {
                    if (equipmentResultInt == 1) {
                        equipmentResult = EquipmentFactory::createEquipment("Hunter Armor", 1);
                    }
                    if (equipmentResultInt == 2) {
                        equipmentResult = EquipmentFactory::createEquipment("Chipped Helmet", 1);
                    }
                    if (equipmentResultInt == 3) {
                        equipmentResult = EquipmentFactory::createEquipment("Hunter Ring", 1);
                    }
                    if (equipmentResultInt == 4) {
                        equipmentResult = EquipmentFactory::createEquipment("Knight Shield", 1);
                    }
                    if (equipmentResultInt == 5) {
                        equipmentResult = EquipmentFactory::createEquipment("Sneaker", 1);
                    }
                    if (equipmentResultInt == 6) {
                        equipmentResult = EquipmentFactory::createEquipment("Hunter Sword", 1);
                    }
                }
                else if (equipmentType == "RARE") {
                    if (equipmentResultInt == 1) {
                        equipmentResult = EquipmentFactory::createEquipment("Armor Of Knight", 1);
                    }
                    if (equipmentResultInt == 2) {
                        equipmentResult = EquipmentFactory::createEquipment("Helmet Of Knight", 1);
                    }
                    if (equipmentResultInt == 3) {
                        equipmentResult = EquipmentFactory::createEquipment("Hunter Ring", 1);
                    }
                    if (equipmentResultInt == 4) {
                        equipmentResult = EquipmentFactory::createEquipment("Knight Shield", 1);
                    }
                    if (equipmentResultInt == 5) {
                        equipmentResult = EquipmentFactory::createEquipment("Sneaker", 1);
                    }
                    if (equipmentResultInt == 6) {
                        equipmentResult = EquipmentFactory::createEquipment("Sword Of Knight", 1);
                    }
                }
                else if (equipmentType == "LEGENDARY") {
                    if (equipmentResultInt == 1) {
                        equipmentResult = EquipmentFactory::createEquipment("Soul Armor", 1);
                    }
                    if (equipmentResultInt == 2) {
                        equipmentResult = EquipmentFactory::createEquipment("Soul Helmet", 1);
                    }
                    if (equipmentResultInt == 3) {
                        equipmentResult = EquipmentFactory::createEquipment("Soul Ring", 1);
                    }
                    if (equipmentResultInt == 4) {
                        equipmentResult = EquipmentFactory::createEquipment("Soul Shield", 1);
                    }
                    if (equipmentResultInt == 5) {
                        equipmentResult = EquipmentFactory::createEquipment("Soul Shoe", 1);
                    }
                    if (equipmentResultInt == 6) {
                        equipmentResult = EquipmentFactory::createEquipment("Soul Slayer Sword", 1);
                    }
                }


                equipmentResult->setPosition(monster->getPosition());
                equipmentResult->showLootIndicator();
                auto game = dynamic_cast<Game*>(Director::getInstance()->getRunningScene()->getChildByName("GameInstance"));
                if (game) {
                    //Push back to ListEquip
                    game->listOfEquipment.push_back(equipmentResult);
                    game->gameMap->getTiledMap()->addChild(equipmentResult, 9.5);
                    //...


                    //falling trajectory
                    float duration = 1.0f;
                    float amplitude = 50.0f;
                    float frequency = 1.0f;

                    Vec2 startPosition2 = monster->getPosition();
                    Vec2 endPosition2 = monster->getPosition() + Vec2(-30, 20);

                    int steps = 100;
                    float stepDuration = duration / steps;

                    Vector<FiniteTimeAction*> actions2;

                    for (int i = 0; i <= steps; ++i) {
                        float t = (float)i / steps;
                        float x = startPosition2.x + t * (endPosition2.x - startPosition2.x);
                        float y = startPosition2.y + t * (endPosition2.y - startPosition2.y) + amplitude * sinf(frequency * M_PI * t);

                        actions2.pushBack(MoveTo::create(stepDuration, Vec2(x, y)));
                    }

                    auto sequence2 = Sequence::create(actions2);
                    equipmentResult->runAction(sequence2);
                }
            }
        }
    }

    static std::string getEquipmentTypeDrop(float standardRate, float enhancedRate, float rareRate, float legendaryRate) {
        std::srand(std::time(0));
        // T?o s? ng?u nhiên t? 0 ??n 1
        float randomValue = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

        if (randomValue < legendaryRate) {
            return "LEGENDARY";
        }
        else if (randomValue < (legendaryRate + rareRate)) {
            return "RARE";
        }
        else if (randomValue < (legendaryRate + rareRate + enhancedRate)) {
            return "ENHANCED";
        }
        else {
            return "STANDARD";
        }
    }

    static std::string getEquipmentDrop(float armorRate, float helmetRate, float ringRate, float shieldRate, float shoeRate, float weaponRate) {
        std::srand(std::time(0));
        // T?o s? ng?u nhiên t? 0 ??n 1
        float randomValue = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

        if (randomValue < weaponRate) {
            return "WEAPON";
        }
        else if (randomValue < (weaponRate + armorRate)) {
            return "ARMOR";
        }
        else if (randomValue < (weaponRate + armorRate + helmetRate)) {
            return "HELMET";
        }
        else if (randomValue < (weaponRate + armorRate + helmetRate + ringRate)) {
            return "RING";
        }
        else if (randomValue < (weaponRate + armorRate + helmetRate + ringRate + shieldRate)) {
            return "SHIELD";
        }
        else {
            return "SHOE";
        }
    }

};

#endif // __BASE_EQUIPMENT_H__
