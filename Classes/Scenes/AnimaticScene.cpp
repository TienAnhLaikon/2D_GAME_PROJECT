#include "AnimaticScene.h"
#include "Game/Game.h"
bool AnimaticScene::init()
{
	if (!Layer::create())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_background = Sprite::create("sound/animatic/1/1.png");
	this->addChild(_background, 2);
	_backgroundForward = Sprite::create("sound/animatic/1/1.png");
	_backgroundForward->setScale(10);
	this->addChild(_backgroundForward, 1);

	nextLabel = Label::createWithTTF("Next", "fonts/Diablo Light.ttf", 9);
	nextLabel->setScale(1.5 * Director::getInstance()->getContentScaleFactor());
	nextLabel->setTextColor(Color4B::WHITE);
	nextLabel->setPosition(_background->getContentSize().width / 2 - nextLabel->getContentSize().width * 3 + 22 * Director::getInstance()->getContentScaleFactor(), -_background->getContentSize().height / 2 + nextLabel->getContentSize().height - 50);
	this->addChild(nextLabel, 3);

	skipLabel = Label::createWithTTF("Skip", "fonts/Diablo Light.ttf", 9);
	skipLabel->setScale(1.5* Director::getInstance()->getContentScaleFactor());
	skipLabel->setTextColor(Color4B::WHITE);
	skipLabel->setPosition(nextLabel->getPosition() + Vec2(nextLabel->getContentSize().width * 0.5 + 50 * Director::getInstance()->getContentScaleFactor(), 0));
	this->addChild(skipLabel, 3);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(AnimaticScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	std::string story1 = "When the world has not yet\n been shaped. \nWhen the world was nothing\n but endless black";
	std::string story2 = "The miracle of creation\n make a disperity. And we \nhumans call them the Outer Gods.";
	std::string story3 = "Okyanus, the mother of the sea";
	std::string story4 = "Terra, the father of the earth";
	std::string story5 = "Paradiso, lord of the sky";
	std::string story6 = "And from there the age of\n humanity began But sooner\n or later the sins of the Outer Gods\n will strike humanity with the\n Lunar Eclipse plagues.";
	std::string story7 = "The appearance of the Outer Gods\n has caused those who receive \nthe power of the Outer Gods to be\n transformed into bloodthirsty \nBeasts every time a Lunar Eclipse occurs. \nInnocent people are turned into beasts\n during four lunar eclipses a year.";
	std::string story8 = "And from there, the beast slayers\n were born with the sole purpose\n of killing the Outer Gods who\n were bringing chaos to the world.";
	story.push_back(AnimaticData("sound/animatic/1/1.png", "sound/animatic/1/1.mp3", story1, 8.0f));
	story.push_back(AnimaticData("sound/animatic/2/2.png", "sound/animatic/2/2.mp3", story2, 8.5f));
	story.push_back(AnimaticData("sound/animatic/3/3.png", "sound/animatic/3/3.mp3", story3, 4.0f));
	story.push_back(AnimaticData("sound/animatic/4/4.png", "sound/animatic/4/4.mp3", story4, 2.9f));
	story.push_back(AnimaticData("sound/animatic/5/5.png", "sound/animatic/5/5.mp3", story5, 2.9f));
	story.push_back(AnimaticData("sound/animatic/6/6.png", "sound/animatic/6/6.mp3", story6, 13.0f));
	story.push_back(AnimaticData("sound/animatic/7/7.png", "sound/animatic/7/7.mp3", story7, 21.0f));
	story.push_back(AnimaticData("sound/animatic/8/8.png", "sound/animatic/8/8.mp3", story8, 12.0f));

	nextStory();
	nextStory();
	// hide();
	return true;
}

bool AnimaticScene::onTouchBegan(Touch *touch, Event *event)
{
	if (isShow())
	{
		auto startPoint = this->convertToNodeSpace(touch->getLocation());
		if (skipLabel->getBoundingBox().containsPoint(startPoint))
		{
			if (UserDefault::getInstance()->getIntegerForKey("StoryVoice", -1) != -1)
				Audio::getInstance()->stop(UserDefault::getInstance()->getIntegerForKey("StoryVoice"));
			hide();
			_background->stopAllActions();
		}
		if (nextLabel->getBoundingBox().containsPoint(startPoint))
		{
			nextStory();
		}
		return true;
	}
	return false;
}

void AnimaticScene::addStoryData(AnimaticData animaticData)
{
	/*story.push_back(animaticData);
	for (auto i : story) {
		showStringWordByWord(i.engsub);

	}*/
}

std::vector<std::string> AnimaticScene::split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}
void AnimaticScene::resetSubs()
{
	for (auto i : subsLabels)
	{
		i->removeFromParentAndCleanup(true);
	}
	subsLabels.clear();
}

void AnimaticScene::nextStory()
{
	_background->stopAllActions();
	_background->setPosition(Vec2(0, 0));
	if (UserDefault::getInstance()->getIntegerForKey("StoryVoice", -1) != -1)
		Audio::getInstance()->stop(UserDefault::getInstance()->getIntegerForKey("StoryVoice"));
	if (currentStoryIndex > -1)
	{
		resetSubs();
		if (currentStoryIndex >= story.size())
		{
			hide();
			return;
		}
		for (int i = 0; i < story.size(); i++)
		{
			if (i == currentStoryIndex)
			{
				auto changeImage = cocos2d::CallFunc::create([this, i]()
															 { _background->setTexture(story[i].imagePath); });
				auto loadSub = cocos2d::CallFunc::create([this, i]()
														 {
					showStringWordByWord(story[i].engsub, 0.65f, this, Vec2(0, -_background->getContentSize().height * 0.28));
						UserDefault::getInstance()->setIntegerForKey("StoryVoice", Audio::getInstance()->play2d(story[i].soundPath)); });
				auto nextCallback = cocos2d::CallFunc::create([this, i]()
															  { nextStory(); });
				auto textSequence = cocos2d::Sequence::create(DelayTime::create(i == 0 ? 2.5 : 0), FadeOut::create(0.5), changeImage, loadSub, FadeIn::create(0.5), DelayTime::create(story[i].soundTime), nextCallback, nullptr);
				_background->runAction(textSequence);
			}
		}
	}
	currentStoryIndex++;
}

void AnimaticScene::showStringWordByWord(const std::string &text, float delay, cocos2d::Node *parentNode, const cocos2d::Vec2 &position)
{
	auto label = cocos2d::Label::createWithTTF("", "fonts/Diablo Light.ttf", 10);
	label->setTextColor(Color4B::WHITE);
	label->setPosition(position);
	subsLabels.push_back(label);
	parentNode->addChild(label, 5);
	label->runAction(FadeIn::create(1.2f));
	std::vector<std::string> words = AnimaticScene::split(text, ' ');
	float index = 0;
	int count = 0;
	for (const std::string &word : words)
	{
		auto delayAction = cocos2d::DelayTime::create(delay * index);
		auto showNextWord = cocos2d::CallFunc::create([label, word]()
													  {
			if (label->getString().empty())
			{
				label->setString(word);
			}
			else
			{
				label->setString(label->getString() + " " + word);
			} 
			
			});

		auto textSequence = cocos2d::Sequence::create(delayAction, showNextWord, nullptr);
		label->runAction(textSequence);
		auto scaleAction = cocos2d::ScaleTo::create(0.8, 0.8);
		auto moveAction = cocos2d::MoveTo::create(8.5f, _background->getPosition() + Vec2(30, 30));
		auto spriteSequence = cocos2d::Sequence::create(delayAction, scaleAction, delayAction, nullptr);
		_background->runAction(spriteSequence);
		index += 0.68;
		count++;
	}
}

void AnimaticScene::hide()
{
	this->setVisible(false);
}
void AnimaticScene::show()
{
	this->setVisible(true);
}

bool AnimaticScene::isShow()
{
	return this->isVisible();
}