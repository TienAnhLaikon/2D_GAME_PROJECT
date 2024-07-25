#include "ScenesManager.h"
#include "Game/Game.h"
#include "MainMenuScene.h"
#include "PauseMenuScene.h"
#include "TutorialScene.h"
#include "AnimaticScene.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::getInstance() {
    if (!instance) {
        instance = new SceneManager();
    }
    return instance;
}

void SceneManager::pushScene(cocos2d::Scene* scene) {
    cocos2d::Director::getInstance()->pushScene(scene);
    sceneStack.push_back(scene);
}

void SceneManager::popScene() {
    if (!sceneStack.empty()) {
        auto scene = sceneStack.back();
        cocos2d::Director::getInstance()->popScene();
        sceneStack.pop_back();
        scene->release(); 
    }
}


void SceneManager::replaceScene(cocos2d::Scene* scene) {
    if (!sceneStack.empty()) {
        cocos2d::Director::getInstance()->replaceScene(scene);
        sceneStack.pop_back();
        sceneStack.push_back(scene);
    }
}

void SceneManager::runMainMenuScene() {
    auto scene = MainMenuScene::create();
    cocos2d::Director::getInstance()->popToRootScene();
    pushScene(scene);
    currentScene = scene;

}

void SceneManager::runGameScene() {
    auto scene = Game::createGame();
    cocos2d::Director::getInstance()->pushScene(scene);
    currentScene = scene;
}

void SceneManager::runPauseMenuScene() {
    if (currentScene) {
        Scene* currentScene = Director::getInstance()->getRunningScene();
        game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
        if(game && currentScene)
            game->pauseGame();
    }
    gameScene = currentScene;
    auto scene = PauseMenuScene::create();
    cocos2d::Director::getInstance()->pushScene(scene); 
    currentScene = scene;
}

void SceneManager::runTutorialScene()
{
    mainMenuScene = currentScene;
    auto scene = TutorialScene::create();
    cocos2d::Director::getInstance()->pushScene(scene);
    currentScene = scene;
}
