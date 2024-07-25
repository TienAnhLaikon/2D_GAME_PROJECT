#ifndef _MAPDECORATION_H_
#define _MAPDECORATION_H_
#include "cocos2d.h";
#include "Game/Game.h";
#include "Engine/Engine.h"
#include "MapDecoration/Obelisk.h";
#include "MapDecoration/BloodMoonTower.h"

USING_NS_CC;

class MapDecoration {
public:
	static MapDecoration* getInstance(TMXTiledMap* _tileMap = nullptr);
	static void destroyInstance();
	bool loadDecoration(std::string objectLayerName);
	void loadFlamesVFXToCache();
	void loadSubservientWorkingToCache();
	void loadTorchFlameToCache();
	void loadTeleportEffectsToCache();
private:
	Animate* flamesAnimate;
	Sprite* flamesSprite;
	
	Animate* torchFlamesAnimate;
	Sprite* torchFlamesSprite;

	Animate* teleport_effects;
	Sprite* teleport_effects_sprite;

	Animate* subservientsWorking_E_;
	Animate* subservientsWorking_N_;
	Animate* subservientsWorking_NE_;
	Animate* subservientsWorking_NEE_;
	Animate* subservientsWorking_NNE_;
	Animate* subservientsWorking_NNW_;
	Animate* subservientsWorking_NW_;
	Animate* subservientsWorking_NWW_;
	Animate* subservientsWorking_S_;
	Animate* subservientsWorking_SE_;
	Animate* subservientsWorking_SEE_;
	Animate* subservientsWorking_SSE_;
	Animate* subservientsWorking_SSW_;
	Animate* subservientsWorking_SW_;
	Animate* subservientsWorking_SWW_;
	Animate* subservientsWorking_W_;
	Sprite* subservientsWorkingSprite;


	Animate* torchAnimate;
	Sprite* torchSprite;

	MapDecoration(TMXTiledMap* _tileMap);
	MapDecoration();
	virtual ~MapDecoration();
	static MapDecoration* instance;
	TMXTiledMap* _tileMap;
	Vector <Sprite*> listMapDecoration;
	Vector <Obelisk*> listObelisk;
	Vector<BloodMoonTower*> listBloodMoon;
};

#endif // !_MAPDECORATION_H_
