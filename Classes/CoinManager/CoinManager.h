#ifndef __COIN_MANAGER_H__
#define __COIN_MANAGER_H__
#include "cocos2d.h"
#include "UserInterface/InGame/TalentCarousel.h"
USING_NS_CC;

class CoinManager
{
public:
	static CoinManager *getInstance()
	{
		if (!instance)
		{
			instance = new CoinManager();
		}
		return instance;
	}

	void destroyInstance() {
		if (instance != nullptr) {
			_coins = 0;
			delete instance;
			instance = nullptr;
		}
	}

	void increaseCoin(int amount) { 
		this->_coins += amount;
	}

	void decreaseCoin(int amount) {
		if (this->_coins < amount) {
			this->_coins -= amount;
		}
		else {
			CCLOG("Coin Amount not valid!");
			return;
		}
	}

	int getCoins() const { 
		return _coins; 
	}

	void saveCoin() {
		UserDefault::getInstance()->setIntegerForKey("coin", _coins);
	}
private:
	CoinManager() : _coins(UserDefault::getInstance()->getIntegerForKey("coin", 0)){};
	~CoinManager(){};
	static CoinManager *instance;
	int _coins;
};

#endif // __ENGINE_H__