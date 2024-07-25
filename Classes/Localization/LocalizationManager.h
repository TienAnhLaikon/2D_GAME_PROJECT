#ifndef __LOCALIZATION_MANAGER__
#define __LOCALIZATION_MANAGER__

#include "cocos2d.h"

USING_NS_CC;
class LocalizationManager : public Node
{
public:
    static LocalizationManager* getInstance()
    {
        if (!s_instance)
        {
            s_instance = new LocalizationManager();
            s_instance->init();
        }
        return s_instance;
    }
    void destroyInstance()
    {
        if (s_instance)
        {
            delete s_instance;
            s_instance = nullptr;
        }
    }
private:
    LocalizationManager() {};
    static LocalizationManager* s_instance;
};
#endif // __SETTINGS_MANAGER_H__
