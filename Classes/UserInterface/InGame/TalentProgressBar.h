#ifndef __TALENT_PROGRESS_BAR__
#define __TALENT_PROGRESS_BAR__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <sstream>
#include <iomanip>
USING_NS_CC;
class TalentProgressBar : public cocos2d::Node {
public:
    virtual bool init() override;
    static TalentProgressBar* create();
    void updateProgress(float currentHealth, float maxHealth);
    //float getHeight() const { return talentProgressBar->getContentSize().height; };
private:
    Sprite* currentProgressSprite;
    ui::LoadingBar* talentProgressBar;
    Label* expLabel;
    Label* maxLabel;
    Label* curLabel;
    float scale;
    TalentProgressBar();
    virtual ~TalentProgressBar();
};

#endif // __TALENT_PROGRESS_BAR__