#include "cocos2d.h"
#include "Player/Player.h"
#include "Enemies/NormalMonster/NormalMonsterFactory.h"
#include "Equipment/EquipmentFactory.h"
#include "Item/ItemFactory.h"
#include "Skills/SkillFactory.h"
#include "JsonManager.h"

USING_NS_CC;

JsonManager* JsonManager::s_instance = nullptr;

JsonManager* JsonManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new JsonManager();
        s_instance->init();
    }
    return s_instance;
}

JsonManager::JsonManager() {}

bool JsonManager::init()
{
    std::string filename = "SoulSlayer-data.json";
    std::string questFilename = "SoulSlayer-quest-data.json";
    path = cocos2d::FileUtils::getInstance()->getWritablePath() + filename;
    questPath = cocos2d::FileUtils::getInstance()->getWritablePath() + questFilename;
    basePlayerPath = FileUtils::getInstance()->fullPathForFilename("BasePlayer.json");

    if (!FileUtils::getInstance()->isFileExist(path))
    {
        std::string content = "{\"Player\":{\"posX\":0,\"posY\":0,\"exp\":0,\"expToConsume\":0,\"health\":0,\"currentMap\":0,\"prevMap\":0,\"mana\":0,\"currentQuest\":0},\"Skills\":[],\"SkillPoint\":0,\"ActiveItems\":[],\"ActiveEquipments\":[],\"UsingEquipments\":[{\"name\":\"\",\"level\":0,\"type\":\"\"},{\"name\":\"\",\"level\":0,\"type\":\"\"},{\"name\":\"\",\"level\":0,\"type\":\"\"},{\"name\":\"\",\"level\":0,\"type\":\"\"},{\"name\":\"\",\"level\":0,\"type\":\"\"},{\"name\":\"\",\"level\":0,\"type\":\"\"}], \"UsingItems\":[],\"Enemies\":[]}";
        saveJsonFile(path, content);
    }
    if (!FileUtils::getInstance()->isFileExist(questPath)) {
        std::string questContent = "{\"SavedQuest\":{\"questName\":\"Free the sinful souls\",\"questStatus\":0,\"enemies\":[{\"name\":\"Skeleton\",\"killed\":0,\"required\":3}],\"items\":[{\"name\":\"crystal\",\"collected\":0,\"required\":5}]}}"; // \"name\":\"quest name 1\", \"monsters\": []
        saveJsonFile(questPath, questContent);
    }

    std::string basePlayerContent = cocos2d::FileUtils::getInstance()->getStringFromFile(basePlayerPath);
    basePlayerDoc.Parse(basePlayerContent.c_str());

    std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path.c_str());
    saveGameDoc.Parse(content.c_str());

    std::string questContent = cocos2d::FileUtils::getInstance()->getStringFromFile(questPath.c_str());
    saveQuestDoc.Parse(questContent.c_str());

    return true;
}
void JsonManager::saveGame(Player::PlayerData data, std::vector<NormalMonster*> listOfMonster)
{
    savePlayerData(data);
    saveEnemiesData(listOfMonster);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    saveGameDoc.Accept(writer);
    saveJsonFile(path, buffer.GetString());
}

bool JsonManager::savePlayerData(Player::PlayerData data) {
    auto playerDataIterator = saveGameDoc.FindMember("Player");
    rapidjson::Value& playerData = playerDataIterator->value;
    playerData["posX"].SetFloat(data.positionX);
    playerData["posY"].SetFloat(data.positionY);
    playerData["exp"].SetFloat(data.exp);
    playerData["expToConsume"].SetFloat(data.expToConsume);
    playerData["health"].SetFloat(data.health);
    playerData["mana"].SetFloat(data.mana);
    playerData["currentQuest"].SetInt(data.currentQuest);
    playerData["currentMap"].SetInt(data.currentMap);
    playerData["prevMap"].SetInt(data.prevMap);
    // using item
    return true;
}

bool JsonManager::saveEnemiesData(std::vector<NormalMonster*> listOfMonster) {
    auto enemiesDataIterator = saveGameDoc.FindMember("Enemies");
    rapidjson::Value& enemiesData = enemiesDataIterator->value;
    if (enemiesDataIterator == saveGameDoc.MemberEnd() || !enemiesDataIterator->value.IsArray()) {
        enemiesData.SetArray();
        saveGameDoc.AddMember("Enemies", enemiesData, saveGameDoc.GetAllocator());
    }
    else {
        enemiesData.Clear();
    }
    for (auto enemy : listOfMonster) {
        rapidjson::Value enemyObject(rapidjson::kObjectType);
        std::string enemyName = enemy->getEnemyName();
        rapidjson::Value rjv;
        rjv.SetString(enemyName.c_str(), saveGameDoc.GetAllocator());
        enemyObject.AddMember("name", rjv, saveGameDoc.GetAllocator());
        enemyObject.AddMember("posX", enemy->getPositionX(), saveGameDoc.GetAllocator());
        enemyObject.AddMember("posY", enemy->getPositionY(), saveGameDoc.GetAllocator());
        enemyObject.AddMember("health", enemy->getCurrentHP(), saveGameDoc.GetAllocator());
        enemiesData.PushBack(enemyObject, saveGameDoc.GetAllocator());
    }

    return true;
}

bool JsonManager::saveQuest(Quest::QuestData questData) { //{\"questName\":\"\",\"enemies\":[],\"items\":[]}
    auto questIterator = saveQuestDoc.FindMember("SavedQuest");
    rapidjson::Value& questDataVal = questIterator->value;
    if (questIterator == saveQuestDoc.MemberEnd() || !questIterator->value.IsObject()) {
        questDataVal.SetObject();
        saveQuestDoc.AddMember("SavedQuest", questDataVal, saveQuestDoc.GetAllocator());
    }
    else {
        rapidjson::Value& questDataVal = questIterator->value;
        questDataVal.RemoveAllMembers(); // Xa tt c cc thnh vin trong i tng "SavedQuest"

        // Nu c cc mng bn trong, cng c th s dng Clear()  lm trng chng
        for (auto& member : questDataVal.GetObject()) {
            if (member.value.IsArray()) {
                member.value.Clear(); // Lm trng mng
            }
        }
    }
    // Add questName
    rapidjson::Value questName;
    questName.SetString(questData.questName.c_str(), saveQuestDoc.GetAllocator());
    questDataVal.AddMember("questName", questName, saveQuestDoc.GetAllocator());
   
    // Add questStatus
    rapidjson::Value questStatus;
    questStatus.SetInt(questData.questStatus);
    questDataVal.AddMember("questStatus", questStatus, saveQuestDoc.GetAllocator());

    // Initialize enemies array
    rapidjson::Value enemiesArray(rapidjson::kArrayType);
    for (auto& enemy : questData.enemies) {
        rapidjson::Value enemyObject(rapidjson::kObjectType);
        rapidjson::Value enemyName;
        enemyName.SetString(enemy->name.c_str(), saveQuestDoc.GetAllocator());
        enemyObject.AddMember("name", enemyName, saveQuestDoc.GetAllocator());
        enemyObject.AddMember("killed", enemy->killed, saveQuestDoc.GetAllocator());
        enemyObject.AddMember("required", enemy->required, saveQuestDoc.GetAllocator());
        enemiesArray.PushBack(enemyObject, saveQuestDoc.GetAllocator());
    }
    questDataVal.AddMember("enemies", enemiesArray, saveQuestDoc.GetAllocator());

    // Initialize items array
    rapidjson::Value itemsArray(rapidjson::kArrayType);
    for (auto& item : questData.items) {
        rapidjson::Value itemObject(rapidjson::kObjectType);
        rapidjson::Value itemName;
        itemName.SetString(item->name.c_str(), saveQuestDoc.GetAllocator());
        itemObject.AddMember("name", itemName, saveQuestDoc.GetAllocator());
        itemObject.AddMember("collected", item->collected, saveQuestDoc.GetAllocator());
        itemObject.AddMember("required", item->required, saveQuestDoc.GetAllocator());
        itemsArray.PushBack(itemObject, saveQuestDoc.GetAllocator());
    }
    questDataVal.AddMember("items", itemsArray, saveQuestDoc.GetAllocator());

    // Serialize JSON document and save to file
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    saveQuestDoc.Accept(writer);
    saveJsonFile(questPath, buffer.GetString());

    return true;

}
bool JsonManager::saveEquipmentsData(std::vector<BaseEquipment::EquipmentData> listOfEquipment, Player::PlayerUsingEquipment playerUsingEquipment) {
    auto equipmentDataIterator = saveGameDoc.FindMember("ActiveEquipments");
    rapidjson::Value& equipmentData = equipmentDataIterator->value;
    if (equipmentDataIterator == saveGameDoc.MemberEnd() || !equipmentDataIterator->value.IsArray()) {
        equipmentData.SetArray();
        saveGameDoc.AddMember("ActiveEquipments", equipmentData, saveGameDoc.GetAllocator());
    }
    else {
        equipmentData.Clear();
    }

    for (auto equipment : listOfEquipment) {
        rapidjson::Value equipmentObject(rapidjson::kObjectType);
        const std::string equipmentName = equipment.name;
        rapidjson::Value rjv;
        rjv.SetString(equipmentName.c_str(), saveGameDoc.GetAllocator());
        equipmentObject.AddMember("name", rjv, saveGameDoc.GetAllocator());
        equipmentObject.AddMember("level", equipment.level, saveGameDoc.GetAllocator());
        equipmentData.PushBack(equipmentObject, saveGameDoc.GetAllocator());
    }


    auto usingEquipmentDataIterator = saveGameDoc.FindMember("UsingEquipments");
    rapidjson::Value& usingEquipmentData = usingEquipmentDataIterator->value;
    if (usingEquipmentDataIterator == saveGameDoc.MemberEnd() || !usingEquipmentDataIterator->value.IsArray()) {
        usingEquipmentData.SetArray();
        saveGameDoc.AddMember("UsingEquipments", usingEquipmentData, saveGameDoc.GetAllocator());
    }
    else {
        usingEquipmentData.Clear();
    }

    rapidjson::Value weaponObject(rapidjson::kObjectType);
    const std::string weaponName = std::get<0>(playerUsingEquipment.weaponNLevel);
    rapidjson::Value weaponNameVal;
    const std::string equipmentType = std::get<2>(playerUsingEquipment.weaponNLevel);
    rapidjson::Value equipmentTypeVal;
    weaponNameVal.SetString(weaponName.c_str(), saveGameDoc.GetAllocator());
    equipmentTypeVal.SetString(equipmentType.c_str(), saveGameDoc.GetAllocator());
    weaponObject.AddMember("name", weaponNameVal, saveGameDoc.GetAllocator());
    weaponObject.AddMember("level", std::get<1>(playerUsingEquipment.weaponNLevel), saveGameDoc.GetAllocator());
    weaponObject.AddMember("type", equipmentTypeVal, saveGameDoc.GetAllocator());
    usingEquipmentData.PushBack(weaponObject, saveGameDoc.GetAllocator());

    rapidjson::Value armorObject(rapidjson::kObjectType);
    const std::string armorName = std::get<0>(playerUsingEquipment.armorNLevel);
    rapidjson::Value armorNameVal;
    const std::string armorType = std::get<2>(playerUsingEquipment.armorNLevel);
    rapidjson::Value armorTypeVal;
    armorNameVal.SetString(armorName.c_str(), saveGameDoc.GetAllocator());
    armorTypeVal.SetString(armorType.c_str(), saveGameDoc.GetAllocator());
    armorObject.AddMember("name", armorNameVal, saveGameDoc.GetAllocator());
    armorObject.AddMember("level", std::get<1>(playerUsingEquipment.armorNLevel), saveGameDoc.GetAllocator());
    armorObject.AddMember("type", armorTypeVal, saveGameDoc.GetAllocator());
    usingEquipmentData.PushBack(armorObject, saveGameDoc.GetAllocator());

    rapidjson::Value glovesObject(rapidjson::kObjectType);
    const std::string glovesName = std::get<0>(playerUsingEquipment.shieldNLevel);
    rapidjson::Value glovesNameVal;
    const std::string glovesType = std::get<2>(playerUsingEquipment.shieldNLevel);
    rapidjson::Value glovesTypeVal;
    glovesNameVal.SetString(glovesName.c_str(), saveGameDoc.GetAllocator());
    glovesTypeVal.SetString(glovesType.c_str(), saveGameDoc.GetAllocator());
    glovesObject.AddMember("name", glovesNameVal, saveGameDoc.GetAllocator());
    glovesObject.AddMember("level", std::get<1>(playerUsingEquipment.shieldNLevel), saveGameDoc.GetAllocator());
    glovesObject.AddMember("type", glovesTypeVal, saveGameDoc.GetAllocator());
    usingEquipmentData.PushBack(glovesObject, saveGameDoc.GetAllocator());

    rapidjson::Value ringObject(rapidjson::kObjectType);
    const std::string ringName = std::get<0>(playerUsingEquipment.ringNLevel);
    rapidjson::Value ringNameVal;
    const std::string ringType = std::get<2>(playerUsingEquipment.ringNLevel);
    rapidjson::Value ringTypeVal;
    ringNameVal.SetString(ringName.c_str(), saveGameDoc.GetAllocator());
    ringTypeVal.SetString(ringType.c_str(), saveGameDoc.GetAllocator());
    ringObject.AddMember("name", ringNameVal, saveGameDoc.GetAllocator());
    ringObject.AddMember("level", std::get<1>(playerUsingEquipment.ringNLevel), saveGameDoc.GetAllocator());
    ringObject.AddMember("type", ringTypeVal, saveGameDoc.GetAllocator());
    usingEquipmentData.PushBack(ringObject, saveGameDoc.GetAllocator());

    rapidjson::Value shoeObject(rapidjson::kObjectType);
    const std::string shoeName = std::get<0>(playerUsingEquipment.shoeNLevel);
    rapidjson::Value shoeNameVal;
    const std::string shoeType = std::get<2>(playerUsingEquipment.shoeNLevel);
    rapidjson::Value shoeTypeVal;
    shoeNameVal.SetString(shoeName.c_str(), saveGameDoc.GetAllocator());
    shoeTypeVal.SetString(shoeType.c_str(), saveGameDoc.GetAllocator());
    shoeObject.AddMember("name", shoeNameVal, saveGameDoc.GetAllocator());
    shoeObject.AddMember("level", std::get<1>(playerUsingEquipment.shoeNLevel), saveGameDoc.GetAllocator());
    shoeObject.AddMember("type", shoeTypeVal, saveGameDoc.GetAllocator());
    usingEquipmentData.PushBack(shoeObject, saveGameDoc.GetAllocator());

    rapidjson::Value helmetObject(rapidjson::kObjectType);
    const std::string helmetName = std::get<0>(playerUsingEquipment.helmetNLevel);
    rapidjson::Value helmetNameVal;
    const std::string helmetType = std::get<2>(playerUsingEquipment.helmetNLevel);
    rapidjson::Value helmetTypeVal;
    helmetNameVal.SetString(helmetName.c_str(), saveGameDoc.GetAllocator());
    helmetTypeVal.SetString(helmetType.c_str(), saveGameDoc.GetAllocator());
    helmetObject.AddMember("name", helmetNameVal, saveGameDoc.GetAllocator());
    helmetObject.AddMember("level", std::get<1>(playerUsingEquipment.helmetNLevel), saveGameDoc.GetAllocator());
    helmetObject.AddMember("type", helmetTypeVal, saveGameDoc.GetAllocator());
    usingEquipmentData.PushBack(helmetObject, saveGameDoc.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    saveGameDoc.Accept(writer);
    saveJsonFile(path, buffer.GetString());
    return true;
}

bool JsonManager::saveItemsData(std::vector<std::string> listOfItem, std::vector<std::pair<std::string, int>> subInventoryItems) {
    auto itemDataIterator = saveGameDoc.FindMember("ActiveItems");
    rapidjson::Value& itemData = itemDataIterator->value;
    if (itemDataIterator == saveGameDoc.MemberEnd() || !itemDataIterator->value.IsArray()) {
        itemData.SetArray();
        saveGameDoc.AddMember("ActiveItems", itemData, saveGameDoc.GetAllocator());
    }
    else {
        itemData.Clear();
    }
    for (auto item : listOfItem) {
        rapidjson::Value itemObject(rapidjson::kObjectType);
        const std::string itemName = item;
        rapidjson::Value rjv;
        rjv.SetString(itemName.c_str(), saveGameDoc.GetAllocator());
        itemObject.AddMember("name", rjv, saveGameDoc.GetAllocator());
        itemData.PushBack(itemObject, saveGameDoc.GetAllocator());
    }


    auto usingItemIterator = saveGameDoc.FindMember("UsingItems");
    rapidjson::Value& usingItemData = usingItemIterator->value;
    if (usingItemIterator == saveGameDoc.MemberEnd() || !usingItemIterator->value.IsArray()) {
        usingItemData.SetArray();
        saveGameDoc.AddMember("UsingItems", usingItemData, saveGameDoc.GetAllocator());
    }
    else {
        usingItemData.Clear();
    }
    for (auto item : subInventoryItems) {
        rapidjson::Value itemObject(rapidjson::kObjectType);
        const std::string itemName = std::get<0>(item);
        rapidjson::Value rjv;
        rjv.SetString(itemName.c_str(), saveGameDoc.GetAllocator());
        itemObject.AddMember("name", rjv, saveGameDoc.GetAllocator());
        itemObject.AddMember("quantity", std::get<1>(item), saveGameDoc.GetAllocator());
        usingItemData.PushBack(itemObject, saveGameDoc.GetAllocator());
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    saveGameDoc.Accept(writer);
    saveJsonFile(path, buffer.GetString());
    return true;
}

bool JsonManager::saveSkillsData(std::vector<SkillBase*> listOfSkill) {
    auto skillDataIterator = saveGameDoc.FindMember("Skills");
    rapidjson::Value& skillData = skillDataIterator->value;
    if (skillDataIterator == saveGameDoc.MemberEnd() || !skillDataIterator->value.IsArray()) {
        skillData.SetArray();
        saveGameDoc.AddMember("Skills", skillData, saveGameDoc.GetAllocator());
    }
    else {
        skillData.Clear();
    }
    for (auto skill : listOfSkill) {
        rapidjson::Value skillObject(rapidjson::kObjectType);
        const std::string skillName = skill->getSkillTalent()->skillName;
        rapidjson::Value rjv;
        rjv.SetString(skillName.c_str(), saveGameDoc.GetAllocator());
        skillObject.AddMember("name", rjv, saveGameDoc.GetAllocator());
        skillObject.AddMember("level", skill->getCurrentLevelSkill(), saveGameDoc.GetAllocator());
        skillObject.AddMember("index", skill->getSkillIndex(), saveGameDoc.GetAllocator());
        skillData.PushBack(skillObject, saveGameDoc.GetAllocator());
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    saveGameDoc.Accept(writer);
    saveJsonFile(path, buffer.GetString());
    return true;
}

Player::PlayerData JsonManager::getPlayerData()
{
    Player::PlayerData playerData;
    playerData.positionX = 0;
    playerData.positionY = 0;
    playerData.currentMap = 0;
    playerData.prevMap = 0;
    playerData.exp = 0;
    playerData.expToConsume = 0;

    if (saveGameDoc.HasMember("Player")) {
        if (saveGameDoc["Player"]["posX"].IsFloat()) playerData.positionX = saveGameDoc["Player"]["posX"].GetFloat();
        if (saveGameDoc["Player"]["posY"].IsFloat()) playerData.positionY = saveGameDoc["Player"]["posY"].GetFloat();
        if (saveGameDoc["Player"]["exp"].IsFloat()) playerData.exp = saveGameDoc["Player"]["exp"].GetFloat();
        if (saveGameDoc["Player"]["expToConsume"].IsFloat()) playerData.expToConsume = saveGameDoc["Player"]["expToConsume"].GetFloat();
        if (saveGameDoc["Player"]["currentMap"].IsInt()) playerData.currentMap = saveGameDoc["Player"]["currentMap"].GetInt();
        if (saveGameDoc["Player"]["prevMap"].IsInt()) playerData.prevMap = saveGameDoc["Player"]["prevMap"].GetInt();
    }
    return playerData;
}

BasePlayer JsonManager::getBasePlayer() {
    BasePlayer result;
    if (basePlayerDoc.HasMember("BasePlayer")) {
        if (basePlayerDoc["BasePlayer"].HasMember("Max_level") && basePlayerDoc["BasePlayer"]["Max_level"].IsInt())
            result.Max_level = basePlayerDoc["BasePlayer"]["Max_level"].GetInt();

        if (basePlayerDoc["BasePlayer"].HasMember("HP") && basePlayerDoc["BasePlayer"]["HP"].IsFloat())
            result.HP = basePlayerDoc["BasePlayer"]["HP"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("MP") && basePlayerDoc["BasePlayer"]["MP"].IsFloat())
            result.MP = basePlayerDoc["BasePlayer"]["MP"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("HP_regen") && basePlayerDoc["BasePlayer"]["HP_regen"].IsFloat())
            result.HP_regen = basePlayerDoc["BasePlayer"]["HP_regen"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("MP_regen") && basePlayerDoc["BasePlayer"]["MP_regen"].IsFloat())
            result.MP_regen = basePlayerDoc["BasePlayer"]["MP_regen"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("Armor") && basePlayerDoc["BasePlayer"]["Armor"].IsFloat())
            result.Armor = basePlayerDoc["BasePlayer"]["Armor"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("AD") && basePlayerDoc["BasePlayer"]["AD"].IsFloat())
            result.AD = basePlayerDoc["BasePlayer"]["AD"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("AP") && basePlayerDoc["BasePlayer"]["AP"].IsFloat())
            result.AP = basePlayerDoc["BasePlayer"]["AP"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("MS") && basePlayerDoc["BasePlayer"]["MS"].IsFloat())
            result.MS = basePlayerDoc["BasePlayer"]["MS"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("Attack_range") && basePlayerDoc["BasePlayer"]["Attack_range"].IsFloat())
            result.Attack_range = basePlayerDoc["BasePlayer"]["Attack_range"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("Attack_speed") && basePlayerDoc["BasePlayer"]["Attack_speed"].IsFloat())
            result.Attack_speed = basePlayerDoc["BasePlayer"]["Attack_speed"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("Exp_gain") && basePlayerDoc["BasePlayer"]["Exp_gain"].IsFloat())
            result.Exp_gain = basePlayerDoc["BasePlayer"]["Exp_gain"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("CDR") && basePlayerDoc["BasePlayer"]["CDR"].IsFloat())
            result.CDR = basePlayerDoc["BasePlayer"]["CDR"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("HP_bonus") && basePlayerDoc["BasePlayer"]["HP_bonus"].IsFloat())
            result.HP_bonus = basePlayerDoc["BasePlayer"]["HP_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("MP_bonus") && basePlayerDoc["BasePlayer"]["MP_bonus"].IsFloat())
            result.MP_bonus = basePlayerDoc["BasePlayer"]["MP_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("HP_regen_bonus") && basePlayerDoc["BasePlayer"]["HP_regen_bonus"].IsFloat())
            result.HP_regen_bonus = basePlayerDoc["BasePlayer"]["HP_regen_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("MP_regen_bonus") && basePlayerDoc["BasePlayer"]["MP_regen_bonus"].IsFloat())
            result.MP_regen_bonus = basePlayerDoc["BasePlayer"]["MP_regen_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("Armor_bonus") && basePlayerDoc["BasePlayer"]["Armor_bonus"].IsFloat())
            result.Armor_bonus = basePlayerDoc["BasePlayer"]["Armor_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("AD_bonus") && basePlayerDoc["BasePlayer"]["AD_bonus"].IsFloat())
            result.AD_bonus = basePlayerDoc["BasePlayer"]["AD_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("AP_bonus") && basePlayerDoc["BasePlayer"]["AP_bonus"].IsFloat())
            result.AP_bonus = basePlayerDoc["BasePlayer"]["AP_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("MS_bonus") && basePlayerDoc["BasePlayer"]["MS_bonus"].IsFloat())
            result.MS_bonus = basePlayerDoc["BasePlayer"]["MS_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("AR_bonus") && basePlayerDoc["BasePlayer"]["AR_bonus"].IsFloat())
            result.AR_bonus = basePlayerDoc["BasePlayer"]["AR_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("AS_bonus") && basePlayerDoc["BasePlayer"]["AS_bonus"].IsFloat())
            result.AS_bonus = basePlayerDoc["BasePlayer"]["AS_bonus"].GetFloat();

        if (basePlayerDoc["BasePlayer"].HasMember("Exp_gain_bonus") && basePlayerDoc["BasePlayer"]["Exp_gain_bonus"].IsFloat())
            result.Exp_gain_bonus = basePlayerDoc["BasePlayer"]["Exp_gain_bonus"].GetFloat();
    }
    return result;
}

std::vector<NormalMonster*> JsonManager::getEnemiesData() {
    std::vector<NormalMonster*> result;
    if (saveGameDoc.HasMember("Enemies") && saveGameDoc["Enemies"].IsArray()) {
        rapidjson::Value& enemies = saveGameDoc["Enemies"];
        for (int i = 0; i < enemies.Size(); i++) {
            auto enemy = NormalMonsterFactory::createNormalMonster(enemies[i]["name"].GetString());
            enemy->setCurrentHP(enemies[i]["health"].GetFloat());
            enemy->setPosition(enemies[i]["posX"].GetFloat(), enemies[i]["posY"].GetFloat());
            enemy->setPosition(enemies[i]["posX"].GetFloat(), enemies[i]["posY"].GetFloat());
            result.push_back(enemy);
        }
    }
    return result;
}

Quest::QuestData JsonManager::getQuestData() {
    Quest::QuestData result;
    if (saveQuestDoc.HasMember("SavedQuest") && saveQuestDoc["SavedQuest"].IsObject()) {
        rapidjson::Value& questData = saveQuestDoc["SavedQuest"];
        result.questName = questData["questName"].GetString();
        for (int i = 0; i < questData["enemies"].Size(); i++) {
            Quest::Enemy* enemy =new Quest::Enemy(questData["enemies"][i]["name"].GetString(), questData["enemies"][i]["killed"].GetInt(), questData["enemies"][i]["required"].GetInt());
            result.enemies.push_back(enemy);
        }
        for (int i = 0; i < questData["items"].Size(); i++) { //questData["enemies"][i]["name"]
            Quest::Item* item =new Quest::Item(questData["items"][i]["name"].GetString(), questData["items"][i]["collected"].GetInt(), questData["items"][i]["required"].GetInt());
            result.items.push_back(item);
        }
        result.questStatus = questData["questStatus"].GetInt();
    }
    return result;
}

std::vector<std::pair<std::string, int>> JsonManager::getUsingItem() {
    std::vector<std::pair<std::string, int>> result;
    if (saveGameDoc.HasMember("UsingItems") && saveGameDoc["UsingItems"].IsArray()) {
        rapidjson::Value& usingItems = saveGameDoc["UsingItems"];
        if (!usingItems.Empty()) {
            for (int i = 0; i < usingItems.Size(); i++) {
                result.push_back(std::make_pair(usingItems[i]["name"].GetString(), usingItems[i]["quantity"].GetInt()));
            }
        }
    }
    return result;
}

Player::PlayerUsingEquipment JsonManager::getPlayerUsingEquipmentData() { //wp, arm. gloves, ring, shoe, helm
    Player::PlayerUsingEquipment result;
    if (saveGameDoc.HasMember("UsingEquipments") && saveGameDoc["UsingEquipments"].IsArray()) {
        rapidjson::Value& usingEquipments = saveGameDoc["UsingEquipments"];
        result.weaponNLevel = std::make_tuple(usingEquipments[0]["name"].GetString(), usingEquipments[0]["level"].GetInt(), usingEquipments[0]["type"].GetString());
        result.armorNLevel = std::make_tuple(usingEquipments[1]["name"].GetString(), usingEquipments[1]["level"].GetInt(), usingEquipments[1]["type"].GetString());
        result.shieldNLevel = std::make_tuple(usingEquipments[2]["name"].GetString(), usingEquipments[2]["level"].GetInt(), usingEquipments[2]["type"].GetString());
        result.ringNLevel = std::make_tuple(usingEquipments[3]["name"].GetString(), usingEquipments[3]["level"].GetInt(), usingEquipments[3]["type"].GetString());
        result.shoeNLevel = std::make_tuple(usingEquipments[4]["name"].GetString(), usingEquipments[4]["level"].GetInt(), usingEquipments[4]["type"].GetString());
        result.helmetNLevel = std::make_tuple(usingEquipments[5]["name"].GetString(), usingEquipments[5]["level"].GetInt(), usingEquipments[5]["type"].GetString());
    }
    return result;
}


std::vector<BaseEquipment::EquipmentData> JsonManager::getEquipmentsData() {
    std::vector<BaseEquipment::EquipmentData> result;
    if (saveGameDoc.HasMember("ActiveEquipments") && saveGameDoc["ActiveEquipments"].IsArray()) {
        rapidjson::Value& equipments = saveGameDoc["ActiveEquipments"];
        for (int i = 0; i < equipments.Size(); i++) {
            BaseEquipment::EquipmentData equipment;
            equipment.name = equipments[i]["name"].GetString();
            equipment.level = equipments[i]["level"].GetInt();
            result.push_back(equipment);
        }
    }
    return result;
}

std::vector<SkillBase*> JsonManager::getSkillsData() {
    std::vector<SkillBase*> result;
    if (saveGameDoc.HasMember("Skills") && saveGameDoc["Skills"].IsArray()) {
        rapidjson::Value& skills = saveGameDoc["Skills"];
        for (int i = 0; i < skills.Size(); i++) {
            auto skill = SkillFactory::createSkill(skills[i]["name"].GetString());
            while (skill->getCurrentLevelSkill() < skills[i]["level"].GetInt()) skill->levelUpSkill();
            skill->setSkillIndex(skills[i]["index"].GetInt());
            result.push_back(skill);
        }
    }
    return result;
}

std::vector<std::string> JsonManager::getItemsData() {
    std::vector<std::string> result;
    if (saveGameDoc.HasMember("ActiveItems") && saveGameDoc["ActiveItems"].IsArray()) {
        rapidjson::Value& items = saveGameDoc["ActiveItems"];
        for (int i = 0; i < items.Size(); i++) {
            result.push_back(items[i]["name"].GetString());
        }
    }
    return result;
}

std::string JsonManager::loadJsonFile(const std::string& filename)
{
    if (FileUtils::getInstance()->isFileExist(path))
    {
        return FileUtils::getInstance()->getStringFromFile(path);
    }
    else
    {
        CCLOG("JsonManager::loadJsonFile - File not found: %s", filename.c_str());
        return "";
    }

}

bool JsonManager::saveJsonFile(const std::string& filename, const std::string& content)
{
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        CCLOG("Error creating file: %s", filename.c_str());
        return false;
    }

    fputs(content.c_str(), file);
    fclose(file);
    return true;

}

JsonManager::~JsonManager()
{
}

void JsonManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}