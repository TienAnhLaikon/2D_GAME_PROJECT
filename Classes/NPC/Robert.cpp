﻿#include "Robert.h"

bool Robert::init()
{
	this->loadAnimate();
	this->idleState = new NPCIdle(this);
	this->talkingState = new NPCTalking(this);

	direction = 12;
	detectionRadius = 100;
	NPCCurrentSprite = Sprite::createWithSpriteFrameName("warrior_armed_idle_E_0.0_0.png");
	dialogue = {
		"HAHAHAHAHAHAHA...",
		"HIHIHIHIHIHIHIHHIIHHI...",
		"My name is Robert",
		"Do you need some weapons ?"};
	auto physicsBody = PhysicsBody::createBox(Size(20, 20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(false);
	this->setPhysicsBody(physicsBody);
	this->addChild(NPCCurrentSprite);

	//Name Label
	nameLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 7);
	nameLabel->setString("Robert");
	nameLabel->setPosition(Vec2(0, 25));
	nameLabel->retain();
	this->addChild(nameLabel);

	//Interact Sprite
	interactSprite = Sprite::create("res/talk_box.png");
	interactSprite->setScale(0.15 * Director::getInstance()->getContentScaleFactor());
	interactSprite->setName("InteractSprite");
	interactSprite->retain();

	//Talk
	talkSprite = Sprite::create("res/button_orange.png");
	talkSprite->setScale(0.25 * Director::getInstance()->getContentScaleFactor());

	talkLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 30);
	talkLabel->setString("Talk");
	talkLabel->setPosition(Vec2(talkSprite->getContentSize().width / 2, talkSprite->getContentSize().height / 2));
	talkLabel->setColor(Color3B::WHITE);
	talkSprite->addChild(talkLabel);
	talkSprite->retain();

	auto touchListenerToInteract = EventListenerTouchOneByOne::create();
	touchListenerToInteract->setSwallowTouches(true);
	touchListenerToInteract->onTouchBegan = CC_CALLBACK_2(Robert::onTouchBeganToInteract, this);
	interactSprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListenerToInteract, interactSprite);

	auto touchListenerToTalk = EventListenerTouchOneByOne::create();
	touchListenerToTalk->setSwallowTouches(true);
	touchListenerToTalk->onTouchBegan = CC_CALLBACK_2(Robert::onTouchBeganToTalk, this);
	talkSprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListenerToTalk, talkSprite);

	auto touchListenerToShop = EventListenerTouchOneByOne::create();
	touchListenerToShop->setSwallowTouches(true);
	touchListenerToShop->onTouchBegan = CC_CALLBACK_2(Robert::onTouchBeganToShop, this);

	//
	schedule(CC_SCHEDULE_SELECTOR(NPC::updateTarget), 0.1f);

	return true;
}
//
bool Robert::onTouchBeganToTalk(Touch* touch, Event* event) {

	return false;
}

bool Robert::onTouchBeganToInteract(Touch* touch, Event* event)
{
	if (interactSprite->isVisible() && UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) >= 10) {
		Vec2 touchLocationInNode = target->convertToNodeSpace(touch->getLocation());
		if (interactSprite->getBoundingBox().containsPoint(touchLocationInNode))
		{
			UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
			DialogueManager::getInstance()->showDialogue();
			return true;
		}
		return false;
	}
	return false;
}

bool Robert::onTouchBeganToShop(Touch* touch, Event* event) {

	return false;
}


void Robert::loadAnimate()
{
	NPC_Idle_E = Animate::create(Engine::getInstance()->createAnimation1("warrior_armed_idle_E_0.0_", 1, 0.05));
	NPC_Idle_N = Animate::create(Engine::getInstance()->createAnimation1("warrior_armed_idle_N_90.0_", 1, 0.05));
	NPC_Idle_NE = Animate::create(Engine::getInstance()->createAnimation1("warrior_armed_idle_NE_45.0_", 1, 0.05));
	NPC_Idle_NW = Animate::create(Engine::getInstance()->createAnimation1("warrior_armed_idle_NW_135.0_", 1, 0.05));
	NPC_Idle_S = Animate::create(Engine::getInstance()->createAnimation1("warrior_armed_idle_S_270.0_", 1, 0.05));
	NPC_Idle_SE = Animate::create(Engine::getInstance()->createAnimation1("warrior_armed_idle_SE_315.0_", 1, 0.05));
	NPC_Idle_SW = Animate::create(Engine::getInstance()->createAnimation1("warrior_armed_idle_SW_225.0_", 1, 0.05));
	NPC_Idle_W = Animate::create(Engine::getInstance()->createAnimation1("warrior_armed_idle_W_180.0_", 1, 0.05));

	NPC_Select_E = Animate::create(Engine::getInstance()->createAnimation1("warrior_special_select_E_0.0_", 7, 0.05));
	NPC_Select_N = Animate::create(Engine::getInstance()->createAnimation1("warrior_special_select_N_90.0_", 7, 0.05));
	NPC_Select_NE = Animate::create(Engine::getInstance()->createAnimation1("warrior_special_select_NE_45.0_", 7, 0.05));
	NPC_Select_NW = Animate::create(Engine::getInstance()->createAnimation1("warrior_special_select_NW_135.0_", 7, 0.05));
	NPC_Select_S = Animate::create(Engine::getInstance()->createAnimation1("warrior_special_select_S_270.0_", 7, 0.05));
	NPC_Select_SE = Animate::create(Engine::getInstance()->createAnimation1("warrior_special_select_SE_315.0_", 7, 0.05));
	NPC_Select_SW = Animate::create(Engine::getInstance()->createAnimation1("warrior_special_select_SW_225.0_", 7, 0.05));
	NPC_Select_W = Animate::create(Engine::getInstance()->createAnimation1("warrior_special_select_W_180.0_", 7, 0.05));

	// Animation
	NPC_Idle_E->retain();
	NPC_Idle_N->retain();
	NPC_Idle_NE->retain();
	NPC_Idle_NW->retain();
	NPC_Idle_S->retain();
	NPC_Idle_SE->retain();
	NPC_Idle_SW->retain();
	NPC_Idle_W->retain();

	NPC_Select_E->retain();
	NPC_Select_N->retain();
	NPC_Select_NE->retain();
	NPC_Select_NW->retain();
	NPC_Select_S->retain();
	NPC_Select_SE->retain();
	NPC_Select_SW->retain();
	NPC_Select_W->retain();
}
