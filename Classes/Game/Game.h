#ifndef __GAME_H__
#define __GAME_H__

#include "cocos2d.h"
#include "../NPC/NPC.h"
#include "../NPC/NPCFactory.h"
#include "../Player/Player.h"
#include "Enemies/Boss/Boss.h"
#include "../Item/ItemFactory.h"
#include "Enemies/NormalMonster/NormalMonster.h"
#include "../Enemies/Boss/BossFactory.h"
#include "../Enemies/NormalMonster/NormalMonsterFactory.h"
#include "../UserInterface/InGame/Joystick.h"
#include "../UserInterface/InGame/Minimap.h"
#include "../Inventory/Inventory.h"
#include "../UserInterface/InGame/HealthBar.h"
#include "../UserInterface/InGame/Joystick.h"
#include "../UserInterface/InGame/TalentCarousel.h"
#include "../UserInterface/InGame/TalentButton.h"
#include "../UserInterface/InGame/TeleportEffect.h"
#include "Quest/QuestManager.h"
#include "Quest/Quest.h"
#include "Scenes/LoadingScene.h"
#include "Scenes/DiedLayer.h"

// #include "../Skills/SkillTree.h"

USING_NS_CC;
class InGameUI;
class InventoryButton;
class GameMap;
class Game : public cocos2d::Scene
{
public:
    static Scene *createGame();
    virtual bool init();
    GameMap *gameMap;
    std::vector<Node*> listOfCollision;
    std::vector<Node*> listOfSafeZone;
    std::vector<Node*> listOfHideObject;
    std::vector<Node*> listOfShowObjectRoom;
    std::vector<NormalMonster*> listOfMonster;
    //std::vector<Boss*> listOfBoss;
    std::vector<BaseItem*> listOfItems;
    std::vector<BaseEquipment*> listOfEquipment;
    LoadingScene* loadingSceneFirst;
    LoadingScene* loadingScene;
    static int backgroundMusicId;
    std::vector<NPC*> listNPC;
    Player* getPlayer() const { return _player; };
    bool save();
    void setCurrentQuestFromData();
    void updatePlayer(float);
    Compass* compass = nullptr;
    bool isGamePaused = false;
    void pauseGame();
    void pauseEnemies();
    bool loadMap(int);
    void resumeEnemies();
    void resumeGame();
    void showDiedLayer();
    void showTeleportEffect();
    Boss* boss = nullptr;
    InGameUI* inGameUI;

    void moveCameraToBoss();
    ~Game();
    CREATE_FUNC(Game);
private:
    TeleportEffect* teleportEffect = nullptr;
    DiedLayer* diedLayer = nullptr;
    bool isLoad = false;
    bool isResourcesLoaded;
    Scene* getCurrentScene() const { return Director::getInstance()->getRunningScene(); };
    bool IS_SAVING = false;
    Player::PlayerData playerSavedData;
    std::vector<SkillBase*> skillsSavedData;
    // SkillTree* skillTree;
    void updateCamera(float);
    Vec2 nellPos;
    void followResourcesLoading(float);
    void updateEnemies(float);
    void updateUserInterface(float);
    bool isShowMessage = false;
    void setup();
    int currentMap = 0;
    InventoryButton *inventoryButton;
    HealthBar *healthBar;
    Player *_player;
    float zoomLevel;
    Joystick *_joystick;
    NormalMonster *enemy;
    NormalMonster *enemy1;
    NormalMonster *enemy2;
    NormalMonster* spider1;
    NormalMonster* spider2;
    NormalMonster* spider3;
    NormalMonster* angel1;
    NormalMonster* sentinel1;
    Size visibleSize;

    Boss *okyanus = nullptr;
    Boss* paradiso = nullptr;
    NPC* robert = nullptr;
    NPC *durin = nullptr;
    NPC* thienAn = nullptr;
    Inventory *inventory;
    cocos2d::PhysicsWorld *world;
    void setPhysicsWorld(cocos2d::PhysicsWorld *m_world)
    {
        m_world = world;
    }



};

#endif // __GAME_H__