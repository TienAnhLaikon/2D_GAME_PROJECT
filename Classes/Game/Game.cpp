#include "Game.h"
#include "Skills/SkillFactory.h"
#include "Skills/SkillBase.h"
#include "UserInterface/InGame/InGameUI.h"
#include "Map/Map.h"
#include "Enemies/NormalMonster/NormalMonsterFactory.h"
#include "Scenes/LoadingScene.h"
#include "WorkingWithData/JSON/jsonManager.h"
#include "Scenes/ScenesManager.h"
#include "AudioEngine.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
#include "DialogueManager/DialogueManager.h"
#include "TutorialManager/TutorialManager.h"
#include "UserInterface/InGame/SelectMapButton.h"
#include "CoinManager/CoinManager.h"
#include "Scenes/AnimaticScene.h"
#include "UserInterface/InGame/TeleportEffect.h"
int Game::backgroundMusicId = 0;
Scene *Game::createGame()
{
    auto scene = Scene::createWithPhysics();
    scene->setName("GameSceneCheck");
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -98));
    scene->getPhysicsWorld()->setSubsteps(10);

    auto layer = dynamic_cast<Game*>(scene->getChildByName("GameInstance"));
    if (layer) {
        layer->removeAllChildren();
        layer->removeFromParentAndCleanup(true);
    }


    layer = Game::create();
    layer->setName("GameInstance");
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    JsonManager::getInstance();
    return scene;
}

bool Game::init()
{
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    zoomLevel = 1;
    visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();

    playerSavedData = JsonManager::getInstance()->getPlayerData();
    currentMap = playerSavedData.currentMap;
    if (!(currentMap >= 0 && currentMap < 10)) {
        currentMap = 0;
    }

    IS_SAVING = (playerSavedData.positionX != 0 && playerSavedData.positionY != 0) ? true : false;

    // setup
    auto addLoadingSceneAction = CallFunc::create([this]() {
        loadingSceneFirst = LoadingScene::create();
        loadingSceneFirst->setPosition(Vec2());
        loadingSceneFirst->setScale(visibleSize.width / (loadingSceneFirst->getContentSize().width * zoomLevel), visibleSize.height / (loadingSceneFirst->getContentSize().height * zoomLevel));
        this->addChild(loadingSceneFirst, 1000);
    });

    auto loadMapAction = CallFunc::create([this]() {
        Game::setup();
        schedule(CC_SCHEDULE_SELECTOR(Game::updatePlayer), 0.06f);
        schedule(CC_SCHEDULE_SELECTOR(Game::updateEnemies), 0.06f);
        schedule(CC_SCHEDULE_SELECTOR(Game::updateUserInterface), 0.0f);
        schedule(CC_SCHEDULE_SELECTOR(Game::updateCamera), 0.0f);


        });

    auto sequence = Sequence::create(
        addLoadingSceneAction,
        DelayTime::create(0.2f),
        loadMapAction,
        nullptr
    );
    this->runAction(sequence);


    return true;
}

void Game::updatePlayer(float dt)
{
    if (_player) {
        if ((gameMap->getEndPoint().containsPoint(_player->getPosition())))
        {
            inGameUI->selectMapButton->showButton();
        }
        else if (inGameUI->selectMapButton->isVisible()) {
            inGameUI->selectMapButton->hideButton();
        }
        if (inGameUI && inGameUI->getMovementJoystick())
        {
            _player->movementDirection = inGameUI->getMovementJoystick()->getDirection();
            if (inGameUI->getMovementJoystick()->getIsPress())
            {
                _player->handleMove(dt);
            }
            else
            {
                _player->setMoving(false);
            }
        }
        for (auto item : listOfItems)
        {
            auto playerRect = Rect(_player->getPositionX() - 18, _player->getPositionY() - 18, 36, 36);
            if (playerRect.containsPoint(item->getPosition()))
            {
                _player->pickUpItem(item->getItemName());
                if(item->getItemName() != "Gold")
                    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/pickup_item_sound1.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                else
                    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/pickup_gold_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

                listOfItems.erase(std::remove(listOfItems.begin(), listOfItems.end(), item), listOfItems.end());
                item->removeFromParentAndCleanup(true);
                break;
            }
        }
        for (auto item : listOfEquipment)
        {
            auto playerRect = Rect(_player->getPositionX() - 18, _player->getPositionY() - 18, 36, 36);
            if (playerRect.containsPoint(item->getPosition()))
            {
                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/pickup_item_sound1.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                _player->pickUpEquipment(item->getEquipmentName(), item->getLevel());
                listOfEquipment.erase(std::remove(listOfEquipment.begin(), listOfEquipment.end(), item), listOfEquipment.end());
                item->removeFromParentAndCleanup(true);
                break;
            }
        }
    } 
}

bool Game::loadMap(int levelMap)
{

    Audio::getInstance()->stopAll();
    UserDefault::getInstance()->setIntegerForKey("background_music", Audio::getInstance()->play2d("sound/musics/background-music_2.mp3", true, SettingsData::getInstance()->getMusicSlider() / 100.0f));
    SceneManager::getInstance()->game = this;
    if(gameMap)
    {   
        QuestManager::getInstance()->removeFromParentAndCleanup(true);
        std::vector<BaseItem *> itemsToRemove;
        for (auto item : listOfItems)
        {
            if (item)
            {
                item->removeFromParentAndCleanup(true);
                itemsToRemove.push_back(item);
            }
        }
        for (auto item : itemsToRemove)
        {
            auto it = std::find(listOfItems.begin(), listOfItems.end(), item);
            if (it != listOfItems.end())
            {
                listOfItems.erase(it);
            }
        }

        if (compass) {
            compass->removeFromParentAndCleanup(true);
            compass = nullptr;
        }
        listOfMonster.clear();
        //listOfBoss.clear();
        listNPC.clear();

        _player->observers.clear();

        _player->removeFromParentAndCleanup(true);
        _player = nullptr;
        gameMap->getTiledMap()->removeFromParentAndCleanup(true);
        gameMap->~GameMap();

        std::vector<Node *> nodesToRemove;
        for (auto node : listOfCollision)
        {
            nodesToRemove.push_back(node);
        }
        for (auto node : nodesToRemove)
        {
            node->removeFromParent();
        }
        listOfCollision.clear();

        std::vector<Node*> nodesToRemove2;
        for (auto node : listOfHideObject)
        {
            nodesToRemove2.push_back(node);
        }
        for (auto node : nodesToRemove2)
        {
            node->removeFromParent();
        }
        listOfHideObject.clear();

        std::vector<Node*> nodesToRemove3;
        for (auto node : listOfShowObjectRoom)
        {
            nodesToRemove3.push_back(node);
        }
        for (auto node : nodesToRemove3)
        {
            node->removeFromParent();
        }
        listOfShowObjectRoom.clear();

        teleportEffect->removeFromParentAndCleanup(true);
        teleportEffect = nullptr;
        
        std::vector<Node*> nodesToRemove4;
        for (auto node : listOfSafeZone)
        {
            nodesToRemove4.push_back(node);
        }
        for (auto node : nodesToRemove4)
        {
            node->removeFromParent();
        }
        listOfSafeZone.clear();

        this->removeChildByName("map");
    }

    gameMap = GameMap::getInstance(levelMap);
    auto tiledMap = gameMap->getTiledMap();
    tiledMap->setName("map");
    this->addChild(tiledMap);

    teleportEffect = TeleportEffect::create();
    teleportEffect->setVisible(false);
    tiledMap->addChild(teleportEffect, 60);

    auto objectLayer = gameMap->getTiledMap()->getObjectGroup("Collision");

    if (objectLayer)
    {
        const auto& objects = objectLayer->getObjects();
        for (const auto& object : objects)
        {
            ValueMap objectProps = object.asValueMap();
            float x = objectProps["x"].asFloat();
            float y = objectProps["y"].asFloat();
            float width = objectProps["width"].asFloat();
            float height = objectProps["height"].asFloat();
            Node* node = Node::create();
            node->setName("Wall");
            node->setContentSize(Size(width, height));
            node->setPosition(x, y);
            listOfCollision.push_back(node);
            this->addChild(node);
        }
    }

    auto hideObjectLayer = gameMap->getTiledMap()->getObjectGroup("HideObject");

    if (hideObjectLayer)
    {
        const auto& objects = hideObjectLayer->getObjects();
        for (const auto& object : objects)
        {
            ValueMap objectProps = object.asValueMap();
            float x = objectProps["x"].asFloat();
            float y = objectProps["y"].asFloat();
            float width = objectProps["width"].asFloat();
            float height = objectProps["height"].asFloat();
            Node* node = Node::create();
            node->setContentSize(Size(width, height));
            node->setPosition(x, y);
            listOfHideObject.push_back(node);
            this->addChild(node);
        }
    }

    auto showObjectRoom = gameMap->getTiledMap()->getObjectGroup("JoinRoom");

    if (showObjectRoom)
    {
        const auto& objects = showObjectRoom->getObjects();
        for (const auto& object : objects)
        {
            ValueMap objectProps = object.asValueMap();
            float x = objectProps["x"].asFloat();
            float y = objectProps["y"].asFloat();
            float width = objectProps["width"].asFloat();
            float height = objectProps["height"].asFloat();
            Node* node = Node::create();
            node->setContentSize(Size(width, height));
            node->setPosition(x, y);
            listOfShowObjectRoom.push_back(node);
            this->addChild(node);
        }
    }

    if (levelMap != 0) {
        auto safeZone = gameMap->getTiledMap()->getObjectGroup("SafeZone");

        if (safeZone)
        {
            const auto& objects = safeZone->getObjects();
            for (const auto& object : objects)
            {
                ValueMap objectProps = object.asValueMap();
                float x = objectProps["x"].asFloat();
                float y = objectProps["y"].asFloat();
                float width = objectProps["width"].asFloat();
                float height = objectProps["height"].asFloat();
                Node* node = Node::create();
                node->setContentSize(Size(width, height));
                node->setPosition(x, y);
                listOfSafeZone.push_back(node);
                this->addChild(node);
            }

        }
    }
    
    // Thien An
    // ValueMap ThienAnNPCPoint = NPCobjGroup->getObject("Thien An The First Slayer");
    // int thienAnX = ThienAnNPCPoint.at("x").asInt();
    // int thienAnY = ThienAnNPCPoint.at("y").asInt();

    TMXObjectGroup* objGroup = gameMap->getTiledMap()->getObjectGroup("Player");
    ValueMap spawnPoint = objGroup->getObject("Start Point");
    int x = spawnPoint.at("x").asInt();
    int y = spawnPoint.at("y").asInt();

    playerSavedData = JsonManager::getInstance()->getPlayerData();
    if (gameMap->currentMap != JsonManager::getInstance()->getPlayerData().currentMap || JsonManager::getInstance()->getPlayerData().positionX == 0) {
        _player = Player::create();

        _player->addSkill("FireSear");
        for (auto skill : _player->skills) {
            if (skill->getSkillTalent()->skillName == "FireSear") skill->setSkillIndex(5);
        }
        _player->resetSkillSlotOrder();
        
        _player->setPosition(Vec2(x, y));
    }
    else {
        _player = Player::create();

        _player->addSkill("FireSear");
        for (auto skill : _player->skills) {
            if (skill->getSkillTalent()->skillName == "FireSear") skill->setSkillIndex(5);
        }
        _player->resetSkillSlotOrder();
        _player->setPosition(playerSavedData.positionX, playerSavedData.positionY);
       // _player->setPosition(Vec2(x, y));
    }
    _player->gainExpFromData(playerSavedData.exp);
    
    _player->setExpToConsume(playerSavedData.expToConsume);
    //_player->setExpToConsume(20000);
    //_player->gainExp(1200);
    _player->listOfCollision = this->listOfCollision;
    _player->listOfHideObject = this->listOfHideObject;
    _player->listOfShowObjectRoom = this->listOfShowObjectRoom;
    _player->currentState = _player->idleState;
    _player->currentState->EnterState();
    _player->currentMap = levelMap;
    _player->setTiledMap(gameMap);
    gameMap->getTiledMap()->addChild(_player, 20);


    diedLayer = DiedLayer::create();
    diedLayer->setScale(visibleSize.width / (diedLayer->getContentSize().width * zoomLevel), visibleSize.height / (diedLayer->getContentSize().height * zoomLevel));
    this->addChild(diedLayer, 5200);
    diedLayer->setVisible(false);

    Player::PlayerUsingEquipment playerUsingEquipment = JsonManager::getInstance()->getPlayerUsingEquipmentData();
    _player->equipWeapon(std::get<0>(playerUsingEquipment.weaponNLevel), std::get<1>(playerUsingEquipment.weaponNLevel));
    _player->equipArmor(std::get<0>(playerUsingEquipment.armorNLevel), std::get<1>(playerUsingEquipment.armorNLevel));
    _player->equipRing(std::get<0>(playerUsingEquipment.ringNLevel), std::get<1>(playerUsingEquipment.ringNLevel));
    _player->equipHelmet(std::get<0>(playerUsingEquipment.helmetNLevel), std::get<1>(playerUsingEquipment.helmetNLevel));
    _player->equipShoe(std::get<0>(playerUsingEquipment.shoeNLevel), std::get<1>(playerUsingEquipment.shoeNLevel));
    _player->equipShield(std::get<0>(playerUsingEquipment.shieldNLevel), std::get<1>(playerUsingEquipment.shieldNLevel));
    _player->getUpgradeInventory()->addEquipment(std::get<0>(playerUsingEquipment.weaponNLevel), std::get<1>(playerUsingEquipment.weaponNLevel), 1);
    _player->getUpgradeInventory()->addEquipment(std::get<0>(playerUsingEquipment.armorNLevel), std::get<1>(playerUsingEquipment.armorNLevel), 1);
    _player->getUpgradeInventory()->addEquipment(std::get<0>(playerUsingEquipment.ringNLevel), std::get<1>(playerUsingEquipment.ringNLevel), 1);
    _player->getUpgradeInventory()->addEquipment(std::get<0>(playerUsingEquipment.helmetNLevel), std::get<1>(playerUsingEquipment.helmetNLevel), 1);
    _player->getUpgradeInventory()->addEquipment(std::get<0>(playerUsingEquipment.shoeNLevel), std::get<1>(playerUsingEquipment.shoeNLevel), 1);
    _player->getUpgradeInventory()->addEquipment(std::get<0>(playerUsingEquipment.shieldNLevel), std::get<1>(playerUsingEquipment.shieldNLevel), 1);

    std::vector<std::pair<std::string, int>> usingItemsNQuantity = JsonManager::getInstance()->getUsingItem();
    if (!usingItemsNQuantity.empty()) {
        for (auto item : usingItemsNQuantity) {
            _player->equipItem(std::get<0>(item), std::get<1>(item));
            if (_player->getInventory()->itemNode1->getStatus() == "free") {
                _player->getInventory()->itemNode1->setBaseItem(std::get<0>(item));
                _player->getInventory()->itemNode1->setQuantity(std::get<1>(item));
                continue;
            }
            if (_player->getInventory()->itemNode2->getStatus() == "free") {
                _player->getInventory()->itemNode2->setBaseItem(std::get<0>(item));
                _player->getInventory()->itemNode2->setQuantity(std::get<1>(item));
                continue;
            }
            if (_player->getInventory()->itemNode3->getStatus() == "free") {
                _player->getInventory()->itemNode3->setBaseItem(std::get<0>(item));
                _player->getInventory()->itemNode3->setQuantity(std::get<1>(item));
                continue;
            }
            if (_player->getInventory()->itemNode4->getStatus() == "free") {
                _player->getInventory()->itemNode4->setBaseItem(std::get<0>(item));
                _player->getInventory()->itemNode4->setQuantity(std::get<1>(item));
                continue;
            }

        }
    }

    if (loadingSceneFirst) {
        loadingSceneFirst->setPosition(_player->getPosition() - visibleSize / 2);
        loadingSceneFirst->setScale(visibleSize.width / (loadingSceneFirst->getContentSize().width * zoomLevel), visibleSize.height / (loadingSceneFirst->getContentSize().height * zoomLevel));
    }
    else {
        CCLOG("LOADING SCENE ERROR");
    }

    for (auto i : JsonManager::getInstance()->getSkillsData())
    {
        _player->setSkill(i);
    }
    for (auto i : JsonManager::getInstance()->getItemsData()) {
        _player->pickUpItem(i);
    }
    if (!JsonManager::getInstance()->getEquipmentsData().empty()) {
        for (auto i : JsonManager::getInstance()->getEquipmentsData()) {
            _player->pickUpEquipment(i.name, i.level);
        }
    }
    else
    {
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Soul Slayer Sword", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);
        _player->pickUpEquipment("Chipped Shoe", 1);


        
    }

    if (!inGameUI) {
        inGameUI = InGameUI::getInstance(_player);
        inGameUI->setAnchorPoint(Vec2(0, 0));
        inGameUI->setScale(1 / (visibleSize.width / visibleSize.height));
        inGameUI->setName("ingameui");
        this->addChild(inGameUI, 500);

    }
    // ly v tr NPC
    TMXObjectGroup* NPCobjGroup = gameMap->getTiledMap()->getObjectGroup("NPC");
    if (NPCobjGroup) {
        // Durin
        auto NPCobjGroups = NPCobjGroup->getObjects();
        for (const auto& object : NPCobjGroups) {
            auto objectMap = object.asValueMap();
            std::string objectName = objectMap["name"].asString();
            if (objectName == "Durin The Blacksmith") {
                ValueMap durinNPCPoint = NPCobjGroup->getObject("Durin The Blacksmith");
                if (!durinNPCPoint.empty()) {
                    int durinX = durinNPCPoint.at("x").asInt();
                    int durinY = durinNPCPoint.at("y").asInt();
                    if (durin != nullptr)
                        durin->removeFromParentAndCleanup(true);
                    durin = NPCFactory::createNPC("Durin The Blacksmith");
                    durin->setPosition(Vec2(durinX, durinY));
                    durin->setName("Durin");
                    gameMap->getTiledMap()->addChild(durin, 9.5);
                    durin->currentState = durin->idleState;
                    durin->currentState->EnterState();
                    //durin->setTarget(_player);
                    listNPC.push_back(durin);
                    //DialogueManager::getInstance()->setNPC(durin);
                }
            }
            //else if (objectName == "Robert The Fallen Slayer") {
            //    // robert
            //    ValueMap robertNPCPoint = NPCobjGroup->getObject("Robert The Fallen Slayer");
            //    if (!robertNPCPoint.empty()) {
            //        int robertX = robertNPCPoint.at("x").asInt();
            //        int robertY = robertNPCPoint.at("y").asInt();
            //        if (robert != nullptr)
            //            robert->removeFromParentAndCleanup(true);
            //        robert = NPCFactory::createNPC("Robert The Fallen Slayer");
            //        robert->setPosition(Vec2(robertX, robertY));
            //        robert->setName("robert");
            //        gameMap->getTiledMap()->addChild(robert, 9.5);
            //        robert->currentState = robert->idleState;
            //        robert->currentState->EnterState();
            //        listNPC.push_back(robert);
            //        //DialogueManager::getInstance()->setNPC(durin);
            //    }
            //}
        }
    }
    if(inGameUI) {
        inGameUI->setTarget(_player);
        inGameUI->setNPCs(listNPC);
    }
    if(levelMap == 1)
    _player->registerObserver(QuestManager::getInstance());

    if (gameMap->currentMap != JsonManager::getInstance()->getPlayerData().currentMap) {
        auto objectGroup = gameMap->getTiledMap()->getObjectGroup("Enemies");
        if (objectGroup)
        {
            auto objects = objectGroup->getObjects();
            for (const auto& object : objects)
            {
                auto objectMap = object.asValueMap();
                std::string objectName = objectMap["name"].asString();
                if (objectName == "Skeleton")
                {
                    enemy1 = NormalMonsterFactory::createNormalMonster("Skeleton");
                    float objectX = objectMap["x"].asFloat();
                    float objectY = objectMap["y"].asFloat();
                    enemy1->setTarget(_player);
                    enemy1->setName("Skeleton");
                    enemy1->setPosition(Vec2(objectX, objectY));
                    gameMap->getTiledMap()->addChild(enemy1, 9.5);
                    enemy1->currentState = enemy1->walkState;
                    enemy1->currentState->EnterState();
                    enemy1->setLevel(2);
                    listOfMonster.push_back(enemy1);
                }
                else if (objectName == "BabySpider")
                {
                    spider3 = NormalMonsterFactory::createNormalMonster("BabySpider");
                    float objectX = objectMap["x"].asFloat();
                    float objectY = objectMap["y"].asFloat();
                    spider3->setTarget(_player);
                    spider3->setName("BabySpider");
                    spider3->setPosition(objectX, objectY);
                    gameMap->getTiledMap()->addChild(spider3, 9.5);
                    spider3->currentState = spider3->walkState;
                    spider3->currentState->EnterState();
                    spider3->setLevel(4);
                    listOfMonster.push_back(spider3);
                }
                else if (objectName == "terra")
                {
                    boss = BossFactory::createBoss("HellBeast");
                    float objectX = objectMap["x"].asFloat();
                    float objectY = objectMap["y"].asFloat();
                    boss->setPosition(Vec2(objectX, objectY));
                    boss->setName("Terra");
                    gameMap->getTiledMap()->addChild(boss, 9.5);
                    boss->currentState = boss->roarState;
                    boss->currentState->EnterState();
                    //listOfBoss.push_back(boss);
                }
                /*else if (objectName == "ShadowSentinel")
                {
                    sentinel1 = NormalMonsterFactory::createNormalMonster("ShadowSentinel");
                    float objectX = objectMap["x"].asFloat();
                    float objectY = objectMap["y"].asFloat();
                    sentinel1->setTarget(_player);
                    sentinel1->setName("ShadowSentinel");
                    sentinel1->setPosition(objectX, objectY);
                    gameMap->getTiledMap()->addChild(sentinel1, 9.5);
                    sentinel1->currentState = sentinel1->walkState;
                    sentinel1->currentState->EnterState();
                    listOfMonster.push_back(sentinel1);
                }*/
            }
        }
    }
    else {
        for (auto enemy : JsonManager::getInstance()->getEnemiesData()) {
            gameMap->getTiledMap()->addChild(enemy, 9.5);
            enemy->currentState = enemy->walkState;
            enemy->currentState->EnterState();
            enemy->setTarget(_player);
            enemy->setName(enemy->getEnemyName());
            if (enemy->getEnemyName() == "Skeleton") {
                enemy->setLevel(2);
            }
            else if (enemy->getEnemyName() == "BabySpider") {
                enemy->setLevel(4);
            }
            listOfMonster.push_back(enemy);
        }
    }
    setCurrentQuestFromData();

    if (boss && _player && levelMap == 3) {
        moveCameraToBoss();
    }
    compass = Compass::create();
    compass->setScale(0.05 * Director::getInstance()->getContentScaleFactor());
    this->addChild(compass);

    //QuestManager::getInstance()->init(listNPC);
    //QuestManager::getInstance()->setVisible(false);
    
    // BOSS
  /*   boss = BossFactory::createBoss("HellBeast");
     boss->setPosition(Vec2(900, 500));
     boss->setName("Terra");
     gameMap->getTiledMap()->addChild(boss, 9.5);
     boss->currentState = boss->roarState;
     boss->currentState->EnterState();*/

    //// OKYANUS BOSS
    // okyanus = BossFactory::createBoss("Okyanus");
    // okyanus->setPosition(Vec2(500, 500));
    // okyanus->setName("Okyanus");
    // gameMap->getTiledMap()->addChild(okyanus, 9.5);
    // okyanus->currentState = okyanus->idleState;
    // okyanus->currentState->EnterState();
    //
    //// Paradiso boss
    // paradiso = BossFactory::createBoss("Paradiso");
    // paradiso->setPosition(Vec2(300, 500));
    // paradiso->setName("Paradiso");
    // gameMap->getTiledMap()->addChild(paradiso, 9.5);
    // paradiso->currentState = paradiso->idleState;
    // paradiso->currentState->EnterState();

    // NPC Thien An
    // thienAn = NPCFactory::createNPC("Thien An The First Slayer");
    // thienAn->setPosition(Vec2(thienAnX, thienAnY));
    // thienAn->setName("ThienAn");
    // this->addChild(thienAn);
    // thienAn->currentState = thienAn->idleState;
    // thienAn->currentState->EnterState();



    ////0
    // enemy = NormalMonsterFactory::createNormalMonster("Slime");
    // enemy->setTarget(_player);
    // enemy->setPosition(Vec2(visibleSize.width / 2 + 100, visibleSize.height / 2));
    // gameMap->getTiledMap()->addChild(enemy, 9.5);

    // enemy->currentState = enemy->walkState;
    // enemy->currentState->EnterState();
    ////1
    // enemy1 = NormalMonsterFactory::createNormalMonster("Skeleton");
    // enemy1->setTarget(_player);
    // enemy1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
    // gameMap->getTiledMap()->addChild(enemy1, 9.5);

    // enemy1->currentState = enemy1->walkState;

    // enemy1->currentState->EnterState();
    ////2
    // enemy2 = NormalMonsterFactory::createNormalMonster("Skeleton");
    // enemy2->setTarget(_player);
    // enemy2->setPosition(Vec2(visibleSize.width / 2 + 100, visibleSize.height / 2 + 100));
    // gameMap->getTiledMap()->addChild(enemy2, 9.5);

    // enemy2->currentState = enemy2->walkState;
    // enemy2->currentState->EnterState();

    //// to baby spider
    // spider1 = NormalMonsterFactory::createNormalMonster("BabySpider");
    // spider1->setTarget(_player);
    // spider1->setPosition(250,350);
    // gameMap->getTiledMap()->addChild(spider1, 9.5);
    // spider1->currentState = spider1->walkState;
    // spider1->currentState->EnterState();

    // spider2 = NormalMonsterFactory::createNormalMonster("BabySpider");
    // spider2->setTarget(_player);
    // spider2->setPosition(300, 100);
    // gameMap->getTiledMap()->addChild(spider2, 9.5);
    // spider2->currentState = spider2->walkState;
    // spider2->currentState->EnterState();
    //
    // spider3 = NormalMonsterFactory::createNormalMonster("BabySpider");
    // spider3->setTarget(_player);
    // spider3->setPosition(100, 220);
    // gameMap->getTiledMap()->addChild(spider3, 9.5);
    // spider3->currentState = spider3->walkState;
    // spider3->currentState->EnterState();

    // auto en6 = NormalMonsterFactory::createNormalMonster("Angelos");
    // angel1 = dynamic_cast<Angelos*>(en6);
    // angel1->setTarget(_player);
    // angel1->setPosition(300, 100);
    // gameMap->getTiledMap()->addChild(angel1, 9.5);
    // angel1->currentState = angel1->walkState;
    // angel1->currentState->EnterState();

    // auto en7 = NormalMonsterFactory::createNormalMonster("ShadowSentinels");
    // sentinel1 = dynamic_cast <ShadowSentinels*>(en7);
    // sentinel1->setTarget(_player);
    // sentinel1->setPosition(200, 300);
    // gameMap->getTiledMap()->addChild(sentinel1, 9.5);
    // sentinel1->currentState = sentinel1->walkState;
    // sentinel1->currentState->EnterState();

    return true;
}

void Game::pauseGame()
{
    _player->SwitchState(_player->idleState);
     //duyệt qua các skills mà player đang sử dụng trong 4 slot 
    if (_player) {
        auto playerSkills = _player->getPlayerSkills();
        if (!playerSkills.empty()) {
            for (auto skill : playerSkills) {
                // tạm dừng schedule của chúng
                if (skill && skill->getSkillIndex() >= 1 && skill->getSkillIndex() <= 5) {
                    if (skill->performEffect && skill->getSkillSprite() && skill->getSkillSprite()->getPhysicsBody()) {
                        //skill->stopAction(skill->performEffect);
                        skill->getSkillSprite()->removeComponent(skill->getSkillSprite()->getPhysicsBody());
                        skill->getSkillSprite()->pause();
 
                    }
                }
            }
        }
    }


    this->unschedule(CC_SCHEDULE_SELECTOR(Game::updatePlayer));
    this->unschedule(CC_SCHEDULE_SELECTOR(Game::updateEnemies));
    if (!this->listOfMonster.empty()) {
        for (auto enemy : this->listOfMonster) {
            if (enemy)
                enemy->stopSprite();
        }
    }

    //if(boss && boss->getCurrentSprite())
    //    boss->getCurrentSprite()->pause();
    //if (okyanus && okyanus->getCurrentSprite())
    //    okyanus->getCurrentSprite()->pause();
    //if (paradiso && paradiso->getCurrentSprite())
    //    paradiso->getCurrentSprite()->pause();
    

    this->unschedule(CC_SCHEDULE_SELECTOR(Game::updateUserInterface));
    this->unschedule(CC_SCHEDULE_SELECTOR(Game::updateCamera));
}

void Game::pauseEnemies()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(Game::updateEnemies));
    if (!this->listOfMonster.empty()) {
        for (auto enemy : this->listOfMonster) {
            if (enemy)
                enemy->stopSprite();
        }
    }
    //if (boss)
    //    boss->getCurrentSprite()->pause();
    //if (okyanus)
    //    okyanus->getCurrentSprite()->pause();
    //if (paradiso)
    //    paradiso->getCurrentSprite()->pause();
}

void Game::resumeEnemies() {
    this->schedule(CC_SCHEDULE_SELECTOR(Game::updateEnemies), 0.06f);
    for (auto enemy : listOfMonster) {
        enemy->schedule(CC_SCHEDULE_SELECTOR(NormalMonster::updateEnemy), 0.5f);
    }

  /*  if (boss)
        boss->getCurrentSprite()->resume();
    if (okyanus)
        okyanus->getCurrentSprite()->resume();
    if (paradiso)
        paradiso->getCurrentSprite()->resume();*/
}

void Game::resumeGame()
{
    // duyệt qua các skills mà player đang sử dụng trong 4 slot 

    // Reschedule các hàm update trong game scene
    this->schedule(CC_SCHEDULE_SELECTOR(Game::updatePlayer), 0.06f);
    this->schedule(CC_SCHEDULE_SELECTOR(Game::updateEnemies), 0.06f);
    this->schedule(CC_SCHEDULE_SELECTOR(Game::updateUserInterface), 0.0f);
    this->schedule(CC_SCHEDULE_SELECTOR(Game::updateCamera), 0.0f);

    _player->SwitchState(_player->idleState);

    for (auto enemy : listOfMonster) {
        enemy->schedule(CC_SCHEDULE_SELECTOR(NormalMonster::updateEnemy), 0.5f);
    }
  /*  if (boss)
        boss->getCurrentSprite()->resume();
    if (okyanus)
        okyanus->getCurrentSprite()->resume();
    if (paradiso)
        paradiso->getCurrentSprite()->resume();*/

    if (_player) {
        auto playerSkills = _player->getPlayerSkills();
        for (auto skill : playerSkills) {
            if (skill->performEffect && skill->getSkillSprite() && skill->physicsBody) {
                //skill->runAction(skill->performEffect);
                if (skill->physicsBody) {
                    skill->getSkillSprite()->addComponent(skill->physicsBody);
                    skill->getSkillSprite()->resume();
                }
            }
        }
    }
}

void Game::updateEnemies(float dt)
{
   /* for (auto it = listOfMonster.begin(); it != listOfMonster.end(); ) {
        if ((*it)->isDead) {
            (*it)->removeFromParentAndCleanup(true);
            it = listOfMonster.erase(it);
        }
        else {
            ++it;
        }
    }*/

}

void Game::updateCamera(float deltaT)
{
    if (_player) {
        Scene *currentscene = Director::getInstance()->getRunningScene();
        currentscene->setScale(zoomLevel);
        Vec2 playerPosition = _player->getPosition();
        float movementSpeed = _player->getMovementSpeed();

        Vec2 targetCameraPosition = Vec2((-playerPosition.x + visibleSize.width * 0.5) * zoomLevel,
            (-playerPosition.y + visibleSize.height * 0.5) * zoomLevel);

        auto cameraPosition = currentscene->getPosition();
        Vec2 newPosition = cameraPosition + (targetCameraPosition - cameraPosition);
        // Đặt vị trí mới cho camera
        currentscene->setPosition(newPosition);
        if (compass) {
            compass->setPosition(_player->getPosition() + Vec2(0, visibleSize.height / 2 / zoomLevel) - Vec2(0, compass->compassBorder->getContentSize().height * 0.05 * Director::getInstance()->getContentScaleFactor() / 2 + 3.5 * Director::getInstance()->getContentScaleFactor()));
            if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->questStatus == 0) {
                compass->setTarget(_player->getPosition(), listNPC[0]->getPosition());
            }
            else if(QuestManager::getInstance()->currentQuestIndex == 0) {
                if (gameMap->currentMap == 0) {
                    compass->setTarget(_player->getPosition(), Vec2(gameMap->getEndPoint().getMidX(), gameMap->getEndPoint().getMidY()));
                }
                else if (gameMap->currentMap == 2) {
                    compass->setTarget(_player->getPosition(), Vec2(gameMap->getEndPoint().getMidX(), gameMap->getEndPoint().getMidY()));
                }
                else {
                    compass->setTarget(Vec2::ZERO, Vec2::ZERO);
                    if (gameMap->currentMap == 1 && QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->questStatus == 2) {
                        compass->setTarget(_player->getPosition(), listNPC[0]->getPosition());
                    }
                }
            }
            else if (QuestManager::getInstance()->currentQuestIndex == 1) {
                if (gameMap->currentMap == 0) {
                    compass->setTarget(_player->getPosition(), Vec2(gameMap->getEndPoint().getMidX(), gameMap->getEndPoint().getMidY()));
                }
                else if (gameMap->currentMap == 2) {
                    compass->setTarget(_player->getPosition(), Vec2(gameMap->getEndPoint().getMidX(), gameMap->getEndPoint().getMidY()));
                }
                else {
                    compass->setTarget(Vec2::ZERO, Vec2::ZERO);
                    if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->questStatus == 2) {
                        if (gameMap->currentMap == 2) {
                            compass->setTarget(_player->getPosition(), listNPC[0]->getPosition());
                        }
                        else if (gameMap->currentMap == 1) {
                            compass->setTarget(_player->getPosition(), listNPC[0]->getPosition());
                        }
                    }
                }
            }
            else if (QuestManager::getInstance()->currentQuestIndex == 2) {
                if (gameMap->currentMap == 1) {
                    compass->setTarget(_player->getPosition(), Vec2(gameMap->getEndPoint().getMidX(), gameMap->getEndPoint().getMidY()));
                }
                else if (gameMap->currentMap == 0) {
                    compass->setTarget(_player->getPosition(), Vec2(gameMap->getEndPoint().getMidX(), gameMap->getEndPoint().getMidY()));
                }
                else if (gameMap->currentMap == 2 && QuestManager::getInstance()->currentQuest->questStatus == 2) {
                    compass->setTarget(_player->getPosition(), listNPC[0]->getPosition());
                }
                else {
                    compass->setTarget(Vec2::ZERO, Vec2::ZERO);
                }
            }
            else if (QuestManager::getInstance()->currentQuestIndex == 3) {
                if (gameMap->currentMap == 1 || gameMap->currentMap == 2) {
                    compass->setTarget(_player->getPosition(), Vec2(gameMap->getEndPoint().getMidX(), gameMap->getEndPoint().getMidY()));
                }
                else if (gameMap->currentMap == 0) {
                    compass->setTarget(_player->getPosition(), Vec2(gameMap->getEndPoint().getMidX(), gameMap->getEndPoint().getMidY()));
                }
                else {
                    compass->setTarget(Vec2::ZERO, Vec2::ZERO); 
                    if (gameMap->currentMap == 3 && QuestManager::getInstance()->currentQuest->questStatus == 2) {
                        compass->setTarget(_player->getPosition(), listNPC[0]->getPosition());
                    }
                }
            }
        }
    }
}

void Game::showDiedLayer() {
    if (diedLayer && _player) {
        diedLayer->setPosition(_player->getPosition() - visibleSize / 2);
        diedLayer->setVisible(true);
        // hide ui
        
        //Game::pauseGame();
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
            if (diedLayer->isVisible() && diedLayer->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
                diedLayer->tabToContinue();
                diedLayer->setVisible(false);
              //  Game::resumeGame();
                Game::loadMap(0);
                TMXObjectGroup* objGroup = gameMap->getTiledMap()->getObjectGroup("Player");
                ValueMap spawnPoint = objGroup->getObject("Start Point");
                int x = spawnPoint.at("x").asInt();
                int y = spawnPoint.at("y").asInt();
                _player->setPosition(x, y);
                return true;
            }
            return false;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, diedLayer);

    }
};

void Game::updateUserInterface(float dt)
{
    //if ((loadingScene != nullptr && loadingScene->isDeleted == false) || (loadingSceneFirst != nullptr && loadingSceneFirst->isDeleted == false)) {
    //    if(inGameUI->getMovementJoystick()->getIsCanPress())
    //        inGameUI->getMovementJoystick()->setCanPress(false);
    //}
    //else if ((loadingScene == nullptr || loadingScene->isDeleted == true) && (loadingSceneFirst == nullptr || loadingSceneFirst->isDeleted == true)) {
    //    if (!inGameUI->getMovementJoystick()->getIsCanPress())
    //        inGameUI->getMovementJoystick()->setCanPress(false);
    //}
    if (inGameUI && _player) {
        inGameUI->setPosition(_player->getPosition());
    }
}


void Game::setup()
{
    zoomLevel = visibleSize.width / visibleSize.height * 2; 
    Game::loadMap(currentMap);

    BaseItem *crystal1 = ItemFactory::createItem("crystal");
    crystal1->setPosition(_player->getPosition() + Vec2(40, 0));
    crystal1->showLootIndicator();
    gameMap->getTiledMap()->addChild(crystal1, 9.5);

    BaseItem *crystal2 = ItemFactory::createItem("diamond");
    crystal2->showLootIndicator();
    crystal2->setPosition(_player->getPosition() + Vec2(-25, 25));
    gameMap->getTiledMap()->addChild(crystal2, 9.5);

    BaseItem *speedPotion = ItemFactory::createItem("Speed Potion");
    speedPotion->setPosition(_player->getPosition() + Vec2(0, 40));
    speedPotion->showLootIndicator();
    gameMap->getTiledMap()->addChild(speedPotion, 9.5);

    BaseItem *manaPotion = ItemFactory::createItem("Mana Potion");
    manaPotion->setPosition(_player->getPosition() + Vec2(0, 60));
    manaPotion->showLootIndicator();
    gameMap->getTiledMap()->addChild(manaPotion, 9.5);

    BaseItem *healthPotion = ItemFactory::createItem("Health Potion");
    healthPotion->setPosition(_player->getPosition() + Vec2(20, 40));
    healthPotion->showLootIndicator();
    gameMap->getTiledMap()->addChild(healthPotion, 9.5);

    BaseItem *healthPotion2 = ItemFactory::createItem("emerald");
    healthPotion2->setPosition(_player->getPosition() + Vec2(20, 50));
    healthPotion2->showLootIndicator();
    gameMap->getTiledMap()->addChild(healthPotion2, 9.5);

    BaseItem *healthPotion3 = ItemFactory::createItem("topaz");
    healthPotion3->setPosition(_player->getPosition() + Vec2(20, 70));
    healthPotion3->showLootIndicator();
    gameMap->getTiledMap()->addChild(healthPotion3, 9.5);

    BaseItem *scroll1 = ItemFactory::createItem("Scroll Paper");
    scroll1->setPosition(_player->getPosition() + Vec2(20, 70));
    scroll1->showLootIndicator();
    gameMap->getTiledMap()->addChild(scroll1, 9.5);
    
    BaseItem *scroll2 = ItemFactory::createItem("Scroll Paper");
    scroll2->setPosition(_player->getPosition() + Vec2(30, 70));
    scroll2->showLootIndicator();
    gameMap->getTiledMap()->addChild(scroll2, 9.5);

    BaseItem* gon = ItemFactory::createItem("Gold");
    gon->setPosition(_player->getPosition() + Vec2(20, 90));
    gon->showLootIndicator();
    gameMap->getTiledMap()->addChild(gon, 9.5);

    listOfItems.push_back(crystal1);
    listOfItems.push_back(crystal2);
    listOfItems.push_back(speedPotion);
    listOfItems.push_back(manaPotion);
    listOfItems.push_back(healthPotion);
    listOfItems.push_back(healthPotion2);
    listOfItems.push_back(healthPotion3);
    listOfItems.push_back(gon);
    listOfItems.push_back(scroll1);
    listOfItems.push_back(scroll2);

    // ly v tr NPC
    // TMXObjectGroup *NPCobjGroup = gameMap->getTiledMap()->getObjectGroup("NPC");
    // ValueMap durinNPCPoint = NPCobjGroup->getObject("Durin The Blacksmith");
    // int durinX = durinNPCPoint.at("x").asInt();
    // int durinY = durinNPCPoint.at("y").asInt();
}

void Game::showTeleportEffect() {

    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/teleport_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    teleportEffect->setVisible(true);
    teleportEffect->setPosition(_player->getPosition() - Vec2(0, -teleportEffect->getContentSize().height * teleportEffect->getScale() / 4));
}

bool Game::save() {
    JsonManager::getInstance()->saveGame(_player->getPlayerData(), listOfMonster);
    CoinManager::getInstance()->saveCoin();
    JsonManager::getInstance()->saveSkillsData(_player->getPlayerSkills());
    JsonManager::getInstance()->saveItemsData(_player->getAllItem(), _player->getSubInventory()->getSubInventoryItems());
    JsonManager::getInstance()->saveEquipmentsData(_player->getAllEquipment(), _player->getPlayerUsingEquipmentData());
    JsonManager::getInstance()->saveQuest(QuestManager::getInstance()->getCurrentQuestData());
    return true;
}
void Game::setCurrentQuestFromData() {

    Quest::QuestData questFromData = JsonManager::getInstance()->getQuestData();
    Quest* quest_temp;

    //Kiem tra quest nam o vi tri thu may trong List Quest
    int indexQuest = 0;
    bool checkQuest = false;
    for (int i = 0; i < QuestManager::getInstance()->listQuest.size(); i++) {
        if (QuestManager::getInstance()->listQuest[i]->questName == questFromData.questName) {
            quest_temp = QuestManager::getInstance()->listQuest[i];
            indexQuest = i;
            checkQuest = true;
            break;
        }
    }

    if (checkQuest) {
        QuestManager::getInstance()->currentQuestIndex = indexQuest;

        //Get Enemies from Data
        std::vector<Quest::Enemy*> enemies = std::vector<Quest::Enemy*>();
        int enemyIndex = 0;
        for (auto enemy : quest_temp->enemies) {
            if (questFromData.enemies[enemyIndex]->name == enemy->name)
                enemies.push_back(questFromData.enemies[enemyIndex]);
            enemyIndex++;
        }
        //Get Items from Data
        int itemIndex = 0;
        std::vector<Quest::Item*> items = std::vector<Quest::Item*>();
        for (auto item : quest_temp->items) {
            if (questFromData.items[itemIndex]->name == item->name)
                items.push_back(questFromData.items[itemIndex]);
            itemIndex++;
        }

        QuestManager::getInstance()->currentQuest = QuestManager::getInstance()->listQuest[indexQuest];
        QuestManager::getInstance()->currentQuest->enemies = enemies;
        QuestManager::getInstance()->currentQuest->items = items;
        QuestManager::getInstance()->currentQuest->questDialogue = quest_temp->questDialogue;
        QuestManager::getInstance()->currentQuest->processingQuestDialogue = quest_temp->processingQuestDialogue;
        QuestManager::getInstance()->currentQuest->completedQuestDialogue = quest_temp->completedQuestDialogue;
        QuestManager::getInstance()->currentQuest->questStatus = questFromData.questStatus;
        QuestManager::getInstance()->currentQuest->npc = quest_temp->npc;
        QuestManager::getInstance()->currentQuest->setName("CurrentQuest");
        QuestManager::getInstance()->currentQuest->retain();

        QuestManager::getInstance()->changeQuestIconOfNPC(QuestManager::getInstance()->currentQuest->questStatus);

        if (QuestManager::getInstance()->currentQuest->questStatus != QuestManager::getInstance()->currentQuest->NOT_RECEIVED) {
            if (QuestManager::getInstance()->currentQuest != nullptr)QuestManager::getInstance()->currentQuest->removeFromParentAndCleanup(true);

            QuestManager::getInstance()->addChild(QuestManager::getInstance()->currentQuest, 1);
            QuestManager::getInstance()->generalDisplay();

            //Update Quest tabel label progress
            //Kill Enemy Quest
            if (QuestManager::getInstance()->currentQuest->enemies.size() > 0) {
                for (auto progressLabel : QuestManager::getInstance()->currentQuest->progressLabelList) {
                    for (auto enemy : QuestManager::getInstance()->currentQuest->enemies) {
                        if (progressLabel->getName() == enemy->name + " Label")
                            progressLabel->setString(StringUtils::format("%s: %d / %d", enemy->name.c_str(), enemy->killed, enemy->required));
                    }
                }
            }
            //Kill Enemy Quest
            if (QuestManager::getInstance()->currentQuest->items.size() > 0) {
                for (auto progressLabel : QuestManager::getInstance()->currentQuest->progressLabelList) {
                    for (auto item : QuestManager::getInstance()->currentQuest->items) {
                        if (progressLabel->getName() == item->name + " Label")
                            progressLabel->setString(StringUtils::format("%s: %d / %d", item->name.c_str(), item->collected, item->required));
                    }
                }
            }

        }
    }
    else {
        QuestManager::getInstance()->changeQuestIconOfNPC(10);
        QuestManager::getInstance()->currentQuest = nullptr;
    }
}
Game::~Game() {
    //unscheduleAllCallbacks();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this); // Hy tt c cc event listener ca i tng ny
    if (_player) {
        _player->removeFromParentAndCleanup(true);
        _player = nullptr;
    }

    if (gameMap) {
        gameMap->getTiledMap()->removeFromParentAndCleanup(true); // Xa bn  v tt c cc i tng con ca n
        gameMap = nullptr;
    }

    // Xa danh sch cc i tng vt l
    for (auto node : listOfCollision) {
        node->removeFromParentAndCleanup(true);
    }
    listOfCollision.clear();

    // Xa danh sch cc i tng vt l
    for (auto node : listOfHideObject) {
        node->removeFromParentAndCleanup(true);
    }
    listOfHideObject.clear();

    // Xa danh sch cc i tng vt l
    for (auto node : listOfSafeZone) {
        node->removeFromParentAndCleanup(true);
    }
    listOfSafeZone.clear();

    // Xa danh sch cc item
    for (auto item : listOfItems) {
        item->removeFromParentAndCleanup(true);
    }
    listOfItems.clear();

    // Xa danh sch cc qui vt
    for (auto monster : listOfMonster) {
        monster->removeFromParentAndCleanup(true);
    }
    listOfMonster.clear();

    //// Xa danh sch cc qui vt
    //for (auto boss : listOfBoss) {
    //    boss->removeFromParentAndCleanup(true);
    //}
    //listOfBoss.clear();

    // Xa danh sch NPC
    for (auto npc : listNPC) {
        npc->removeFromParentAndCleanup(true);
    }
    listNPC.clear();

    // Xa i tng InGameUI
    if (inGameUI) {
        inGameUI->removeFromParentAndCleanup(true);
        inGameUI = nullptr;
    }
    //// Xa i tng LoadingScene
    //if (loadingScene) {
    //    loadingScene->removeFromParentAndCleanup(true);
    //    loadingScene = nullptr;
    //}

    //// Xa i tng LoadingScene (dng trong hm updatePlayer)
    if (loadingSceneFirst) {
        loadingSceneFirst->removeFromParentAndCleanup(true);
        loadingSceneFirst = nullptr;
    }
}

void Game::moveCameraToBoss() {
    //
    Scene* currentscene = Director::getInstance()->getRunningScene();
    Game* game = dynamic_cast<Game*>(currentscene->getChildByName("GameInstance"));
    //
    Vec2 playerPos = Vec2((-_player->getPosition().x + visibleSize.width * 0.5) * zoomLevel,
        (-_player->getPosition().y + visibleSize.height * 0.5) * zoomLevel);
    Vec2 bossPos = Vec2((-boss->getPosition().x + visibleSize.width * 0.5) * zoomLevel,
        (-boss->getPosition().y + visibleSize.height * 0.5) * zoomLevel);
    auto sqe = Sequence::create(DelayTime::create(1.5f), cocos2d::CallFunc::create([this]() {
                unschedule(CC_SCHEDULE_SELECTOR(Game::updateCamera));
                inGameUI->setVisible(false);
                Scene* currentscene = Director::getInstance()->getRunningScene();
                Game* game = dynamic_cast<Game*>(currentscene->getChildByName("GameInstance"));
                if (game->gameMap && game->gameMap->getTiledMap()) {
                    if (game->gameMap->getTiledMap()->getLayer("topgrass") && game->gameMap->getTiledMap()->getLayer("topgrass")->isVisible() == true)
                        game->gameMap->getTiledMap()->getLayer("topgrass")->setVisible(false);
                    if (game->gameMap->getTiledMap()->getLayer("topwall") && game->gameMap->getTiledMap()->getLayer("topwall")->isVisible() == true)
                        game->gameMap->getTiledMap()->getLayer("topwall")->setVisible(false);
                    if (game->gameMap->getTiledMap()->getLayer("topshadow") && game->gameMap->getTiledMap()->getLayer("topshadow")->isVisible() == true)
                        game->gameMap->getTiledMap()->getLayer("topshadow")->setVisible(false);
                }}),
            MoveTo::create(6.0f, bossPos),
        cocos2d::CallFunc::create([this]() {
            boss->SwitchState(boss->roarState);
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/terra_roar.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            }), 
            DelayTime::create(3.0f), MoveTo::create(3.0f, playerPos),
            cocos2d::CallFunc::create([this]() {
            schedule(CC_SCHEDULE_SELECTOR(Game::updateCamera), 0.0f);
            inGameUI->setVisible(true);
            Scene* currentscene = Director::getInstance()->getRunningScene();
            Game* game = dynamic_cast<Game*>(currentscene->getChildByName("GameInstance"));
            if (game->gameMap && game->gameMap->getTiledMap()) {
                if (game->gameMap->getTiledMap()->getLayer("topgrass") && game->gameMap->getTiledMap()->getLayer("topgrass")->isVisible() == false)
                    game->gameMap->getTiledMap()->getLayer("topgrass")->setVisible(true);
                if (game->gameMap->getTiledMap()->getLayer("topwall") && game->gameMap->getTiledMap()->getLayer("topwall")->isVisible() == false)
                    game->gameMap->getTiledMap()->getLayer("topwall")->setVisible(true);
                if (game->gameMap->getTiledMap()->getLayer("topshadow") && game->gameMap->getTiledMap()->getLayer("topshadow")->isVisible() == false)
                    game->gameMap->getTiledMap()->getLayer("topshadow")->setVisible(true);

            }
                }),
            nullptr);

    currentscene->runAction(sqe);

        

}