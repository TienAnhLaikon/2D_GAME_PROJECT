#ifndef __TALENT_CAROUSEL__
#define __TALENT_CAROUSEL__

#include "cocos2d.h"
USING_NS_CC;
#include "Skills/SkillTree.h"
class Player;
class TalentProgressBar;
class CoinManager;
class TalentCarousel : public cocos2d::Node
{
public:
    static TalentCarousel* create(Player* player);
    virtual bool init(Player* player);
    bool onTouchBegan(Touch* touch, Event* event);
    bool onTouchMoved(Touch* touch, Event* event);
    bool onTouchEnded(Touch* touch, Event* event);
    void showTalentCarousel();
    //bool yesOrNo(CoinManager* cointManager);
    //void onYesClicked();
    //void onNoClicked();
    //void onCloseClicked();
    //bool onTouchBeganToResetSkills(Touch* touch, Event* event);
    Player* _player = nullptr;
    SkillTree* skillTreeAoe = nullptr;
    SkillTree* skillTreeBuff = nullptr;
    TalentProgressBar* progressBar;
    void hideComponents();
    void showComponents();
    void resetSkill(Player* player);
    void resetUI();
    std::vector<SkillTree*> listOfTrees;
    EventListenerTouchOneByOne* touchListener = nullptr;
    Sprite* talentBorder = nullptr;
private: 
    //Label* goldLabel;
    //Sprite* gold;
    Sprite* prevButton;
    Sprite* nextButton;
    Sprite* buttonClose;
    //Sprite* resetSkillButton;
    bool lastChoosedFirstTab;
    Sprite* fireTreeTabButton;
    Sprite* thunderTreeTabButton;
    int currentTree = 0;
    std::promise<bool> promise;
};

#endif // __JOYSTICK_H__