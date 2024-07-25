#ifndef __EXP_BAR__
#define __EXP_BAR__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <sstream>
#include <iomanip>
USING_NS_CC;
class ExpBar : public cocos2d::Node {
public:
    virtual bool init() override;
    static ExpBar* create();

    // C?p nh?t thanh mu d?a trn mu hi?n t?i v mu t?i ?a
    void updateExpBar(float currentHealth, float maxHealth);
    /*void setLevel(int newLevel) {
        levelLabel->setString(StringUtils::format("level %d", newLevel));
    }*/
    /*float getHeight() const { return expBar->getContentSize().height; };
    Label* levelLabel;*/
    ui::LoadingBar* expBar;
    Label* expLabel;
private:
    // Thnh ph?n c?a thanh mu

    // Constructor v Destructor
    ExpBar();
    virtual ~ExpBar();
};

#endif // __EXP_BAR__