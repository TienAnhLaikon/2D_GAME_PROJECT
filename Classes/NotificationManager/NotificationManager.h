#ifndef __NOTIFICATION_MANAGER_H__
#define __NOTIFICATION_MANAGER_H__
#include "cocos2d.h"
USING_NS_CC;

class NotificationManager
{
public:
	static NotificationManager* getInstance()
	{
		if (!instance)
		{
			instance = new NotificationManager();
		}
		return instance;
	}

	void destroyInstance() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}
	void showMessageNotification(const std::string& message, Vec2 positionApply, Color3B colorApply, int fontSize);
	void showSkillEffectivePeriod(const std::string& spritePath, float time);
	void hideMessageNotification();
private:
	NotificationManager();
	virtual ~NotificationManager();
	static NotificationManager* instance;
};

#endif // __ENGINE_H__