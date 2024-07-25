#ifndef __UPGRADE_PROGRESS_BAR__
#define __UPGRADE_PROGRESS_BAR__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <sstream>
#include <iomanip>
USING_NS_CC;
class UpgradeProgressBar : public cocos2d::Node {
public:
    virtual bool init() override;
    static UpgradeProgressBar* create();
    void updateProgress(float currentHealth, float maxHealth);
    float getHeight() const { return upgradeProgressBar->getContentSize().height; };
private:
    ui::LoadingBar* upgradeProgressBar;
    Label* expLabel;
    UpgradeProgressBar();
    virtual ~UpgradeProgressBar();
};

#endif // __UPGRADE_PROGRESS_BAR__