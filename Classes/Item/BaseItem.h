#ifndef __BASE_EQUIPMENT_H__
#define __BASE_EQUIPMENT_H__

#include "cocos2d.h"
#include "../Engine/Engine.h"
USING_NS_CC;
class Player;
class Game;
class ItemDetails;
class BaseItem : public cocos2d::Node
{
public:
    virtual bool init();
    enum Type
    {
        UPGRADE_ITEM,
        POTION_ITEM,
        COMON
    };
    enum ItemLevel
    {
        STANDARD,
        ENHANCED,
        RARE
    };
    std::string getItemName() const { return itemName; };
    void showLootIndicator()
    {
        if (lootIndicator)
            lootIndicator->setVisible(true);
        if (titleIndicator)
            titleIndicator->setVisible(true);
        if (itemNameIndicator)
            itemNameIndicator->setVisible(true);
        //if (itemName == "Health Potion" || itemName == "Mana Potion" || itemName == "Speed Potion") {
            itemSprite->setScale(0.2);
        //}
    }
    ItemDetails* itemDetails = nullptr;
    Sprite *getItemSprite() const { return itemSprite; };
    BaseItem &operator=(const BaseItem &other)
    {
        if (this != &other)
        {
            // Gn cc thnh vin ca lp t 'other' cho lp hin ti
            this->type = other.type;
            this->itemName = other.itemName;
            // Trin khai gn cho cc thnh vin khc nu cn thit
            // V d:
            // this->lootIndicator = other.lootIndicator;
            // this->itemSprite = other.itemSprite;
        }
        return *this;
    }
    Type getItemType() const { return type; };
    void setUser(Player* user) { _user = user; };
    virtual void use() {};
    Player* _user;
    float effectTime;
    float cooldown;
    float currentCooldown;
    float increaseAmount; // of potion item
    ItemLevel getItemLevel() { return itemLevel; };
    void setQuantity(int qtt) { this->quantity = qtt; };
    int getQuantity() const { return this->quantity; };
    std::string getItemDesc() const { return des; };
    bool operator==(const BaseItem& other) const {
        return itemName == other.itemName;
    }
protected:
    int quantity = 0;
    bool isActive = false;
    std::string des;
    Label* timeCooldown;
    Game* game;
    ItemLevel itemLevel;
    Type type;
    std::string itemName;
    Sprite *lootIndicator;
    Sprite *titleIndicator;
    Label *itemNameIndicator;
    Sprite *itemSprite;
};

#endif // __BASE_EQUIPMENT_H__
