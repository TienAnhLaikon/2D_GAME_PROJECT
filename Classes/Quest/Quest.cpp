#include "Quest.h"
#include "NPC/NPC.h"
Quest* Quest::create(const std::string questName, const std::string description, std::vector<Item*>items,
	std::vector<Enemy*> enemies, const int questType, const QuestRewards rewards, NPC* npc) {

	Quest* object = new (std::nothrow)Quest();
	if (object && object->init(questName, description, items, enemies, questType, rewards, npc)) {
		object->autorelease();
		return object;
	}
	CC_SAFE_DELETE(object);
	return nullptr;

}
bool Quest::init(const std::string questName, const std::string description, std::vector<Item*>items,
	std::vector<Enemy*> enemies, const int questType, const QuestRewards rewards, NPC* npc)
{
	//Init Props

	this->questName = questName;
	this->questDescription = questDescription;
	this->items = items;
	this->enemies = enemies;
	this->reward = rewards;
	this->npc = npc;
	this->questType = questType;
	this->questStatus = NOT_RECEIVED;
	this->npc->retain();
	objectiveLabel = Label::createWithTTF("OBJECTIVE", "fonts/Diablo Heavy.ttf", 20);
	objectiveLabel->setColor(Color3B::WHITE); // M RGB cho mu GOLD
	objectiveLabel->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 75));
	this->addChild(objectiveLabel);
	///////////////// MAIN QUEST INTERFACE/////////////
	//Init Name Label
	nameLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 10);
	nameLabel->setColor(Color3B(255, 215, 0));
	nameLabel->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height + 15));
	nameLabel->setString(StringUtils::format("%s", questName.c_str()));
	this->addChild(nameLabel);

	questInformationLabel = Label::createWithTTF("Quest Information", "fonts/Diablo Light.ttf", 15);
	questInformationLabel->setColor(Color3B::WHITE);
	questInformationLabel->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 30));
	this->addChild(questInformationLabel);

	//Init Description Lable
	descriptionLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 8);
	descriptionLabel->setColor(Color3B::WHITE);
	descriptionLabel->setPosition(Vec2(0, this->getContentSize().height / 2 -20));
	descriptionLabel->setString(StringUtils::format("%s", splitLines(description).c_str()));
	this->addChild(descriptionLabel);

	// I. Init Progress Lable List
	//1. For every single monster we need every label for this quest (E.g: 3 monster need 3 progress label)
	if (enemies.size() > 0) {
		int rowEnemies = 0;
		for (auto enemy : enemies) {
			Label* progress_temp = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 11);
			progress_temp->setColor(Color3B::WHITE);
			progress_temp->setName(enemy->name +" Label");
			progress_temp->setPosition(Vec2(this->getContentSize().width/2 + 50, rowEnemies - 90));
			progress_temp->setString(StringUtils::format("%s: %d / %d", enemy->name.c_str(), enemy->killed, enemy->required));
			progressLabelList.push_back(progress_temp);
			rowEnemies++;
		}
	}
	if (items.size() > 0) {
		int rowItems = 0;
		for (auto item : items) {
			Label* progress_temp = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 11);
			progress_temp->setColor(Color3B::GRAY);
			progress_temp->setName(item->name + " Label");
			progress_temp->setPosition(Vec2(this->getContentSize().width / 2 - 50, rowItems - 90));
			progress_temp->setString(StringUtils::format("%s: %d / %d", item->name.c_str(), item->collected, item->required));
			progressLabelList.push_back(progress_temp);

			rowItems++;
		}
	}
	//2. Add child labels to this quest Detail
	for (auto progressLabel : progressLabelList) {
		this->addChild(progressLabel);
	}

	
	//Note: When player kill a monster, we need to increase monsterKilled value (if monsterKilled < monsterRequired)
	// And reset string label by setString. 
	return true;
}
Quest::Quest(const std::string questName, const std::string description, std::vector<Item*>items,
	std::vector<Enemy*> enemies, const int questType, int questStatus, const QuestRewards rewards, NPC* npc) {
	this->questName = questName;
	this->questDescription = description;
	this->items = items;
	this->enemies = enemies;
	this->questType = questType;
	this->questStatus = questStatus;
	this->reward = rewards;
	this->npc = npc;
}
Quest::Quest() {

}

Quest::~Quest() {

}
