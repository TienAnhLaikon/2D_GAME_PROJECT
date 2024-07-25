#ifndef __JSON_MANAGER_H__
#define __JSON_MANAGER_H__

#include "cocos2d.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "Player/Player.h"
#include "Quest/QuestManager.h"
USING_NS_CC;

static const std::string BASE_PLAYER_PATH = "res/json/BasePlayer.json";


struct BasePlayer {
    int Max_level = 0;
    float HP = 0;
    float MP = 0;
    float HP_regen = 0;
    float MP_regen = 0;
    float Armor = 0;
    float AD = 0;
    float AP = 0;
    float MS = 0;
    float Attack_range = 0;
    float Attack_speed = 0;
    float Exp_gain = 0;
    float CDR = 0;
    float HP_bonus = 0;
    float MP_bonus = 0;
    float HP_regen_bonus = 0;
    float MP_regen_bonus = 0;
    float Armor_bonus = 0;
    float AD_bonus = 0;
    float AP_bonus = 0;
    float MS_bonus = 0;
    float AR_bonus = 0;
    float AS_bonus = 0;
    float Exp_gain_bonus = 0;
};

class BaseEquipment;
class SkillBase;
class NormalMonster;
class JsonManager
{
public:
    static JsonManager* getInstance();
    static void destroyInstance();
    std::string loadJsonFile(const std::string& filename);
    JsonManager();
    ~JsonManager();
    bool init();
    bool saveJsonFile(const std::string& filename, const std::string& content);
    void saveGame(Player::PlayerData playerData, std::vector<NormalMonster*> listOfMonster);
    //bool saveMonsterQuest(KillMonsterQuest::KillMonsterQuestData monsterQuestData); // s dng trong updatePlayer (game scene) khi player chm vo end point (cng vi save game)
    //bool saveCollectItemQuest(CollectItemQuest::CollectItemQuestData collectItemQuestData); // s dng trong updatePlayer (game scene) khi player chm vo end point (cng vi save game)
    //bool saveBossQuest(KillBossQuest::KillBossQuestData killBossQuestData); // s dng trong updatePlayer (game scene) khi player chm vo end point (cng vi save game)
    //KillMonsterQuest::KillMonsterQuestData getMonsterQuest(); // s dng trong questManager khi  d liu vo KillMonsterQuest
    //CollectItemQuest::CollectItemQuestData getCollectItemQuest(); // s dng trong questManager khi  d liu vo CollectItemQuest
    //KillBossQuest::KillBossQuestData getBossQuest(); // s dng trong questManager khi  d liu vo KillBossQuest
    Player::PlayerData getPlayerData();
    std::vector<NormalMonster*> getEnemiesData();
    std::vector<BaseEquipment::EquipmentData> getEquipmentsData();
    std::vector<std::string> getItemsData();
    std::vector<SkillBase*> getSkillsData();
    bool saveSkillsData(std::vector<SkillBase*> listOfSkill);
    bool saveQuest(Quest::QuestData questData);
    Quest::QuestData getQuestData();
    bool saveItemsData(std::vector<std::string> listOfItem, std::vector<std::pair<std::string, int>> subInventoryItems);
    std::vector<std::pair<std::string, int>> getUsingItem();
    bool saveEquipmentsData(std::vector<BaseEquipment::EquipmentData> listOfEquipment, Player::PlayerUsingEquipment playerUsingEquipment);
    Player::PlayerUsingEquipment getPlayerUsingEquipmentData();
    BasePlayer getBasePlayer();
private:
    bool savePlayerData(Player::PlayerData);
    bool saveEnemiesData(std::vector<NormalMonster*> listOfMonster);
    static JsonManager* s_instance;
    rapidjson::Document jDoc;
    rapidjson::Document saveGameDoc;
    rapidjson::Document saveQuestDoc;
    rapidjson::Document basePlayerDoc;
    std::string basePlayerPath;
    std::string path;
    std::string questPath;
};



#endif // __JSON_MANAGER_H__
