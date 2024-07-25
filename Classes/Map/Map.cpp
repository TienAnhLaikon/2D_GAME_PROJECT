#include "./Map.h"
#include <queue>
GameMap* GameMap::instance = nullptr;


GameMap* GameMap::getInstance(int mapIndex) {
	if (!instance) {
		instance = new GameMap(mapIndex);
		instance->visibleSize = Director::getInstance()->getVisibleSize();
	}
	return instance;
}
GameMap::GameMap(int mapIndex) : _mapIndex(mapIndex) {
	_tileMap = new TMXTiledMap();
	GameMap::loadMap(mapIndex);
}

bool GameMap::loadMap(int mapIndex)
{
    if (_tileMap) {
        delete _tileMap;
        _tileMap = nullptr;
        _tileMap = new TMXTiledMap();
    }
    if (mapIndex > 3) {
        return false;
    }
    if (currentMap == mapIndex) return true;
	 _mapName = "map/tilemap0" + std::to_string(mapIndex) + ".tmx";
     currentMap = mapIndex;


	if (_tileMap->initWithTMXFile(_mapName)) {
		_mapSize = _tileMap->getMapSize();
		_tileSize = _tileMap->getTileSize();
        loadResource(mapIndex);
        UserDefault::getInstance()->setBoolForKey("IsLoadedGame", true);
		return true;
	 }

	return false;
}

void GameMap::loadResource(int mapIndex)
{
    if (!UserDefault::getInstance()->getBoolForKey("KnightInCache", false)) {
        loadKnightToCache();
        UserDefault::getInstance()->setBoolForKey("KnightInCache", true);
    }
    if (!UserDefault::getInstance()->getBoolForKey("FighterInCache", false)) {
        loadNPCFighterToCache();
        UserDefault::getInstance()->setBoolForKey("FighterInCache", true);
    }
    if (!UserDefault::getInstance()->getBoolForKey("EffectInCache", false)) {
        loadEffectToCache();
        UserDefault::getInstance()->setBoolForKey("EffectInCache", true);
    }
    if (mapIndex == 0) {
        if (!UserDefault::getInstance()->getBoolForKey("WarriorInCache", false)) {
            loadWarriorToCache();
            UserDefault::getInstance()->setBoolForKey("WarriorInCache", true);
        }
        if (!UserDefault::getInstance()->getBoolForKey("KnightInCache", false)) {
            loadKnightToCache();
            UserDefault::getInstance()->setBoolForKey("KnightInCache", true);
        }
        if (!UserDefault::getInstance()->getBoolForKey("FighterInCache", false)) {
            loadNPCFighterToCache();
            UserDefault::getInstance()->setBoolForKey("FighterInCache", true);
        }
        if (!UserDefault::getInstance()->getBoolForKey("EffectInCache", false)) {
            loadEffectToCache();
            UserDefault::getInstance()->setBoolForKey("EffectInCache", true);
        }
    }
    else if (mapIndex == 1) {
        if (!UserDefault::getInstance()->getBoolForKey("SkeletonInCache", false)) {
            loadSkeletonToCache();
            UserDefault::getInstance()->setBoolForKey("SkeletonInCache", true);
        }
    }
    else if (mapIndex == 2) {
        if (!UserDefault::getInstance()->getBoolForKey("BabySpiderInCache", false)) {
            loadBabySpiderToCache();
            UserDefault::getInstance()->setBoolForKey("BabySpiderInCache", true);
        }
    }
    else if (mapIndex == 3) {
        if (!UserDefault::getInstance()->getBoolForKey("HellBeastInCache", false)) {
            loadHellBeastToCache();
            UserDefault::getInstance()->setBoolForKey("HellBeastInCache", true);
        }
    }
}

Rect GameMap::getEndPoint() {
    if (_tileMap) {
        auto _objectGroup = _tileMap->getObjectGroup("Player");
        ValueMap endPoint = _objectGroup->getObject("End Point");
        float x = 500, y = 500, width = 50, height = 50;
        if (!endPoint.empty()) {
            width = endPoint.at("width").asFloat() * (visibleSize.width / visibleSize.height * 2);
            height = endPoint.at("height").asFloat() * (visibleSize.width / visibleSize.height * 2);
            x = endPoint.at("x").asFloat() - width / 2;
            y = endPoint.at("y").asFloat() - height / 2;
        }
        return Rect(x, y, width, height);
    }
    return Rect(0, 0, 0, 0);
}

Rect GameMap::getPrevPoint() {
    if (_tileMap) {
        auto _objectGroup = _tileMap->getObjectGroup("Player");
        ValueMap startPoint = _objectGroup->getObject("Prev Point");
        float x = 500, y = 500, width = 50, height = 50;
        if (!startPoint.empty()) {
            width = startPoint.at("width").asFloat() * (visibleSize.width / visibleSize.height * 2);
            height = startPoint.at("height").asFloat() * (visibleSize.width / visibleSize.height * 2);
            x = startPoint.at("x").asFloat() - width / 2;
            y = startPoint.at("y").asFloat() - height / 2;
        }
        return Rect(x, y, width, height);
    }
    return Rect(0, 0, 0, 0);
}

GameMap::~GameMap() {
	if (_tileMap) {
		delete _tileMap;
		_tileMap = nullptr;
	}

	instance = nullptr;
}

void GameMap::loadEffectToCache() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/BossSkill/HellBeastSkill/summon/summon.plist", "skill/SkillSprite/BossSkill/HellBeastSkill/summon/summon.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/effect/teleport_effect.plist", "sprites/effect/teleport_effect.png");
    //Skills
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Fire/fire_ball/fire_ball.plist", "skill/SkillSprite/PlayerCharacterSkill/Fire/fire_ball/fire_ball.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Fire/fire_cast_a/fire_cast_a.plist", "skill/SkillSprite/PlayerCharacterSkill/Fire/fire_cast_a/fire_cast_a.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Fire/fire_cast_b/fire_cast_b.plist", "skill/SkillSprite/PlayerCharacterSkill/Fire/fire_cast_b/fire_cast_b.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Fire/fire_combust/fire_combust.plist", "skill/SkillSprite/PlayerCharacterSkill/Fire/fire_combust/fire_combust.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Fire/fire_conflagration/fire_conflagration.plist", "skill/SkillSprite/PlayerCharacterSkill/Fire/fire_conflagration/fire_conflagration.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Fire/fire_flare/fire_flare.plist", "skill/SkillSprite/PlayerCharacterSkill/Fire/fire_flare/fire_flare.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Fire/fire_scorch/fire_scorch.plist", "skill/SkillSprite/PlayerCharacterSkill/Fire/fire_scorch/fire_scorch.png");

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_bolt/thunder_bolt.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_bolt/thunder_bolt.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_cast_a/thunder_cast_a.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_cast_a/thunder_cast_a.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_cast_b/thunder_cast_b.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_cast_b/thunder_cast_b.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_crackle/thunder_crackle.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_crackle/thunder_crackle.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_current/thunder_current.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_current/thunder_current.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_dynamo/thunder_dynamo.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_dynamo/thunder_dynamo.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_shock/thunder_shock.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_shock/thunder_shock.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_voltage/thunder_voltage_flying.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_voltage/thunder_voltage_flying.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_voltage/thunder_voltage_explode.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_voltage/thunder_voltage_explode.png");

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Fire/fire_sear/fire_sear.plist", "skill/SkillSprite/PlayerCharacterSkill/Fire/fire_sear/fire_sear.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_spark/thunder_spark.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_spark/thunder_spark.png");

}

void GameMap::loadKnightToCache()
{
    // Armed Idle
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_idle/E/knight_armed_idle_E.plist", "playable character/knight/knight_armed_idle/E/knight_armed_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_idle/N/knight_armed_idle_N.plist", "playable character/knight/knight_armed_idle/N/knight_armed_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_idle/NE/knight_armed_idle_NE.plist", "playable character/knight/knight_armed_idle/NE/knight_armed_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_idle/NW/knight_armed_idle_NW.plist", "playable character/knight/knight_armed_idle/NW/knight_armed_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_idle/S/knight_armed_idle_S.plist", "playable character/knight/knight_armed_idle/S/knight_armed_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_idle/SE/knight_armed_idle_SE.plist", "playable character/knight/knight_armed_idle/SE/knight_armed_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_idle/SW/knight_armed_idle_SW.plist", "playable character/knight/knight_armed_idle/SW/knight_armed_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_idle/W/knight_armed_idle_W.plist", "playable character/knight/knight_armed_idle/W/knight_armed_idle_W.png");

    // Armed Attack
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_attack/E/knight_armed_attack_E.plist", "playable character/knight/knight_armed_attack/E/knight_armed_attack_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_attack/N/knight_armed_attack_N.plist", "playable character/knight/knight_armed_attack/N/knight_armed_attack_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_attack/NE/knight_armed_attack_NE.plist", "playable character/knight/knight_armed_attack/NE/knight_armed_attack_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_attack/NW/knight_armed_attack_NW.plist", "playable character/knight/knight_armed_attack/NW/knight_armed_attack_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_attack/S/knight_armed_attack_S.plist", "playable character/knight/knight_armed_attack/S/knight_armed_attack_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_attack/SE/knight_armed_attack_SE.plist", "playable character/knight/knight_armed_attack/SE/knight_armed_attack_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_attack/SW/knight_armed_attack_SW.plist", "playable character/knight/knight_armed_attack/SW/knight_armed_attack_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_attack/W/knight_armed_attack_W.plist", "playable character/knight/knight_armed_attack/W/knight_armed_attack_W.png");

    // Arm Walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_walk/E/knight_armed_walk_E.plist", "playable character/knight/knight_armed_walk/E/knight_armed_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_walk/N/knight_armed_walk_N.plist", "playable character/knight/knight_armed_walk/N/knight_armed_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_walk/NE/knight_armed_walk_NE.plist", "playable character/knight/knight_armed_walk/NE/knight_armed_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_walk/NW/knight_armed_walk_NW.plist", "playable character/knight/knight_armed_walk/NW/knight_armed_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_walk/S/knight_armed_walk_S.plist", "playable character/knight/knight_armed_walk/S/knight_armed_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_walk/SE/knight_armed_walk_SE.plist", "playable character/knight/knight_armed_walk/SE/knight_armed_walk_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_walk/SW/knight_armed_walk_SW.plist", "playable character/knight/knight_armed_walk/SW/knight_armed_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_armed_walk/W/knight_armed_walk_W.plist", "playable character/knight/knight_armed_walk/W/knight_armed_walk_W.png");
    // Default Walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_default_walk/E/knight_default_walk_E.plist", "playable character/knight/knight_default_walk/E/knight_default_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_default_walk/N/knight_default_walk_N.plist", "playable character/knight/knight_default_walk/N/knight_default_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_default_walk/NE/knight_default_walk_NE.plist", "playable character/knight/knight_default_walk/NE/knight_default_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_default_walk/NW/knight_default_walk_NW.plist", "playable character/knight/knight_default_walk/NW/knight_default_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_default_walk/S/knight_default_walk_S.plist", "playable character/knight/knight_default_walk/S/knight_default_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_default_walk/SE/knight_default_walk_SE.plist", "playable character/knight/knight_default_walk/SE/knight_default_walk_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_default_walk/SW/knight_default_walk_SW.plist", "playable character/knight/knight_default_walk/SW/knight_default_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_default_walk/W/knight_default_walk_W.plist", "playable character/knight/knight_default_walk/W/knight_default_walk_W.png");

    // Special Death
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_death/E/knight_special_death_E.plist", "playable character/knight/knight_special_death/E/knight_special_death_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_death/N/knight_special_death_N.plist", "playable character/knight/knight_special_death/N/knight_special_death_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_death/NE/knight_special_death_NE.plist", "playable character/knight/knight_special_death/NE/knight_special_death_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_death/NW/knight_special_death_NW.plist", "playable character/knight/knight_special_death/NW/knight_special_death_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_death/S/knight_special_death_S.plist", "playable character/knight/knight_special_death/S/knight_special_death_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_death/SE/knight_special_death_SE.plist", "playable character/knight/knight_special_death/SE/knight_special_death_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_death/SW/knight_special_death_SW.plist", "playable character/knight/knight_special_death/SW/knight_special_death_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_death/W/knight_special_death_W.plist", "playable character/knight/knight_special_death/W/knight_special_death_W.png");


    // Special Select
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_select/E/knight_special_select_E.plist", "playable character/knight/knight_special_select/E/knight_special_select_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_select/N/knight_special_select_N.plist", "playable character/knight/knight_special_select/N/knight_special_select_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_select/NE/knight_special_select_NE.plist", "playable character/knight/knight_special_select/NE/knight_special_select_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_select/NW/knight_special_select_NW.plist", "playable character/knight/knight_special_select/NW/knight_special_select_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_select/S/knight_special_select_S.plist", "playable character/knight/knight_special_select/S/knight_special_select_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_select/SE/knight_special_select_SE.plist", "playable character/knight/knight_special_select/SE/knight_special_select_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_select/SW/knight_special_select_SW.plist", "playable character/knight/knight_special_select/SW/knight_special_select_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/knight/knight_special_select/W/knight_special_select_W.plist", "playable character/knight/knight_special_select/W/knight_special_select_W.png");
}
// Skeleton
void GameMap::loadSkeletonToCache()
{
    // Default Walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_walk/E/skeleton_default_walk_E.plist", "enemy/skeleton/skeleton_default_walk/E/skeleton_default_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_walk/N/skeleton_default_walk_N.plist", "enemy/skeleton/skeleton_default_walk/N/skeleton_default_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_walk/NE/skeleton_default_walk_NE.plist", "enemy/skeleton/skeleton_default_walk/NE/skeleton_default_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_walk/NW/skeleton_default_walk_NW.plist", "enemy/skeleton/skeleton_default_walk/NW/skeleton_default_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_walk/S/skeleton_default_walk_S.plist", "enemy/skeleton/skeleton_default_walk/S/skeleton_default_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_walk/SE/skeleton_default_walk_SE.plist", "enemy/skeleton/skeleton_default_walk/SE/skeleton_default_walk_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_walk/SW/skeleton_default_walk_SW.plist", "enemy/skeleton/skeleton_default_walk/SW/skeleton_default_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_walk/W/skeleton_default_walk_W.plist", "enemy/skeleton/skeleton_default_walk/W/skeleton_default_walk_W.png");

    // Default idle
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_idle/E/skeleton_default_idle_E.plist", "enemy/skeleton/skeleton_default_idle/E/skeleton_default_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_idle/N/skeleton_default_idle_N.plist", "enemy/skeleton/skeleton_default_idle/N/skeleton_default_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_idle/NE/skeleton_default_idle_NE.plist", "enemy/skeleton/skeleton_default_idle/NE/skeleton_default_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_idle/NW/skeleton_default_idle_NW.plist", "enemy/skeleton/skeleton_default_idle/NW/skeleton_default_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_idle/S/skeleton_default_idle_S.plist", "enemy/skeleton/skeleton_default_idle/S/skeleton_default_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_idle/SE/skeleton_default_idle_SE.plist", "enemy/skeleton/skeleton_default_idle/SE/skeleton_default_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_idle/SW/skeleton_default_idle_SW.plist", "enemy/skeleton/skeleton_default_idle/SW/skeleton_default_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_idle/W/skeleton_default_idle_W.plist", "enemy/skeleton/skeleton_default_idle/W/skeleton_default_idle_W.png");
    // Default attack
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_attack/E/skeleton_default_attack_E.plist", "enemy/skeleton/skeleton_default_attack/E/skeleton_default_attack_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_attack/N/skeleton_default_attack_N.plist", "enemy/skeleton/skeleton_default_attack/N/skeleton_default_attack_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_attack/NE/skeleton_default_attack_NE.plist", "enemy/skeleton/skeleton_default_attack/NE/skeleton_default_attack_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_attack/NW/skeleton_default_attack_NW.plist", "enemy/skeleton/skeleton_default_attack/NW/skeleton_default_attack_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_attack/S/skeleton_default_attack_S.plist", "enemy/skeleton/skeleton_default_attack/S/skeleton_default_attack_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_attack/SE/skeleton_default_attack_SE.plist", "enemy/skeleton/skeleton_default_attack/SE/skeleton_default_attack_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_attack/SW/skeleton_default_attack_SW.plist", "enemy/skeleton/skeleton_default_attack/SW/skeleton_default_attack_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_default_attack/W/skeleton_default_attack_W.plist", "enemy/skeleton/skeleton_default_attack/W/skeleton_default_attack_W.png");

    // Default Walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_special_death/E/skeleton_special_death_E.plist", "enemy/skeleton/skeleton_special_death/E/skeleton_special_death_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_special_death/N/skeleton_special_death_N.plist", "enemy/skeleton/skeleton_special_death/N/skeleton_special_death_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_special_death/NE/skeleton_special_death_NE.plist", "enemy/skeleton/skeleton_special_death/NE/skeleton_special_death_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_special_death/NW/skeleton_special_death_NW.plist", "enemy/skeleton/skeleton_special_death/NW/skeleton_special_death_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_special_death/S/skeleton_special_death_S.plist", "enemy/skeleton/skeleton_special_death/S/skeleton_special_death_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_special_death/SE/skeleton_special_death_SE.plist", "enemy/skeleton/skeleton_special_death/SE/skeleton_special_death_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_special_death/SW/skeleton_special_death_SW.plist", "enemy/skeleton/skeleton_special_death/SW/skeleton_special_death_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/skeleton/skeleton_special_death/W/skeleton_special_death_W.plist", "enemy/skeleton/skeleton_special_death/W/skeleton_special_death_W.png");
}
// Slime
void GameMap::loadSlimeToCache()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_walk/E/slime_default_walk_E.plist", "enemy/slime/slime_default_walk/E/slime_default_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_walk/N/slime_default_walk_N.plist", "enemy/slime/slime_default_walk/N/slime_default_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_walk/NE/slime_default_walk_NE.plist", "enemy/slime/slime_default_walk/NE/slime_default_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_walk/NW/slime_default_walk_NW.plist", "enemy/slime/slime_default_walk/NW/slime_default_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_walk/S/slime_default_walk_S.plist", "enemy/slime/slime_default_walk/S/slime_default_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_walk/SE/slime_default_walk_SE.plist", "enemy/slime/slime_default_walk/SE/slime_default_walk_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_walk/SW/slime_default_walk_SW.plist", "enemy/slime/slime_default_walk/SW/slime_default_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_walk/W/slime_default_walk_W.plist", "enemy/slime/slime_default_walk/W/slime_default_walk_W.png");

    // Default idle
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_idle/E/slime_default_idle_E.plist", "enemy/slime/slime_default_idle/E/slime_default_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_idle/N/slime_default_idle_N.plist", "enemy/slime/slime_default_idle/N/slime_default_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_idle/NE/slime_default_idle_NE.plist", "enemy/slime/slime_default_idle/NE/slime_default_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_idle/NW/slime_default_idle_NW.plist", "enemy/slime/slime_default_idle/NW/slime_default_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_idle/S/slime_default_idle_S.plist", "enemy/slime/slime_default_idle/S/slime_default_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_idle/SE/slime_default_idle_SE.plist", "enemy/slime/slime_default_idle/SE/slime_default_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_idle/SW/slime_default_idle_SW.plist", "enemy/slime/slime_default_idle/SW/slime_default_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_default_idle/W/slime_default_idle_W.plist", "enemy/slime/slime_default_idle/W/slime_default_idle_W.png");

    // Default Walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_special_death/E/slime_special_death_E.plist", "enemy/slime/slime_special_death/E/slime_special_death_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_special_death/N/slime_special_death_N.plist", "enemy/slime/slime_special_death/N/slime_special_death_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_special_death/NE/slime_special_death_NE.plist", "enemy/slime/slime_special_death/NE/slime_special_death_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_special_death/NW/slime_special_death_NW.plist", "enemy/slime/slime_special_death/NW/slime_special_death_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_special_death/S/slime_special_death_S.plist", "enemy/slime/slime_special_death/S/slime_special_death_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_special_death/SE/slime_special_death_SE.plist", "enemy/slime/slime_special_death/SE/slime_special_death_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_special_death/SW/slime_special_death_SW.plist", "enemy/slime/slime_special_death/SW/slime_special_death_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/slime/slime_special_death/W/slime_special_death_W.plist", "enemy/slime/slime_special_death/W/slime_special_death_W.png");
}

void GameMap::loadBabySpiderToCache()
{
    // Default Walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_walk/baby_spider_walk_E/baby_spider_walk_E.plist", "enemy/baby_spider/baby_spider_walk/baby_spider_walk_E/baby_spider_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_walk/baby_spider_walk_N/baby_spider_walk_N.plist", "enemy/baby_spider/baby_spider_walk/baby_spider_walk_N/baby_spider_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_walk/baby_spider_walk_NE/baby_spider_walk_NE.plist", "enemy/baby_spider/baby_spider_walk/baby_spider_walk_NE/baby_spider_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_walk/baby_spider_walk_NW/baby_spider_walk_NW.plist", "enemy/baby_spider/baby_spider_walk/baby_spider_walk_NW/baby_spider_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_walk/baby_spider_walk_S/baby_spider_walk_S.plist", "enemy/baby_spider/baby_spider_walk/baby_spider_walk_S/baby_spider_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_walk/baby_spider_walk_SE/baby_spider_walk_SE.plist", "enemy/baby_spider/baby_spider_walk/baby_spider_walk_SE/baby_spider_walk_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_walk/baby_spider_walk_SW/baby_spider_walk_SW.plist", "enemy/baby_spider/baby_spider_walk/baby_spider_walk_SW/baby_spider_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_walk/baby_spider_walk_W/baby_spider_walk_W.plist", "enemy/baby_spider/baby_spider_walk/baby_spider_walk_W/baby_spider_walk_W.png");

    // Default idle
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_idle/baby_spider_idle_E/baby_spider_idle_E.plist", "enemy/baby_spider/baby_spider_idle/baby_spider_idle_E/baby_spider_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_idle/baby_spider_idle_N/baby_spider_idle_N.plist", "enemy/baby_spider/baby_spider_idle/baby_spider_idle_N/baby_spider_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_idle/baby_spider_idle_NE/baby_spider_idle_NE.plist", "enemy/baby_spider/baby_spider_idle/baby_spider_idle_NE/baby_spider_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_idle/baby_spider_idle_NW/baby_spider_idle_NW.plist", "enemy/baby_spider/baby_spider_idle/baby_spider_idle_NW/baby_spider_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_idle/baby_spider_idle_S/baby_spider_idle_S.plist", "enemy/baby_spider/baby_spider_idle/baby_spider_idle_S/baby_spider_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_idle/baby_spider_idle_SE/baby_spider_idle_SE.plist", "enemy/baby_spider/baby_spider_idle/baby_spider_idle_SE/baby_spider_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_idle/baby_spider_idle_SW/baby_spider_idle_SW.plist", "enemy/baby_spider/baby_spider_idle/baby_spider_idle_SW/baby_spider_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_idle/baby_spider_idle_W/baby_spider_idle_W.plist", "enemy/baby_spider/baby_spider_idle/baby_spider_idle_W/baby_spider_idle_W.png");
    // Default attack
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_attack/baby_spider_attack_E/baby_spider_attack_E.plist", "enemy/baby_spider/baby_spider_attack/baby_spider_attack_E/baby_spider_attack_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_attack/baby_spider_attack_N/baby_spider_attack_N.plist", "enemy/baby_spider/baby_spider_attack/baby_spider_attack_N/baby_spider_attack_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_attack/baby_spider_attack_NE/baby_spider_attack_NE.plist", "enemy/baby_spider/baby_spider_attack/baby_spider_attack_NE/baby_spider_attack_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_attack/baby_spider_attack_NW/baby_spider_attack_NW.plist", "enemy/baby_spider/baby_spider_attack/baby_spider_attack_NW/baby_spider_attack_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_attack/baby_spider_attack_S/baby_spider_attack_S.plist", "enemy/baby_spider/baby_spider_attack/baby_spider_attack_S/baby_spider_attack_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_attack/baby_spider_attack_SE/baby_spider_attack_SE.plist", "enemy/baby_spider/baby_spider_attack/baby_spider_attack_SE/baby_spider_attack_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_attack/baby_spider_attack_SW/baby_spider_attack_SW.plist", "enemy/baby_spider/baby_spider_attack/baby_spider_attack_SW/baby_spider_attack_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_attack/baby_spider_attack_W/baby_spider_attack_W.plist", "enemy/baby_spider/baby_spider_attack/baby_spider_attack_W/baby_spider_attack_W.png");

    // Default death
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_death/baby_spider_death_E/baby_spider_death_E.plist", "enemy/baby_spider/baby_spider_death/baby_spider_death_E/baby_spider_death_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_death/baby_spider_death_N/baby_spider_death_N.plist", "enemy/baby_spider/baby_spider_death/baby_spider_death_N/baby_spider_death_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_death/baby_spider_death_NE/baby_spider_death_NE.plist", "enemy/baby_spider/baby_spider_death/baby_spider_death_NE/baby_spider_death_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_death/baby_spider_death_NW/baby_spider_death_NW.plist", "enemy/baby_spider/baby_spider_death/baby_spider_death_NW/baby_spider_death_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_death/baby_spider_death_S/baby_spider_death_S.plist", "enemy/baby_spider/baby_spider_death/baby_spider_death_S/baby_spider_death_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_death/baby_spider_death_SE/baby_spider_death_SE.plist", "enemy/baby_spider/baby_spider_death/baby_spider_death_SE/baby_spider_death_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_death/baby_spider_death_SW/baby_spider_death_SW.plist", "enemy/baby_spider/baby_spider_death/baby_spider_death_SW/baby_spider_death_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/baby_spider/baby_spider_death/baby_spider_death_W/baby_spider_death_W.plist", "enemy/baby_spider/baby_spider_death/baby_spider_death_W/baby_spider_death_W.png");
}

void GameMap::loadAngelosToCache()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_idle/angelos_idle_E/angelos_idle_E.plist", "enemy/angelos/angelos_idle/angelos_idle_E/angelos_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_idle/angelos_idle_N/angelos_idle_N.plist", "enemy/angelos/angelos_idle/angelos_idle_N/angelos_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_idle/angelos_idle_NE/angelos_idle_NE.plist", "enemy/angelos/angelos_idle/angelos_idle_NE/angelos_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_idle/angelos_idle_NW/angelos_idle_NW.plist", "enemy/angelos/angelos_idle/angelos_idle_NW/angelos_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_idle/angelos_idle_S/angelos_idle_S.plist", "enemy/angelos/angelos_idle/angelos_idle_S/angelos_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_idle/angelos_idle_SE/angelos_idle_SE.plist", "enemy/angelos/angelos_idle/angelos_idle_SE/angelos_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_idle/angelos_idle_SW/angelos_idle_SW.plist", "enemy/angelos/angelos_idle/angelos_idle_SW/angelos_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_idle/angelos_idle_W/angelos_idle_W.plist", "enemy/angelos/angelos_idle/angelos_idle_W/angelos_idle_W.png");

    // walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_walk/angelos_walk_E/angelos_walk_E.plist", "enemy/angelos/angelos_walk/angelos_walk_E/angelos_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_walk/angelos_walk_N/angelos_walk_N.plist", "enemy/angelos/angelos_walk/angelos_walk_N/angelos_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_walk/angelos_walk_NE/angelos_walk_NE.plist", "enemy/angelos/angelos_walk/angelos_walk_NE/angelos_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_walk/angelos_walk_NW/angelos_walk_NW.plist", "enemy/angelos/angelos_walk/angelos_walk_NW/angelos_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_walk/angelos_walk_S/angelos_walk_S.plist", "enemy/angelos/angelos_walk/angelos_walk_S/angelos_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_walk/angelos_walk_SE/angelos_walk_SE.plist", "enemy/angelos/angelos_walk/angelos_walk_SE/angelos_walk_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_walk/angelos_walk_SW/angelos_walk_SW.plist", "enemy/angelos/angelos_walk/angelos_walk_SW/angelos_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_walk/angelos_walk_W/angelos_walk_W.plist", "enemy/angelos/angelos_walk/angelos_walk_W/angelos_walk_W.png");
    // attack
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_attack/angelos_attack_E/angelos_attack_E.plist", "enemy/angelos/angelos_attack/angelos_attack_E/angelos_attack_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_attack/angelos_attack_N/angelos_attack_N.plist", "enemy/angelos/angelos_attack/angelos_attack_N/angelos_attack_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_attack/angelos_attack_NE/angelos_attack_NE.plist", "enemy/angelos/angelos_attack/angelos_attack_NE/angelos_attack_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_attack/angelos_attack_NW/angelos_attack_NW.plist", "enemy/angelos/angelos_attack/angelos_attack_NW/angelos_attack_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_attack/angelos_attack_S/angelos_attack_S.plist", "enemy/angelos/angelos_attack/angelos_attack_S/angelos_attack_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_attack/angelos_attack_SE/angelos_attack_SE.plist", "enemy/angelos/angelos_attack/angelos_attack_SE/angelos_attack_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_attack/angelos_attack_SW/angelos_attack_SW.plist", "enemy/angelos/angelos_attack/angelos_attack_SW/angelos_attack_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_attack/angelos_attack_W/angelos_attack_W.plist", "enemy/angelos/angelos_attack/angelos_attack_W/angelos_attack_W.png");
    // death
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_death/angelos_death_E/angelos_death_E.plist", "enemy/angelos/angelos_death/angelos_death_E/angelos_death_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_death/angelos_death_N/angelos_death_N.plist", "enemy/angelos/angelos_death/angelos_death_N/angelos_death_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_death/angelos_death_NE/angelos_death_NE.plist", "enemy/angelos/angelos_death/angelos_death_NE/angelos_death_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_death/angelos_death_NW/angelos_death_NW.plist", "enemy/angelos/angelos_death/angelos_death_NW/angelos_death_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_death/angelos_death_S/angelos_death_S.plist", "enemy/angelos/angelos_death/angelos_death_S/angelos_death_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_death/angelos_death_SE/angelos_death_SE.plist", "enemy/angelos/angelos_death/angelos_death_SE/angelos_death_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_death/angelos_death_SW/angelos_death_SW.plist", "enemy/angelos/angelos_death/angelos_death_SW/angelos_death_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/angelos/angelos_death/angelos_death_W/angelos_death_W.plist", "enemy/angelos/angelos_death/angelos_death_W/angelos_death_W.png");
}

void GameMap::loadShadowSentinels()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_E/shadow_sentinels_idle_E.plist", "enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_E/shadow_sentinels_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_N/shadow_sentinels_idle_N.plist", "enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_N/shadow_sentinels_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_NE/shadow_sentinels_idle_NE.plist", "enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_NE/shadow_sentinels_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_NW/shadow_sentinels_idle_NW.plist", "enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_NW/shadow_sentinels_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_S/shadow_sentinels_idle_S.plist", "enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_S/shadow_sentinels_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_SE/shadow_sentinels_idle_SE.plist", "enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_SE/shadow_sentinels_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_SW/shadow_sentinels_idle_SW.plist", "enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_SW/shadow_sentinels_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_W/shadow_sentinels_idle_W.plist", "enemy/shadow_sentinels/shadow_sentinels_idle/shadow_sentinels_idle_W/shadow_sentinels_idle_W.png");

    // walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_E/shadow_sentinels_walk_E.plist", "enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_E/shadow_sentinels_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_N/shadow_sentinels_walk_N.plist", "enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_N/shadow_sentinels_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_NE/shadow_sentinels_walk_NE.plist", "enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_NE/shadow_sentinels_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_NW/shadow_sentinels_walk_NW.plist", "enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_NW/shadow_sentinels_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_S/shadow_sentinels_walk_S.plist", "enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_S/shadow_sentinels_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_SE/shadow_sentinels_walk_SE.plist", "enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_SE/shadow_sentinels_walk_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_SW/shadow_sentinels_walk_SW.plist", "enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_SW/shadow_sentinels_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_W/shadow_sentinels_walk_W.plist", "enemy/shadow_sentinels/shadow_sentinels_walk/shadow_sentinels_walk_W/shadow_sentinels_walk_W.png");
    // attack
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_E/shadow_sentinels_attack_E.plist", "enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_E/shadow_sentinels_attack_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_N/shadow_sentinels_attack_N.plist", "enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_N/shadow_sentinels_attack_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_NE/shadow_sentinels_attack_NE.plist", "enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_NE/shadow_sentinels_attack_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_NW/shadow_sentinels_attack_NW.plist", "enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_NW/shadow_sentinels_attack_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_S/shadow_sentinels_attack_S.plist", "enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_S/shadow_sentinels_attack_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_SE/shadow_sentinels_attack_SE.plist", "enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_SE/shadow_sentinels_attack_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_SW/shadow_sentinels_attack_SW.plist", "enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_SW/shadow_sentinels_attack_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_W/shadow_sentinels_attack_W.plist", "enemy/shadow_sentinels/shadow_sentinels_attack/shadow_sentinels_attack_W/shadow_sentinels_attack_W.png");
    // death
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/shadow_sentinels/shadow_sentinels_death/shadow_sentinels_death.plist", "enemy/shadow_sentinels/shadow_sentinels_death/shadow_sentinels_death.png");
}

// Boss
void GameMap::loadHellBeastToCache()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack1/hellbeast_attack1_E/hellbeast_attack1_E.plist", "boss/HellBeast/HellBeastAttack1/hellbeast_attack1_E/hellbeast_attack1_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack1/hellbeast_attack1_NE/hellbeast_attack1_NE.plist", "boss/HellBeast/HellBeastAttack1/hellbeast_attack1_NE/hellbeast_attack1_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack1/hellbeast_attack1_N/hellbeast_attack1_N.plist", "boss/HellBeast/HellBeastAttack1/hellbeast_attack1_N/hellbeast_attack1_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack1/hellbeast_attack1_NW/hellbeast_attack1_NW.plist", "boss/HellBeast/HellBeastAttack1/hellbeast_attack1_NW/hellbeast_attack1_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack1/hellbeast_attack1_W/hellbeast_attack1_W.plist", "boss/HellBeast/HellBeastAttack1/hellbeast_attack1_W/hellbeast_attack1_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack1/hellbeast_attack1_SW/hellbeast_attack1_SW.plist", "boss/HellBeast/HellBeastAttack1/hellbeast_attack1_SW/hellbeast_attack1_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack1/hellbeast_attack1_S/hellbeast_attack1_S.plist", "boss/HellBeast/HellBeastAttack1/hellbeast_attack1_S/hellbeast_attack1_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack1/hellbeast_attack1_SE/hellbeast_attack1_SE.plist", "boss/HellBeast/HellBeastAttack1/hellbeast_attack1_SE/hellbeast_attack1_SE.png");

    // Attack2
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack2/hellbeast_attack2_E/hellbeast_attack2_E.plist", "boss/HellBeast/HellBeastAttack2/hellbeast_attack2_E/hellbeast_attack2_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack2/hellbeast_attack2_NE/hellbeast_attack2_NE.plist", "boss/HellBeast/HellBeastAttack2/hellbeast_attack2_NE/hellbeast_attack2_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack2/hellbeast_attack2_N/hellbeast_attack2_N.plist", "boss/HellBeast/HellBeastAttack2/hellbeast_attack2_N/hellbeast_attack2_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack2/hellbeast_attack2_NW/hellbeast_attack2_NW.plist", "boss/HellBeast/HellBeastAttack2/hellbeast_attack2_NW/hellbeast_attack2_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack2/hellbeast_attack2_W/hellbeast_attack2_W.plist", "boss/HellBeast/HellBeastAttack2/hellbeast_attack2_W/hellbeast_attack2_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack2/hellbeast_attack2_SW/hellbeast_attack2_SW.plist", "boss/HellBeast/HellBeastAttack2/hellbeast_attack2_SW/hellbeast_attack2_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack2/hellbeast_attack2_S/hellbeast_attack2_S.plist", "boss/HellBeast/HellBeastAttack2/hellbeast_attack2_S/hellbeast_attack2_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack2/hellbeast_attack2_SE/hellbeast_attack2_SE.plist", "boss/HellBeast/HellBeastAttack2/hellbeast_attack2_SE/hellbeast_attack2_SE.png");

    // Attack3
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastHit/hellbeast_hit_E/hellbeast_hit_E.plist", "boss/HellBeast/HellBeastHit/hellbeast_hit_E/hellbeast_hit_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastHit/hellbeast_hit_NE/hellbeast_hit_NE.plist", "boss/HellBeast/HellBeastHit/hellbeast_hit_NE/hellbeast_hit_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastHit/hellbeast_hit_N/hellbeast_hit_N.plist", "boss/HellBeast/HellBeastHit/hellbeast_hit_N/hellbeast_hit_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastHit/hellbeast_hit_NW/hellbeast_hit_NW.plist", "boss/HellBeast/HellBeastHit/hellbeast_hit_NW/hellbeast_hit_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastHit/hellbeast_hit_W/hellbeast_hit_W.plist", "boss/HellBeast/HellBeastHit/hellbeast_hit_W/hellbeast_hit_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastHit/hellbeast_hit_SW/hellbeast_hit_SW.plist", "boss/HellBeast/HellBeastHit/hellbeast_hit_SW/hellbeast_hit_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastHit/hellbeast_hit_S/hellbeast_hit_S.plist", "boss/HellBeast/HellBeastHit/hellbeast_hit_S/hellbeast_hit_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastHit/hellbeast_hit_SE/hellbeast_hit_SE.plist", "boss/HellBeast/HellBeastHit/hellbeast_hit_SE/hellbeast_hit_SE.png");

    // Death
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastDeath/hellbeast_death_E/hellbeast_death_E.plist", "boss/HellBeast/HellBeastDeath/hellbeast_death_E/hellbeast_death_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastDeath/hellbeast_death_NE/hellbeast_death_NE.plist", "boss/HellBeast/HellBeastDeath/hellbeast_death_NE/hellbeast_death_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastDeath/hellbeast_death_N/hellbeast_death_N.plist", "boss/HellBeast/HellBeastDeath/hellbeast_death_N/hellbeast_death_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastDeath/hellbeast_death_NW/hellbeast_death_NW.plist", "boss/HellBeast/HellBeastDeath/hellbeast_death_NW/hellbeast_death_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastDeath/hellbeast_death_W/hellbeast_death_W.plist", "boss/HellBeast/HellBeastDeath/hellbeast_death_W/hellbeast_death_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastDeath/hellbeast_death_SW/hellbeast_death_SW.plist", "boss/HellBeast/HellBeastDeath/hellbeast_death_SW/hellbeast_death_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastDeath/hellbeast_death_S/hellbeast_death_S.plist", "boss/HellBeast/HellBeastDeath/hellbeast_death_S/hellbeast_death_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastDeath/hellbeast_death_SE/hellbeast_death_SE.plist", "boss/HellBeast/HellBeastDeath/hellbeast_death_SE/hellbeast_death_SE.png");

    // Hit
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack3/hellbeast_attack3_E/hellbeast_attack3_E.plist", "boss/HellBeast/HellBeastAttack3/hellbeast_attack3_E/hellbeast_attack3_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack3/hellbeast_attack3_NE/hellbeast_attack3_NE.plist", "boss/HellBeast/HellBeastAttack3/hellbeast_attack3_NE/hellbeast_attack3_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack3/hellbeast_attack3_N/hellbeast_attack3_N.plist", "boss/HellBeast/HellBeastAttack3/hellbeast_attack3_N/hellbeast_attack3_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack3/hellbeast_attack3_NW/hellbeast_attack3_NW.plist", "boss/HellBeast/HellBeastAttack3/hellbeast_attack3_NW/hellbeast_attack3_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack3/hellbeast_attack3_W/hellbeast_attack3_W.plist", "boss/HellBeast/HellBeastAttack3/hellbeast_attack3_W/hellbeast_attack3_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack3/hellbeast_attack3_SW/hellbeast_attack3_SW.plist", "boss/HellBeast/HellBeastAttack3/hellbeast_attack3_SW/hellbeast_attack3_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack3/hellbeast_attack3_S/hellbeast_attack3_S.plist", "boss/HellBeast/HellBeastAttack3/hellbeast_attack3_S/hellbeast_attack3_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastAttack3/hellbeast_attack3_SE/hellbeast_attack3_SE.plist", "boss/HellBeast/HellBeastAttack3/hellbeast_attack3_SE/hellbeast_attack3_SE.png");
    // Idle
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastIdle/hellbeast_idle_E/hellbeast_idle_E.plist", "boss/HellBeast/HellBeastIdle/hellbeast_idle_E/hellbeast_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastIdle/hellbeast_idle_NE/hellbeast_idle_NE.plist", "boss/HellBeast/HellBeastIdle/hellbeast_idle_NE/hellbeast_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastIdle/hellbeast_idle_N/hellbeast_idle_N.plist", "boss/HellBeast/HellBeastIdle/hellbeast_idle_N/hellbeast_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastIdle/hellbeast_idle_NW/hellbeast_idle_NW.plist", "boss/HellBeast/HellBeastIdle/hellbeast_idle_NW/hellbeast_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastIdle/hellbeast_idle_W/hellbeast_idle_W.plist", "boss/HellBeast/HellBeastIdle/hellbeast_idle_W/hellbeast_idle_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastIdle/hellbeast_idle_SW/hellbeast_idle_SW.plist", "boss/HellBeast/HellBeastIdle/hellbeast_idle_SW/hellbeast_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastIdle/hellbeast_idle_S/hellbeast_idle_S.plist", "boss/HellBeast/HellBeastIdle/hellbeast_idle_S/hellbeast_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastIdle/hellbeast_idle_SE/hellbeast_idle_SE.plist", "boss/HellBeast/HellBeastIdle/hellbeast_idle_SE/hellbeast_idle_SE.png");

    // Roar
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRoar/hellbeast_roar_E/hellbeast_roar_E.plist", "boss/HellBeast/HellBeastRoar/hellbeast_roar_E/hellbeast_roar_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRoar/hellbeast_roar_NE/hellbeast_roar_NE.plist", "boss/HellBeast/HellBeastRoar/hellbeast_roar_NE/hellbeast_roar_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRoar/hellbeast_roar_N/hellbeast_roar_N.plist", "boss/HellBeast/HellBeastRoar/hellbeast_roar_N/hellbeast_roar_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRoar/hellbeast_roar_NW/hellbeast_roar_NW.plist", "boss/HellBeast/HellBeastRoar/hellbeast_roar_NW/hellbeast_roar_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRoar/hellbeast_roar_W/hellbeast_roar_W.plist", "boss/HellBeast/HellBeastRoar/hellbeast_roar_W/hellbeast_roar_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRoar/hellbeast_roar_SW/hellbeast_roar_SW.plist", "boss/HellBeast/HellBeastRoar/hellbeast_roar_SW/hellbeast_roar_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRoar/hellbeast_roar_S/hellbeast_roar_S.plist", "boss/HellBeast/HellBeastRoar/hellbeast_roar_S/hellbeast_roar_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRoar/hellbeast_roar_SE/hellbeast_roar_SE.plist", "boss/HellBeast/HellBeastRoar/hellbeast_roar_SE/hellbeast_roar_SE.png");

    // Run
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRun/hellbeast_run_E/hellbeast_run_E.plist", "boss/HellBeast/HellBeastRun/hellbeast_run_E/hellbeast_run_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRun/hellbeast_run_NE/hellbeast_run_NE.plist", "boss/HellBeast/HellBeastRun/hellbeast_run_NE/hellbeast_run_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRun/hellbeast_run_N/hellbeast_run_N.plist", "boss/HellBeast/HellBeastRun/hellbeast_run_N/hellbeast_run_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRun/hellbeast_run_NW/hellbeast_run_NW.plist", "boss/HellBeast/HellBeastRun/hellbeast_run_NW/hellbeast_run_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRun/hellbeast_run_W/hellbeast_run_W.plist", "boss/HellBeast/HellBeastRun/hellbeast_run_W/hellbeast_run_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRun/hellbeast_run_SW/hellbeast_run_SW.plist", "boss/HellBeast/HellBeastRun/hellbeast_run_SW/hellbeast_run_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRun/hellbeast_run_S/hellbeast_run_S.plist", "boss/HellBeast/HellBeastRun/hellbeast_run_S/hellbeast_run_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastRun/hellbeast_run_SE/hellbeast_run_SE.plist", "boss/HellBeast/HellBeastRun/hellbeast_run_SE/hellbeast_run_SE.png");

    // Walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastWalk/hellbeast_walk_E/hellbeast_walk_E.plist", "boss/HellBeast/HellBeastWalk/hellbeast_walk_E/hellbeast_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastWalk/hellbeast_walk_NE/hellbeast_walk_NE.plist", "boss/HellBeast/HellBeastWalk/hellbeast_walk_NE/hellbeast_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastWalk/hellbeast_walk_N/hellbeast_walk_N.plist", "boss/HellBeast/HellBeastWalk/hellbeast_walk_N/hellbeast_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastWalk/hellbeast_walk_NW/hellbeast_walk_NW.plist", "boss/HellBeast/HellBeastWalk/hellbeast_walk_NW/hellbeast_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastWalk/hellbeast_walk_W/hellbeast_walk_W.plist", "boss/HellBeast/HellBeastWalk/hellbeast_walk_W/hellbeast_walk_W.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastWalk/hellbeast_walk_SW/hellbeast_walk_SW.plist", "boss/HellBeast/HellBeastWalk/hellbeast_walk_SW/hellbeast_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastWalk/hellbeast_walk_S/hellbeast_walk_S.plist", "boss/HellBeast/HellBeastWalk/hellbeast_walk_S/hellbeast_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/HellBeast/HellBeastWalk/hellbeast_walk_SE/hellbeast_walk_SE.plist", "boss/HellBeast/HellBeastWalk/hellbeast_walk_SE/hellbeast_walk_SE.png");
}

void GameMap::loadParadisoToCache()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_idle/paradiso_idle_E/paradiso_idle_E.plist", "boss/Paradiso/paradiso_idle/paradiso_idle_E/paradiso_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_idle/paradiso_idle_N/paradiso_idle_N.plist", "boss/Paradiso/paradiso_idle/paradiso_idle_N/paradiso_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_idle/paradiso_idle_NE/paradiso_idle_NE.plist", "boss/Paradiso/paradiso_idle/paradiso_idle_NE/paradiso_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_idle/paradiso_idle_NW/paradiso_idle_NW.plist", "boss/Paradiso/paradiso_idle/paradiso_idle_NW/paradiso_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_idle/paradiso_idle_S/paradiso_idle_S.plist", "boss/Paradiso/paradiso_idle/paradiso_idle_S/paradiso_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_idle/paradiso_idle_SE/paradiso_idle_SE.plist", "boss/Paradiso/paradiso_idle/paradiso_idle_SE/paradiso_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_idle/paradiso_idle_SW/paradiso_idle_SW.plist", "boss/Paradiso/paradiso_idle/paradiso_idle_SW/paradiso_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_idle/paradiso_idle_W/paradiso_idle_W.plist", "boss/Paradiso/paradiso_idle/paradiso_idle_W/paradiso_idle_W.png");

    // get hit
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_hit/paradiso_hit_E/paradiso_hit_E.plist", "boss/Paradiso/paradiso_hit/paradiso_hit_E/paradiso_hit_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_hit/paradiso_hit_N/paradiso_hit_N.plist", "boss/Paradiso/paradiso_hit/paradiso_hit_N/paradiso_hit_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_hit/paradiso_hit_NE/paradiso_hit_NE.plist", "boss/Paradiso/paradiso_hit/paradiso_hit_NE/paradiso_hit_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_hit/paradiso_hit_NW/paradiso_hit_NW.plist", "boss/Paradiso/paradiso_hit/paradiso_hit_NW/paradiso_hit_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_hit/paradiso_hit_S/paradiso_hit_S.plist", "boss/Paradiso/paradiso_hit/paradiso_hit_S/paradiso_hit_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_hit/paradiso_hit_SE/paradiso_hit_SE.plist", "boss/Paradiso/paradiso_hit/paradiso_hit_SE/paradiso_hit_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_hit/paradiso_hit_SW/paradiso_hit_SW.plist", "boss/Paradiso/paradiso_hit/paradiso_hit_SW/paradiso_hit_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_hit/paradiso_hit_W/paradiso_hit_W.plist", "boss/Paradiso/paradiso_hit/paradiso_hit_W/paradiso_hit_W.png");

    // run
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_run/paradiso_run_E/paradiso_run_E.plist", "boss/Paradiso/paradiso_run/paradiso_run_E/paradiso_run_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_run/paradiso_run_N/paradiso_run_N.plist", "boss/Paradiso/paradiso_run/paradiso_run_N/paradiso_run_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_run/paradiso_run_NE/paradiso_run_NE.plist", "boss/Paradiso/paradiso_run/paradiso_run_NE/paradiso_run_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_run/paradiso_run_NW/paradiso_run_NW.plist", "boss/Paradiso/paradiso_run/paradiso_run_NW/paradiso_run_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_run/paradiso_run_S/paradiso_run_S.plist", "boss/Paradiso/paradiso_run/paradiso_run_S/paradiso_run_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_run/paradiso_run_SE/paradiso_run_SE.plist", "boss/Paradiso/paradiso_run/paradiso_run_SE/paradiso_run_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_run/paradiso_run_SW/paradiso_run_SW.plist", "boss/Paradiso/paradiso_run/paradiso_run_SW/paradiso_run_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_run/paradiso_run_W/paradiso_run_W.plist", "boss/Paradiso/paradiso_run/paradiso_run_W/paradiso_run_W.png");
    // walk
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_walk/paradiso_walk_E/paradiso_walk_E.plist", "boss/Paradiso/paradiso_walk/paradiso_walk_E/paradiso_walk_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_walk/paradiso_walk_N/paradiso_walk_N.plist", "boss/Paradiso/paradiso_walk/paradiso_walk_N/paradiso_walk_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_walk/paradiso_walk_NE/paradiso_walk_NE.plist", "boss/Paradiso/paradiso_walk/paradiso_walk_NE/paradiso_walk_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_walk/paradiso_walk_NW/paradiso_walk_NW.plist", "boss/Paradiso/paradiso_walk/paradiso_walk_NW/paradiso_walk_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_walk/paradiso_walk_S/paradiso_walk_S.plist", "boss/Paradiso/paradiso_walk/paradiso_walk_S/paradiso_walk_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_walk/paradiso_walk_SE/paradiso_walk_SE.plist", "boss/Paradiso/paradiso_walk/paradiso_walk_SE/paradiso_walk_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_walk/paradiso_walk_SW/paradiso_walk_SW.plist", "boss/Paradiso/paradiso_walk/paradiso_walk_SW/paradiso_walk_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_walk/paradiso_walk_W/paradiso_walk_W.plist", "boss/Paradiso/paradiso_walk/paradiso_walk_W/paradiso_walk_W.png");
    // healing
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_healing/paradiso_healing_E/paradiso_healing_E.plist", "boss/Paradiso/paradiso_healing/paradiso_healing_E/paradiso_healing_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_healing/paradiso_healing_N/paradiso_healing_N.plist", "boss/Paradiso/paradiso_healing/paradiso_healing_N/paradiso_healing_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_healing/paradiso_healing_NE/paradiso_healing_NE.plist", "boss/Paradiso/paradiso_healing/paradiso_healing_NE/paradiso_healing_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_healing/paradiso_healing_NW/paradiso_healing_NW.plist", "boss/Paradiso/paradiso_healing/paradiso_healing_NW/paradiso_healing_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_healing/paradiso_healing_S/paradiso_healing_S.plist", "boss/Paradiso/paradiso_healing/paradiso_healing_S/paradiso_healing_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_healing/paradiso_healing_SE/paradiso_healing_SE.plist", "boss/Paradiso/paradiso_healing/paradiso_healing_SE/paradiso_healing_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_healing/paradiso_healing_SW/paradiso_healing_SW.plist", "boss/Paradiso/paradiso_healing/paradiso_healing_SW/paradiso_healing_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_healing/paradiso_healing_W/paradiso_healing_W.plist", "boss/Paradiso/paradiso_healing/paradiso_healing_W/paradiso_healing_W.png");
    // death
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_death/paradiso_death_E/paradiso_death_E.plist", "boss/Paradiso/paradiso_death/paradiso_death_E/paradiso_death_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_death/paradiso_death_N/paradiso_death_N.plist", "boss/Paradiso/paradiso_death/paradiso_death_N/paradiso_death_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_death/paradiso_death_NE/paradiso_death_NE.plist", "boss/Paradiso/paradiso_death/paradiso_death_NE/paradiso_death_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_death/paradiso_death_NW/paradiso_death_NW.plist", "boss/Paradiso/paradiso_death/paradiso_death_NW/paradiso_death_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_death/paradiso_death_S/paradiso_death_S.plist", "boss/Paradiso/paradiso_death/paradiso_death_S/paradiso_death_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_death/paradiso_death_SE/paradiso_death_SE.plist", "boss/Paradiso/paradiso_death/paradiso_death_SE/paradiso_death_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_death/paradiso_death_SW/paradiso_death_SW.plist", "boss/Paradiso/paradiso_death/paradiso_death_SW/paradiso_death_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_death/paradiso_death_W/paradiso_death_W.plist", "boss/Paradiso/paradiso_death/paradiso_death_W/paradiso_death_W.png");
    // attack 1
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack1/paradiso_attack1_E/paradiso_attack1_E.plist", "boss/Paradiso/paradiso_attack1/paradiso_attack1_E/paradiso_attack1_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack1/paradiso_attack1_N/paradiso_attack1_N.plist", "boss/Paradiso/paradiso_attack1/paradiso_attack1_N/paradiso_attack1_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack1/paradiso_attack1_NE/paradiso_attack1_NE.plist", "boss/Paradiso/paradiso_attack1/paradiso_attack1_NE/paradiso_attack1_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack1/paradiso_attack1_NW/paradiso_attack1_NW.plist", "boss/Paradiso/paradiso_attack1/paradiso_attack1_NW/paradiso_attack1_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack1/paradiso_attack1_S/paradiso_attack1_S.plist", "boss/Paradiso/paradiso_attack1/paradiso_attack1_S/paradiso_attack1_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack1/paradiso_attack1_SE/paradiso_attack1_SE.plist", "boss/Paradiso/paradiso_attack1/paradiso_attack1_SE/paradiso_attack1_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack1/paradiso_attack1_SW/paradiso_attack1_SW.plist", "boss/Paradiso/paradiso_attack1/paradiso_attack1_SW/paradiso_attack1_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack1/paradiso_attack1_W/paradiso_attack1_W.plist", "boss/Paradiso/paradiso_attack1/paradiso_attack1_W/paradiso_attack1_W.png");

    // attack 2
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack2/paradiso_attack_2_E/paradiso_attack2_E.plist", "boss/Paradiso/paradiso_attack2/paradiso_attack_2_E/paradiso_attack2_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack2/paradiso_attack_2_N/paradiso_attack2_N.plist", "boss/Paradiso/paradiso_attack2/paradiso_attack_2_N/paradiso_attack2_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack2/paradiso_attack_2_NE/paradiso_attack2_NE.plist", "boss/Paradiso/paradiso_attack2/paradiso_attack_2_NE/paradiso_attack2_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack2/paradiso_attack_2_NW/paradiso_attack2_NW.plist", "boss/Paradiso/paradiso_attack2/paradiso_attack_2_NW/paradiso_attack2_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack2/paradiso_attack_2_S/paradiso_attack2_S.plist", "boss/Paradiso/paradiso_attack2/paradiso_attack_2_S/paradiso_attack2_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack2/paradiso_attack_2_SE/paradiso_attack2_SE.plist", "boss/Paradiso/paradiso_attack2/paradiso_attack_2_SE/paradiso_attack2_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack2/paradiso_attack_2_SW/paradiso_attack2_SW.plist", "boss/Paradiso/paradiso_attack2/paradiso_attack_2_SW/paradiso_attack2_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Paradiso/paradiso_attack2/paradiso_attack_2_W/paradiso_attack2_W.plist", "boss/Paradiso/paradiso_attack2/paradiso_attack_2_W/paradiso_attack2_W.png");
}

void GameMap::loadOkyanusToCache()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Angry/okyanus_angry_E/okyanus_angry_E.plist", "boss/Okyanus/Angry/okyanus_angry_E/okyanus_angry_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Angry/okyanus_angry_N/okyanus_angry_N.plist", "boss/Okyanus/Angry/okyanus_angry_N/okyanus_angry_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Angry/okyanus_angry_NE/okyanus_angry_NE.plist", "boss/Okyanus/Angry/okyanus_angry_NE/okyanus_angry_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Angry/okyanus_angry_NW/okyanus_angry_NW.plist", "boss/Okyanus/Angry/okyanus_angry_NW/okyanus_angry_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Angry/okyanus_angry_S/okyanus_angry_S.plist", "boss/Okyanus/Angry/okyanus_angry_S/okyanus_angry_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Angry/okyanus_angry_SE/okyanus_angry_SE.plist", "boss/Okyanus/Angry/okyanus_angry_SE/okyanus_angry_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Angry/okyanus_angry_SW/okyanus_angry_SW.plist", "boss/Okyanus/Angry/okyanus_angry_SW/okyanus_angry_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Angry/okyanus_angry_W/okyanus_angry_W.plist", "boss/Okyanus/Angry/okyanus_angry_W/okyanus_angry_W.png");
    // Attack1
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack1/okyanus_attack1_E/okyanus_attack1_E.plist", "boss/Okyanus/Attack1/okyanus_attack1_E/okyanus_attack1_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack1/okyanus_attack1_N/okyanus_attack1_N.plist", "boss/Okyanus/Attack1/okyanus_attack1_N/okyanus_attack1_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack1/okyanus_attack1_NE/okyanus_attack1_NE.plist", "boss/Okyanus/Attack1/okyanus_attack1_NE/okyanus_attack1_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack1/okyanus_attack1_NW/okyanus_attack1_NW.plist", "boss/Okyanus/Attack1/okyanus_attack1_NW/okyanus_attack1_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack1/okyanus_attack1_S/okyanus_attack1_S.plist", "boss/Okyanus/Attack1/okyanus_attack1_S/okyanus_attack1_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack1/okyanus_attack1_SE/okyanus_attack1_SE.plist", "boss/Okyanus/Attack1/okyanus_attack1_SE/okyanus_attack1_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack1/okyanus_attack1_SW/okyanus_attack1_SW.plist", "boss/Okyanus/Attack1/okyanus_attack1_SW/okyanus_attack1_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack1/okyanus_attack1_W/okyanus_attack1_W.plist", "boss/Okyanus/Attack1/okyanus_attack1_W/okyanus_attack1_W.png");

    // Attack2
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack2/okyanus_attack2_E/okyanus_attack2_E.plist", "boss/Okyanus/Attack2/okyanus_attack2_E/okyanus_attack2_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack2/okyanus_attack2_N/okyanus_attack2_N.plist", "boss/Okyanus/Attack2/okyanus_attack2_N/okyanus_attack2_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack2/okyanus_attack2_NE/okyanus_attack2_NE.plist", "boss/Okyanus/Attack2/okyanus_attack2_NE/okyanus_attack2_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack2/okyanus_attack2_NW/okyanus_attack2_NW.plist", "boss/Okyanus/Attack2/okyanus_attack2_NW/okyanus_attack2_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack2/okyanus_attack2_S/okyanus_attack2_S.plist", "boss/Okyanus/Attack2/okyanus_attack2_S/okyanus_attack2_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack2/okyanus_attack2_SE/okyanus_attack2_SE.plist", "boss/Okyanus/Attack2/okyanus_attack2_SE/okyanus_attack2_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack2/okyanus_attack2_SW/okyanus_attack2_SW.plist", "boss/Okyanus/Attack2/okyanus_attack2_SW/okyanus_attack2_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack2/okyanus_attack2_W/okyanus_attack2_W.plist", "boss/Okyanus/Attack2/okyanus_attack2_W/okyanus_attack2_W.png");
    // Attack3
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack3/okyanus_attack3_E/okyanus_attack3_E.plist", "boss/Okyanus/Attack3/okyanus_attack3_E/okyanus_attack3_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack3/okyanus_attack3_N/okyanus_attack3_N.plist", "boss/Okyanus/Attack3/okyanus_attack3_N/okyanus_attack3_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack3/okyanus_attack3_NE/okyanus_attack3_NE.plist", "boss/Okyanus/Attack3/okyanus_attack3_NE/okyanus_attack3_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack3/okyanus_attack3_NW/okyanus_attack3_NW.plist", "boss/Okyanus/Attack3/okyanus_attack3_NW/okyanus_attack3_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack3/okyanus_attack3_S/okyanus_attack3_S.plist", "boss/Okyanus/Attack3/okyanus_attack3_S/okyanus_attack3_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack3/okyanus_attack3_SE/okyanus_attack3_SE.plist", "boss/Okyanus/Attack3/okyanus_attack3_SE/okyanus_attack3_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack3/okyanus_attack3_SW/okyanus_attack3_SW.plist", "boss/Okyanus/Attack3/okyanus_attack3_SW/okyanus_attack3_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Attack3/okyanus_attack3_W/okyanus_attack3_W.plist", "boss/Okyanus/Attack3/okyanus_attack3_W/okyanus_attack3_W.png");
    // Death
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Death/okyanus_death_E/okyanus_death_E.plist", "boss/Okyanus/Death/okyanus_death_E/okyanus_death_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Death/okyanus_death_N/okyanus_death_N.plist", "boss/Okyanus/Death/okyanus_death_N/okyanus_death_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Death/okyanus_death_NE/okyanus_death_NE.plist", "boss/Okyanus/Death/okyanus_death_NE/okyanus_death_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Death/okyanus_death_NW/okyanus_death_NW.plist", "boss/Okyanus/Death/okyanus_death_NW/okyanus_death_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Death/okyanus_death_S/okyanus_death_S.plist", "boss/Okyanus/Death/okyanus_death_S/okyanus_death_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Death/okyanus_death_SE/okyanus_death_SE.plist", "boss/Okyanus/Death/okyanus_death_SE/okyanus_death_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Death/okyanus_death_SW/okyanus_death_SW.plist", "boss/Okyanus/Death/okyanus_death_SW/okyanus_death_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Death/okyanus_death_W/okyanus_death_W.plist", "boss/Okyanus/Death/okyanus_death_W/okyanus_death_W.png");
    // Hit
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Hit/okyanus_hit_E/okyanus_hit_E.plist", "boss/Okyanus/Hit/okyanus_hit_E/okyanus_hit_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Hit/okyanus_hit_N/okyanus_hit_N.plist", "boss/Okyanus/Hit/okyanus_hit_N/okyanus_hit_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Hit/okyanus_hit_NE/okyanus_hit_NE.plist", "boss/Okyanus/Hit/okyanus_hit_NE/okyanus_hit_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Hit/okyanus_hit_NW/okyanus_hit_NW.plist", "boss/Okyanus/Hit/okyanus_hit_NW/okyanus_hit_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Hit/okyanus_hit_S/okyanus_hit_S.plist", "boss/Okyanus/Hit/okyanus_hit_S/okyanus_hit_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Hit/okyanus_hit_SE/okyanus_hit_SE.plist", "boss/Okyanus/Hit/okyanus_hit_SE/okyanus_hit_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Hit/okyanus_hit_SW/okyanus_hit_SW.plist", "boss/Okyanus/Hit/okyanus_hit_SW/okyanus_hit_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Hit/okyanus_hit_W/okyanus_hit_W.plist", "boss/Okyanus/Hit/okyanus_hit_W/okyanus_hit_W.png");
    // Idle
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Idle/okyanus_idle_E/okyanus_idle_E.plist", "boss/Okyanus/Idle/okyanus_idle_E/okyanus_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Idle/okyanus_idle_N/okyanus_idle_N.plist", "boss/Okyanus/Idle/okyanus_idle_N/okyanus_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Idle/okyanus_idle_NE/okyanus_idle_NE.plist", "boss/Okyanus/Idle/okyanus_idle_NE/okyanus_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Idle/okyanus_idle_NW/okyanus_idle_NW.plist", "boss/Okyanus/Idle/okyanus_idle_NW/okyanus_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Idle/okyanus_idle_S/okyanus_idle_S.plist", "boss/Okyanus/Idle/okyanus_idle_S/okyanus_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Idle/okyanus_idle_SE/okyanus_idle_SE.plist", "boss/Okyanus/Idle/okyanus_idle_SE/okyanus_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Idle/okyanus_idle_SW/okyanus_idle_SW.plist", "boss/Okyanus/Idle/okyanus_idle_SW/okyanus_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/Okyanus/Idle/okyanus_idle_W/okyanus_idle_W.plist", "boss/Okyanus/Idle/okyanus_idle_W/okyanus_idle_W.png");
}
void GameMap::loadNPCFighterToCache()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_armed_idle/E/fighter_armed_idle_E.plist", "playable character/fighter/fighter_armed_idle/E/fighter_armed_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_armed_idle/N/fighter_armed_idle_N.plist", "playable character/fighter/fighter_armed_idle/N/fighter_armed_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_armed_idle/NE/fighter_armed_idle_NE.plist", "playable character/fighter/fighter_armed_idle/NE/fighter_armed_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_armed_idle/NW/fighter_armed_idle_NW.plist", "playable character/fighter/fighter_armed_idle/NW/fighter_armed_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_armed_idle/S/fighter_armed_idle_S.plist", "playable character/fighter/fighter_armed_idle/S/fighter_armed_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_armed_idle/SE/fighter_armed_idle_SE.plist", "playable character/fighter/fighter_armed_idle/SE/fighter_armed_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_armed_idle/SW/fighter_armed_idle_SW.plist", "playable character/fighter/fighter_armed_idle/SW/fighter_armed_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_armed_idle/W/fighter_armed_idle_W.plist", "playable character/fighter/fighter_armed_idle/W/fighter_armed_idle_W.png");

    // SELECT
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_special_select/E/fighter_special_select_E.plist", "playable character/fighter/fighter_special_select/E/fighter_special_select_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_special_select/N/fighter_special_select_N.plist", "playable character/fighter/fighter_special_select/N/fighter_special_select_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_special_select/NE/fighter_special_select_NE.plist", "playable character/fighter/fighter_special_select/NE/fighter_special_select_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_special_select/NW/fighter_special_select_NW.plist", "playable character/fighter/fighter_special_select/NW/fighter_special_select_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_special_select/S/fighter_special_select_S.plist", "playable character/fighter/fighter_special_select/S/fighter_special_select_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_special_select/SE/fighter_special_select_SE.plist", "playable character/fighter/fighter_special_select/SE/fighter_special_select_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_special_select/SW/fighter_special_select_SW.plist", "playable character/fighter/fighter_special_select/SW/fighter_special_select_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/fighter/fighter_special_select/W/fighter_special_select_W.plist", "playable character/fighter/fighter_special_select/W/fighter_special_select_W.png");
}
void GameMap::loadWarriorToCache()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_armed_idle/E/warrior_armed_idle_E.plist", "playable character/warrior/warrior_armed_idle/E/warrior_armed_idle_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_armed_idle/N/warrior_armed_idle_N.plist", "playable character/warrior/warrior_armed_idle/N/warrior_armed_idle_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_armed_idle/NE/warrior_armed_idle_NE.plist", "playable character/warrior/warrior_armed_idle/NE/warrior_armed_idle_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_armed_idle/NW/warrior_armed_idle_NW.plist", "playable character/warrior/warrior_armed_idle/NW/warrior_armed_idle_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_armed_idle/S/warrior_armed_idle_S.plist", "playable character/warrior/warrior_armed_idle/S/warrior_armed_idle_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_armed_idle/SE/warrior_armed_idle_SE.plist", "playable character/warrior/warrior_armed_idle/SE/warrior_armed_idle_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_armed_idle/SW/warrior_armed_idle_SW.plist", "playable character/warrior/warrior_armed_idle/SW/warrior_armed_idle_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_armed_idle/W/warrior_armed_idle_W.plist", "playable character/warrior/warrior_armed_idle/W/warrior_armed_idle_W.png");

    // SELECT
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_special_select/E/warrior_special_select_E.plist", "playable character/warrior/warrior_special_select/E/warrior_special_select_E.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_special_select/N/warrior_special_select_N.plist", "playable character/warrior/warrior_special_select/N/warrior_special_select_N.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_special_select/NE/warrior_special_select_NE.plist", "playable character/warrior/warrior_special_select/NE/warrior_special_select_NE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_special_select/NW/warrior_special_select_NW.plist", "playable character/warrior/warrior_special_select/NW/warrior_special_select_NW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_special_select/S/warrior_special_select_S.plist", "playable character/warrior/warrior_special_select/S/warrior_special_select_S.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_special_select/SE/warrior_special_select_SE.plist", "playable character/warrior/warrior_special_select/SE/warrior_special_select_SE.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_special_select/SW/warrior_special_select_SW.plist", "playable character/warrior/warrior_special_select/SW/warrior_special_select_SW.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playable character/warrior/warrior_special_select/W/warrior_special_select_W.plist", "playable character/warrior/warrior_special_select/W/warrior_special_select_W.png");
}