//#ifndef __GAME_H__
//#define __GAME_H__
//
//#include "cocos2d.h"
//#include "../NPC/NPC.h"
//#include "../NPC/NPCFactory.h"
//#include "../Player/Player.h"
//#include "Enemies/Boss/Boss.h"
//#include "../Item/ItemFactory.h"
//#include "Enemies/NormalMonster/NormalMonster.h"
//#include "../Enemies/Boss/BossFactory.h"
//#include "../Enemies/NormalMonster/NormalMonsterFactory.h"
//#include "../UserInterface/InGame/Joystick.h"
//#include "../UserInterface/InGame/Minimap.h"
//#include "../Inventory/Inventory.h"
//#include "../UserInterface/InGame/HealthBar.h"
//#include "../UserInterface/InGame/Joystick.h"
//#include "MapDecoration/MapDecoration.h"
//#include "../UserInterface/InGame/TalentCarousel.h"
//#include "../UserInterface/InGame/TalentButton.h"
//// #include "../Skills/SkillTree.h"
//
//USING_NS_CC;
//class InGameUI;
//class InventoryButton;
//class GameMap;
//class Game : public cocos2d::Scene
//{
//public:
//    static Scene *createGame();
//    virtual bool init();
//    GameMap *gameMap;
//    MinimapLayer* minimap;
//    std::vector<Node*> listOfCollision;
//    CREATE_FUNC(Game);
//
//private:
//    InGameUI *inGameUI;
//    // SkillTree* skillTree;
//    void updateCamera(float);
//    void updatePlayer(float);
//    void updateEnemies(float);
//    void updateUserInterface(float);
//    void setup();
//    bool loadMap(int);
//    int currentMap = 0;
//    InventoryButton *inventoryButton;
//    HealthBar *healthBar;
//    Player *_player;
//    float zoomLevel;
//    Joystick *_joystick;
//    NormalMonster *enemy;
//    NormalMonster *enemy1;
//    NormalMonster *enemy2;
//    NormalMonster* spider1;
//    NormalMonster* spider2;
//    NormalMonster* spider3;
//    NormalMonster* angel1;
//    NormalMonster* sentinel1;
//    Size visibleSize;
//    Boss *boss;
//    Boss *okyanus;
//    Boss* paradiso;
//    NPC* robert;
//    NPC *durin;
//    NPC* thienAn;
//    Inventory *inventory;
//    std::vector<BaseItem*> listOfItems;
//    cocos2d::PhysicsWorld *world;
//    void setPhysicsWorld(cocos2d::PhysicsWorld *m_world)
//    {
//        m_world = world;
//    }
//};
//
//#endif // __GAME_H__