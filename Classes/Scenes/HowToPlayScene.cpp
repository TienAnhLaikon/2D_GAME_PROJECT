//#include "Game.h"
//#include "Skills/SkillFactory.h"
//#include "Skills/SkillBase.h"
//#include "UserInterface/InGame/Minimap.h"
//#include "UserInterface/InGame/InGameUI.h"
//#include "MapDecoration/MapDecoration.h"
//#include "Map/Map.h"
//#include "Enemies/NormalMonster/NormalMonsterFactory.h"
//
//Scene *Game::createGame()
//{
//    auto scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//    scene->getPhysicsWorld()->setGravity(Vec2(0, -98));
//    scene->getPhysicsWorld()->setSubsteps(10);
//    auto layer = Game::create();
//    layer->setName("GameInstance");
//    layer->setPhysicsWorld(scene->getPhysicsWorld());
//    scene->addChild(layer);
//    return scene;
//}
//
//bool Game::init()
//{
//    if (!Scene::initWithPhysics())
//    {
//        return false;
//    }
//
//    visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    // setup
//    Game::setup();
//
//
//
//    schedule(CC_SCHEDULE_SELECTOR(Game::updatePlayer), 0.06f);
//    schedule(CC_SCHEDULE_SELECTOR(Game::updateEnemies), 0.06f);
//    schedule(CC_SCHEDULE_SELECTOR(Game::updateUserInterface), 0.0f);
//    schedule(CC_SCHEDULE_SELECTOR(Game::updateCamera), 0.0f);
//
//    // Lv test
//    auto mouseListener = cocos2d::EventListenerMouse::create();
//    mouseListener->onMouseDown = [this](cocos2d::EventMouse *event)
//    {
//        //_player->gainExp(100);
//
//        //CCLOG("LEVEL: %d", _player->getLevel());
//        //CCLOG("Current Exp: %f", _player->getCurrentExp());
//        //CCLOG("ExpRequiredToLevelUp: %f", _player->getExpRequiredToLevelUp());
//
//        //CCLOG("CURRENT LEVEL: %d", _player->getLevel());
//        //CCLOG("Current HP: %f", _player->getCurrentHP());
//        //CCLOG("Current MP: %f", _player->getCurrentMP());
//        //CCLOG("HP regen: %f", _player->getHPRegen());
//        //CCLOG("MP regen: %f", _player->getMPRegen());
//        //CCLOG("Armor: %f", _player->getArmor());
//        //CCLOG("AD: %f", _player->getDamage());
//        //CCLOG("AS: %f", _player->getAttackSpeed());
//        //CCLOG("AR: %f", _player->getAttackRange());
//        //CCLOG("MS: %f", _player->getMS());
//
//        };
//    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
//    return true;
//}
//
//void Game::updatePlayer(float dt)
//{
//    if (inGameUI && inGameUI->getMovementJoystick())
//    {
//        _player->movementDirection = inGameUI->getMovementJoystick()->getDirection();
//        if (inGameUI->getMovementJoystick()->getIsPress())
//        {
//            _player->handleMove(dt);
//        }
//        else
//        {
//            _player->setMoving(false);
//        }
//    }
//    //if ( robert->getParent()) robert->currentState->UpdateState();
//    TMXObjectGroup* objGroup = gameMap->getTiledMap()->getObjectGroup("Player");
//    ValueMap spawnPoint = objGroup->getObject("End Point");
//    int x = spawnPoint.at("x").asInt();
//    int y = spawnPoint.at("y").asInt();
//    auto endPoint = Rect(x, y, 50, 50);
//    if (objGroup && endPoint.containsPoint(_player->getPosition())) {
//        currentMap = 1;
//        Game::loadMap(currentMap);
//    }
//    for (auto item : listOfItems)
//    {
//        auto playerRect = Rect(_player->getPositionX(), _player->getPositionY(), 30, 60);
//        if(playerRect.containsPoint(item->getPosition())) {
//            _player->pickUpItem(item->getItemName());
//            listOfItems.erase(std::remove(listOfItems.begin(), listOfItems.end(), item), listOfItems.end());
//            item->removeFromParentAndCleanup(true);
//        }
//    }
//}
//
//
//bool Game::loadMap(int levelMap) {
//    if (gameMap)
//    {
//
//        std::vector<BaseItem*> itemsToRemove;
//        for (auto item : listOfItems)
//        {
//            if (item)
//            {
//                item->removeFromParentAndCleanup(true);
//                itemsToRemove.push_back(item);
//            }
//        }
//        for (auto item : itemsToRemove)
//        {
//            auto it = std::find(listOfItems.begin(), listOfItems.end(), item);
//            if (it != listOfItems.end())
//            {
//                listOfItems.erase(it);
//            }
//        }
//        _player->removeFromParent();
//        gameMap->getTiledMap()->removeFromParentAndCleanup(true);
//        gameMap->~GameMap();
//
//        std::vector<Node*> nodesToRemove;
//        for (auto node : listOfCollision) {
//            nodesToRemove.push_back(node);
//        }
//        for (auto node : nodesToRemove) {
//            node->removeFromParent();
//        }
//        listOfCollision.clear();
//
//
//        this->removeChildByName("minimap");
//        this->removeChildByName("map");
//        this->removeChildByName("ingameui");
//    }
//    gameMap = GameMap::getInstance(levelMap);
//    gameMap->getTiledMap()->setName("map");
//    this->addChild(gameMap->getTiledMap());
//
//    minimap = MinimapLayer::create(gameMap->getMapName(), _player);
//    minimap->setScale(minimap->MINIMAP_SCALE_FACTOR);
//    minimap->setName("minimap");
//    this->addChild(minimap);
//    
//    // collision physics
//    auto objectLayer = gameMap->getTiledMap()->getObjectGroup("Collision");
//
//    if (objectLayer) {
//        const auto& objects = objectLayer->getObjects();
//        for (const auto& object : objects) {
//            ValueMap objectProps = object.asValueMap();
//            float x = objectProps["x"].asFloat();
//            float y = objectProps["y"].asFloat();
//            float width = objectProps["width"].asFloat();
//            float height = objectProps["height"].asFloat();
//            Node* node = Node::create();
//            node->setName("Wall");
//            node->setContentSize(Size(width, height));
//            node->setPosition(x, y);
//            listOfCollision.push_back(node);
//            this->addChild(node);
//        }
//    }
//
//    TMXObjectGroup *NPCobjGroup = gameMap->getTiledMap()->getObjectGroup("NPC");
//    // Durin
//    ValueMap durinNPCPoint = NPCobjGroup->getObject("Durin The Blacksmith");
//    int durinX = durinNPCPoint.at("x").asInt();
//    int durinY = durinNPCPoint.at("y").asInt();
//
//    // Thien An
//    //ValueMap ThienAnNPCPoint = NPCobjGroup->getObject("Thien An The First Slayer");
//    //int thienAnX = ThienAnNPCPoint.at("x").asInt();
//    //int thienAnY = ThienAnNPCPoint.at("y").asInt();
//
//    TMXObjectGroup* objGroup = gameMap->getTiledMap()->getObjectGroup("Player");
//    ValueMap spawnPoint = objGroup->getObject("Start Point");
//    int x = spawnPoint.at("x").asInt();
//    int y = spawnPoint.at("y").asInt();
//    
//    _player = Player::create();
//    _player->setPosition(Vec2(x, y));
//    _player->listOfCollision = this->listOfCollision;
//    _player->currentState = _player->idleState;
//    _player->currentState->EnterState();
//    _player->setTiledMap(gameMap);
//    gameMap->getTiledMap()->addChild(_player, 9.5);
//
//    inGameUI = InGameUI::getInstance(_player);
//    inGameUI->setAnchorPoint(Vec2(0, 0));
//    inGameUI->setScale(1);
//    inGameUI->setName("ingameui");
//    this->addChild(inGameUI, 50);
//
//    auto objectGroup = gameMap->getTiledMap()->getObjectGroup("Enemies");
//    if (objectGroup) {
//        auto objects = objectGroup->getObjects();
//        for (const auto& object : objects) {
//            auto objectMap = object.asValueMap();
//            std::string objectName = objectMap["name"].asString();
//            if (objectName == "skeleton") {
//                enemy1 = NormalMonsterFactory::createNormalMonster("Skeleton");
//                float objectX = objectMap["x"].asFloat();
//                float objectY = objectMap["y"].asFloat();
//                enemy1->setTarget(_player);
//                enemy1->setPosition(Vec2(objectX, objectY));
//                gameMap->getTiledMap()->addChild(enemy1, 9.5);
//                enemy1->currentState = enemy1->walkState;
//                enemy1->currentState->EnterState();
//            }
//            else if (objectName == "BabySpider") {
//                spider3 = NormalMonsterFactory::createNormalMonster("BabySpider");
//                float objectX = objectMap["x"].asFloat();
//                float objectY = objectMap["y"].asFloat();
//                spider3->setTarget(_player);
//                spider3->setPosition(objectX, objectY);
//                gameMap->getTiledMap()->addChild(spider3, 9.5);
//                spider3->currentState = spider3->walkState;
//                spider3->currentState->EnterState();
//            }
//            else if (objectName == "terra") {
//                boss = BossFactory::createBoss("HellBeast");
//                float objectX = objectMap["x"].asFloat();
//                float objectY = objectMap["y"].asFloat();
//                boss->setPosition(Vec2(objectX, objectY));
//                boss->setName("HellBeast");
//                gameMap->getTiledMap()->addChild(boss, 9.5);
//                boss->currentState = boss->roarState;
//                boss->currentState->EnterState();
//            }
//        }
//    }
//
//
//
//
//    //// BOSS
//    //boss = BossFactory::createBoss("HellBeast");
//    //boss->setPosition(Vec2(900, 500));
//    //boss->setName("HellBeast");
//    //gameMap->getTiledMap()->addChild(boss, 9.5);
//    //boss->currentState = boss->roarState;
//    //boss->currentState->EnterState();
//
//    //// OKYANUS BOSS
//    //okyanus = BossFactory::createBoss("Okyanus");
//    //okyanus->setPosition(Vec2(500, 500));
//    //okyanus->setName("Okyanus");
//    //gameMap->getTiledMap()->addChild(okyanus, 9.5);
//    //okyanus->currentState = okyanus->idleState;
//    //okyanus->currentState->EnterState();
//    //
//    //// Paradiso boss 
//    //paradiso = BossFactory::createBoss("Paradiso");
//    //paradiso->setPosition(Vec2(300, 500));
//    //paradiso->setName("Paradiso");
//    //gameMap->getTiledMap()->addChild(paradiso, 9.5);
//    //paradiso->currentState = paradiso->idleState;
//    //paradiso->currentState->EnterState();
//
//
//    // NPC Durin
//    durin = NPCFactory::createNPC("Durin The Blacksmith");
//    durin->setPosition(Vec2(durinX, durinY));
//    durin->setName("Durin");
//    this->addChild(durin);
//    durin->currentState = durin->idleState;
//    durin->currentState->EnterState();
//
//    // NPC Thien An
//    //thienAn = NPCFactory::createNPC("Thien An The First Slayer");
//    //thienAn->setPosition(Vec2(thienAnX, thienAnY));
//    //thienAn->setName("ThienAn");
//    //this->addChild(thienAn);
//    //thienAn->currentState = thienAn->idleState;
//    //thienAn->currentState->EnterState();
//
//
//    ////0
//    //enemy = NormalMonsterFactory::createNormalMonster("Slime");
//    //enemy->setTarget(_player);
//    //enemy->setPosition(Vec2(visibleSize.width / 2 + 100, visibleSize.height / 2));
//    //gameMap->getTiledMap()->addChild(enemy, 9.5);
//
//    //enemy->currentState = enemy->walkState;
//    //enemy->currentState->EnterState();
//    ////1
//    //enemy1 = NormalMonsterFactory::createNormalMonster("Skeleton");
//    //enemy1->setTarget(_player);
//    //enemy1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
//    //gameMap->getTiledMap()->addChild(enemy1, 9.5);
//
//    //enemy1->currentState = enemy1->walkState;
//    //enemy1->currentState->EnterState();
//    ////2
//    //enemy2 = NormalMonsterFactory::createNormalMonster("Skeleton");
//    //enemy2->setTarget(_player);
//    //enemy2->setPosition(Vec2(visibleSize.width / 2 + 100, visibleSize.height / 2 + 100));
//    //gameMap->getTiledMap()->addChild(enemy2, 9.5);
//
//    //enemy2->currentState = enemy2->walkState;
//    //enemy2->currentState->EnterState();
//
//    //spider1 = NormalMonsterFactory::createNormalMonster("BabySpider");
//    //spider1->setTarget(_player);
//    //spider1->setPosition(250,350);
//    //gameMap->getTiledMap()->addChild(spider1, 9.5);
//    //spider1->currentState = spider1->walkState;
//    //spider1->currentState->EnterState();
//
//    //spider2 = NormalMonsterFactory::createNormalMonster("BabySpider");
//    //spider2->setTarget(_player);
//    //spider2->setPosition(300, 100);
//    //gameMap->getTiledMap()->addChild(spider2, 9.5);
//    //spider2->currentState = spider2->walkState;
//    //spider2->currentState->EnterState();
//    //
//    //spider3 = NormalMonsterFactory::createNormalMonster("BabySpider");
//    //spider3->setTarget(_player);
//    //spider3->setPosition(100, 220);
//    //gameMap->getTiledMap()->addChild(spider3, 9.5);
//    //spider3->currentState = spider3->walkState;
//    //spider3->currentState->EnterState();
//
//    //auto en6 = NormalMonsterFactory::createNormalMonster("Angelos");
//    //angel1 = dynamic_cast<Angelos*>(en6);
//    //angel1->setTarget(_player);
//    //angel1->setPosition(300, 100);
//    //gameMap->getTiledMap()->addChild(angel1, 9.5);
//    //angel1->currentState = angel1->walkState;
//    //angel1->currentState->EnterState();
//
//    //auto en7 = NormalMonsterFactory::createNormalMonster("ShadowSentinels");
//    //sentinel1 = dynamic_cast <ShadowSentinels*>(en7);
//    //sentinel1->setTarget(_player);
//    //sentinel1->setPosition(200, 300);
//    //gameMap->getTiledMap()->addChild(sentinel1, 9.5);
//    //sentinel1->currentState = sentinel1->walkState;
//    //sentinel1->currentState->EnterState();
//
//
//    return true;
//}
//
//void Game::updateEnemies(float dt)
//{
//    /*enemy->currentState->UpdateState();
//    enemy1->currentState->UpdateState();
//    enemy2->currentState->UpdateState();
//    boss->currentState->UpdateState();
//    okyanus->currentState->UpdateState();
//    paradiso->currentState->UpdateState();
//    spider1->currentState->UpdateState();
//    spider2->currentState->UpdateState();
//    spider3->currentState->UpdateState();*/
//    durin->currentState->UpdateState();
//    //thienAn->currentState->UpdateState();
//    //angel1->currentState->UpdateState();
//    //sentinel1->currentState->UpdateState();
//}
//
//void Game::updateCamera(float deltaT)
//{
//    float movementspeed = _player->getMovementSpeed();
//    Vec2 newposition = _player->getPosition();
//    Scene *currentscene = Director::getInstance()->getRunningScene();
//    currentscene->setScale(zoomLevel);
//    Vec2 cameraposition = Vec2((-newposition.x + visibleSize.width * 0.5) * zoomLevel,
//                               (-newposition.y + visibleSize.height * 0.5) * zoomLevel);
//    currentscene->setPosition(cameraposition);
//    if(minimap)
//        minimap->setPosition((cameraposition * -1 / zoomLevel) + (Vec2(visibleSize.width / 3.5, visibleSize.height / 6.2) / zoomLevel));
//}
//
//void Game::updateUserInterface(float dt)
//{
//    if(inGameUI) inGameUI->setPosition(_player->getPosition());
//}
//
//void Game::setup()
//{
//    zoomLevel = (visibleSize.width / visibleSize.height * 2);
//
//    Game::loadMap(0);
//
//    _player->pickUpEquipment("Chipped Sword", 1);
//    _player->pickUpEquipment("Long Sword", 1);
//    _player->pickUpEquipment("Chipped Armor", 1);
//    _player->pickUpEquipment("Chipped Ring", 1);
//    _player->pickUpEquipment("Chipped Shoe", 1);
//    _player->pickUpEquipment("Chipped Gloves", 1);
//    _player->pickUpEquipment("Chipped Helmet", 1);
//
//    BaseItem* crystal1 = ItemFactory::createItem("crystal");
//    crystal1->setPosition(_player->getPosition() + Vec2(80, 0));
//    crystal1->showLootIndicator();
//    gameMap->getTiledMap()->addChild(crystal1, 9.5);
//
//    BaseItem* crystal2 = ItemFactory::createItem("crystal");
//    crystal2->showLootIndicator();
//    crystal2->setPosition(_player->getPosition() + Vec2(-50, 50));
//    gameMap->getTiledMap()->addChild(crystal2, 9.5);
//
//    BaseItem* crystal3 = ItemFactory::createItem("crystal");
//    crystal3->setPosition(_player->getPosition() + Vec2(0, 80));
//    crystal3->showLootIndicator();
//    gameMap->getTiledMap()->addChild(crystal3, 9.5);
//
//    listOfItems.push_back(crystal1);
//    listOfItems.push_back(crystal2);
//    listOfItems.push_back(crystal3);
//
//    //MapDecoration* mapDecoration = MapDecoration::getInstance(gameMap->getTiledMap());
//  /*  MapDecoration::getInstance()->loadDecoration("flames");
//    MapDecoration::getInstance()->loadDecoration("subservients working");
//    MapDecoration::getInstance()->loadDecoration("torch");
//    MapDecoration::getInstance()->loadDecoration("Obelisk");
//    MapDecoration::getInstance()->loadDecoration("End Point");
//    MapDecoration::getInstance()->loadDecoration("BloodMoon Tower");*/
//
//    //TMXObjectGroup *NPCobjGroup = gameMap->getTiledMap()->getObjectGroup("NPC");
//    //ValueMap durinNPCPoint = NPCobjGroup->getObject("Durin The Blacksmith");
//    //int durinX = durinNPCPoint.at("x").asInt();
//    //int durinY = durinNPCPoint.at("y").asInt();
//
//    
//}