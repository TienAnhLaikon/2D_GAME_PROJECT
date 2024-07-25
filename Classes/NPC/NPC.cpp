#include "NPC.h"
#include "HelloWorldScene.h"
#include "Game/Game.h"
#include "Map/Map.h"

bool NPC::init() {
    return true;
}
void NPC::detectPlayerCharacter()
{
    // V tr ca boss
    Vec2 bossPosition = this->getPosition();

    // Ly scene chnh t Director
    Scene* currentScene = Director::getInstance()->getRunningScene();
    if (currentScene)
    {
        Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
        if (game)
        {
            auto children = game->gameMap->getTiledMap()->getChildren();
            for (const auto& child : children)
            {
                // Kim tra xem i tng con c phi l PlayerCharacter khng
                auto playerCharacter = dynamic_cast<Player*>(child);
                if (playerCharacter)
                {

                    // Tnh khong cch gia boss v playerCharacter
                    Vec2 playerPosition = playerCharacter->getPosition();
                    float distance = bossPosition.distance(playerPosition);

                    // Nu playerCharacter nm trong phm vi pht hin ca boss
                    if (distance <= 45)
                    {
                        setTarget(playerCharacter);
                        return;
                    }
                }
            }
        }
    }
    setTarget(nullptr);
}
void NPC::updateTarget(float dt) {
    detectPlayerCharacter();
}

