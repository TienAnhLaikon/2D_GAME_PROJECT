#ifndef __SETTINGS_DATA_H__
#define __SETTINGS_DATA_H__

#include "cocos2d.h"
#include "Audio/AudioEngine.h"
USING_NS_CC;
class SettingsData : public Node
{
public:
    static SettingsData *getInstance()
    {
        if (!s_instance)
        {
            s_instance = new SettingsData();
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
    void setMusicSlider(int newMusicSlider)  { 
        this->musicSlider = newMusicSlider; 
        UserDefault::getInstance()->setIntegerForKey("music_volume1", newMusicSlider);
    }
    int getMusicSlider() const { return this->musicSlider; }
    void setSoundSlider(int newSoundSlider) { 
        this->soundSlider = newSoundSlider; 
        UserDefault::getInstance()->setIntegerForKey("sound_volume1", newSoundSlider);
    }
    int getSoundSlider() const { return this->soundSlider; }
    void setLanguage(std::string newLanguage) { this->currentLanguage = newLanguage; }
    std::string getLanguage()const { return this->currentLanguage; }
private:
    bool init() {
        // add saved data
        return true;
    }
    SettingsData(){};
    ~SettingsData();
    int musicSlider = UserDefault::getInstance()->getIntegerForKey("music_volume1", 50);
    int soundSlider = UserDefault::getInstance()->getIntegerForKey("sound_volume1", 50);
    std::string currentLanguage;
    static SettingsData *s_instance;
};
#endif // __SETTINGS_MANAGER_H__  
