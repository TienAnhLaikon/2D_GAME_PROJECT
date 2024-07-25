#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "./Player/Player.h"
#include "./Enemies/Enemy.h"
#include "./Game/Game.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "WorkingWithData/SQLITE/sqliteManager.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref *pSender);
    void update(float dt);
    void saveCacheStateToFile(const std::string& filename);
    void loadCacheStateFromFile(const std::string& filename);
    void onGameExit();
    void onGameStart();
    Game *game;
    cocos2d::PhysicsWorld* world;
    CREATE_FUNC(HelloWorld);

private:
    void setPhysicsWorld(cocos2d::PhysicsWorld *m_world)
    {
        m_world = world;
    }
    Player *Player;
    Enemy *enemy;
};

#endif // __HELLOWORLD_SCENE_H__
