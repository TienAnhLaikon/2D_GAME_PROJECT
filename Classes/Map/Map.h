#ifndef _MAP_H_
#define _MAP_H_
#include "cocos2d.h";
#include "Game/Game.h";
#include "Engine/Engine.h"
#include <queue>
USING_NS_CC;

static std::queue<int> PREVIOUS_MAP;
class Minimap;
class GameMap {
public:
	static GameMap* getInstance(int mapIndex = 0);
	TMXTiledMap* getTiledMap() { return _tileMap; };
	std::string getMapName() const { return _mapName; };
	bool loadMap(int mapIndex);
	TMXLayer* getGroundGrassLayer() const { return _groundGrass; };
	TMXLayer* getCliffLayer() const { return _cliff; };
	TMXLayer* getRockLayer() const { return _rock; };
	TMXLayer* getTreeLayer() const { return _tree; };
	TMXLayer* getGroundLayer() const { return _ground; };
	Size getMapSize() const { return _mapSize; };
	Size getTileSize() const { return _tileSize; };
	std::vector<Node*> listOfPhysics;
	GameMap(int mapIndex);
	bool isLoadedSuccessfully() const { return isResourcesLoaded; };
	Rect getEndPoint();
	Rect getPrevPoint();
	int getCurMap() const { return currentMap; };
	int currentMap = -1;
	void loadResource(int mapIndex);
	virtual ~GameMap();
private:
	std::queue<int> previousMapQueue;
	int prevMap = 0;
	bool isResourcesLoaded = false;
	Size visibleSize;
	static GameMap* instance;
	TMXTiledMap* _tileMap;
	TMXLayer* _groundGrass;
	TMXLayer* _cliff;
	TMXLayer* _rock;
	TMXLayer* _tree;
	TMXLayer* _ground;
	TMXLayer* _objectGroup;
	Size _mapSize;
	Size _tileSize;
	std::string _mapName;
	int _mapIndex;
	
	void loadNPCFighterToCache();
	void loadOkyanusToCache();
	void loadParadisoToCache();
	void loadHellBeastToCache();
	void loadShadowSentinels();
	void loadAngelosToCache();
	void loadBabySpiderToCache();
	void loadSlimeToCache();
	void loadSkeletonToCache();
	void loadKnightToCache();
	void loadWarriorToCache();
	void loadEffectToCache();
};

#endif // !_MAP_H_
