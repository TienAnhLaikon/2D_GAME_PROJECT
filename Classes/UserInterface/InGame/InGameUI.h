#ifndef __INGAME_UI__
#define __INGAME_UI__

#include "cocos2d.h"

USING_NS_CC;

class BaseCharacter;
class Player;
class Joystick;
class Inventory;
class UpgradeInventory;
class UpgradeInventoryButton;
class QuestButton;
class HamburgerMenu;
class HamburgerMenuButton;
class InventoryButton;
class TalentButton;
class TalentCarousel;
class ExpBar;
class HealthBar;
class NPC;
class SubInventory;
class QuestManager;
class DialogueManager;
class TutorialManager;
class LevelUpPopup;
class PotionManager;
class SkillButton;
class SelectMapButton;
class SelectMapLayer;
class AnimaticScene;
class Dropdown;
class QuestBtn;

class InGameUI : public cocos2d::Layer
{
public:
    virtual bool init(BaseCharacter* player);
    Joystick* getMovementJoystick() const { return _joystick; };
    HealthBar* healthBar = nullptr;
    ExpBar* expBar = nullptr;

    ~InGameUI() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }
    static InGameUI* getInstance(BaseCharacter* player) {
        if (instance == nullptr)instance = new InGameUI(player);
        return instance;
    }
    void hideUI();
    void showUI();
    void setNPCs(std::vector<NPC*> listNPC);
    void setTarget(Player* plr);
    Player* _player_temp = nullptr;
    DialogueManager* dialogueManager = nullptr;
    UpgradeInventoryButton* upgradeInventoryButton = nullptr;
    Joystick* _joystick = nullptr;
    Inventory* inventory = nullptr;
    UpgradeInventory* upgradeInventory = nullptr;
    QuestButton* questButton = nullptr;
    HamburgerMenuButton* hamburgerButton = nullptr;
    HamburgerMenu* hamburgerMenu = nullptr;
    InventoryButton* inventoryButton = nullptr;
    TalentCarousel* talentCarousel = nullptr;
    TalentButton* talentButton = nullptr;
    QuestManager* questManager = nullptr;
    SubInventory* subInventory = nullptr;
    TutorialManager* tutorialManager = nullptr;
    LevelUpPopup* levelUpPopup = nullptr;
    PotionManager* potionManager = nullptr;
    SelectMapButton* selectMapButton = nullptr;
    SelectMapLayer* selectMapLayer = nullptr;
    AnimaticScene* storyScene = nullptr;
    QuestBtn* questBtn = nullptr;
    Dropdown* dropdownButton = nullptr;
    std::vector<NPC*> _listNPC;
    void showLevelUpPopup();
    void update(float dt);
private:
    InGameUI(BaseCharacter* player);
    static InGameUI* instance;
    BaseCharacter* _player = nullptr;

};

#endif // __JOYSTICK_H__