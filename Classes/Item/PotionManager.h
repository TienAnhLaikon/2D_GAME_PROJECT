#ifndef __POTION_MANAGER__
#define __POTION_MANAGER__

#include "cocos2d.h"
USING_NS_CC;
class PotionManager : public cocos2d::Node
{
public:
    virtual bool init();
    bool addPotion(std::string potionPath, int potionCooldown);
    bool checkPotion(std::string potionPath);
    void removePotion(std::string potionPath);
    bool isActiveItem(std::string potionPath);
    CREATE_FUNC(PotionManager);
private:
    void updateCooldown(float dt);
    Node* potionNode;
    std::vector<std::pair<std::string, int>> _usingPotions;
    std::vector<Node*> listPotionNodes;
    std::vector<std::string> spriteCooldown;
};

#endif // __POTION_MANAGER__
