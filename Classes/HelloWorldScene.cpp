#include "HelloWorldScene.h"
#include "./Inventory/Inventory.h"
#include "Scenes/SplashScene.h"
USING_NS_CC;

Scene *HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
   // SQLiteManager::getInstance();
    return scene;
}

static void problemLoading(const char *filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto scene = SplashScene::create();
    this->addChild(scene);
    //loadKnightToCache();
    //loadHellBeastToCache();
    //loadOkyanusToCache();
    //loadSlimeToCache();
    //loadSkeletonToCache();
    //loadBabySpiderToCache();
    //loadAngelosToCache();
    //loadShadowSentinels();
    //loadNPCFighterToCache();
    //loadParadisoToCache();
   /* game = Game::create();
    if (game)
    {
        this->addChild(game);
    }*/

    return true;
}

void HelloWorld::menuCloseCallback(Ref *pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::update(float dt)
{
}

void HelloWorld::saveCacheStateToFile(const std::string& filename) {
}

// Load cache state from a file
void HelloWorld::loadCacheStateFromFile(const std::string& filename) {

}

void HelloWorld::onGameExit() {
    saveCacheStateToFile("cache_state.json");
}

void HelloWorld::onGameStart() {
    loadCacheStateFromFile("cache_state.json");
}
// Knight
