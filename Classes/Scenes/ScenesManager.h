#ifndef __SCENE_MANAGER__
#define __SCENE_MANAGER__

#include "cocos2d.h"
#include "Game/Game.h"
USING_NS_CC;
class Game;
class SceneManager {
public:
    static SceneManager* getInstance();
    void pushScene(cocos2d::Scene* scene);
    void popScene();
    void replaceScene(cocos2d::Scene* scene);
    void runMainMenuScene();
    void runGameScene();
    void runPauseMenuScene();
    void runTutorialScene();
    Scene* getCurrentScene() const { return currentScene; };
    Game* game;
private:
    static SceneManager* instance;
    std::vector<cocos2d::Scene*> sceneStack;
    Scene* currentScene;
    Scene* gameScene = nullptr;
    Scene* pauseScene = nullptr;
    Scene* mainMenuScene = nullptr;
    Scene* tutorialScene = nullptr;
    Scene* storyScene = nullptr;
};


#endif // __SPLASH_H__