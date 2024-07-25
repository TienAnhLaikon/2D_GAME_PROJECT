#ifndef __SKILL_TREE__
#define __SKILL_TREE__

#include "cocos2d.h"
#include "SkillBase.h"
#include "SkillFactory.h"
#include "2d/CCLabel.h"
#include "UserInterface/InGame/TalentProgressBar.h"
USING_NS_CC;
class Player;
class TalentCarousel;
class SkillTree : public cocos2d::Node
{
public:
	static SkillTree* createSkillTree(Player* player, SkillBase::SkillTalent* skillRoot);
	virtual bool init(Player* player, SkillBase::SkillTalent* skillRoot);
	void drawTree(Player* player, SkillBase::SkillTalent* node, Vec2 position, float xSpacing, float ySpacing, SkillBase::SkillTalent* parent);
	void showSkillDetail(SkillBase* skill, Player* player);
    std::string splitLines(const std::string& str) {
        std::string result = "";
        std::stringstream ss(str); // To stringstream t chui u vo
        std::string word;
        int count = 0;

        while (ss >> word) {
            if (count == 6) {
                word += "\n";
                count = 0;
            }
            result += word + " ";
            count++;
        }

        return result;
    }
    void showAlert(std::string alert, bool isSuccess);
	~SkillTree();
    float treeWidth = 0;
	float treeHeight = 0;
    Sprite* skillDetail;
    EventListenerTouchOneByOne* _touchListenerSlot1;
    EventListenerTouchOneByOne* _touchListenerSlot2;
    EventListenerTouchOneByOne* _touchListenerSlot3;
    EventListenerTouchOneByOne* _touchListenerSlot4;

    EventListenerTouchOneByOne* touchListener1;
    EventListenerTouchOneByOne* touchListener2;
    EventListenerTouchOneByOne* touchListener3;
    EventListenerTouchOneByOne* touchListener4;
    EventListenerTouchOneByOne* touchListener5;
    EventListenerTouchOneByOne* touchListener6;

    EventListenerTouchOneByOne* outsideSkillDetailListener;
    EventListenerTouchOneByOne* touchListenerEquip;
    EventListenerTouchOneByOne* touchListenerCancelEquip;
    EventListenerTouchOneByOne* touchListenerAttack;
    //Get buttons and labels for TUTORIALS
    Label* moneyLabel = nullptr; // Skill Points of Player
    Label* unlockPointRequired = nullptr;
    Label* upgradePointRequired = nullptr;
    Sprite* equipButton = nullptr;
    Sprite* slot1Button = nullptr;
    Sprite* unlockButton = nullptr;
    Sprite* upgradeButton = nullptr;
    Sprite* yesButton = nullptr;
    std::vector<SkillBase::SkillTalent*> listOfSkills;
    std::vector<std::tuple<SkillBase::SkillTalent*, Node*> > skillRects;
    void setProgressBar(TalentProgressBar* progressBar) { _progressBar = progressBar; };
    void setOverlay(LayerColor* overlay) { _overlay = overlay; };
    void setParentNode(TalentCarousel* talentCarousel) { _talentCarousel = talentCarousel; };
    void showUI();
    TalentProgressBar* _progressBar = nullptr;
    void hideUI();
    std::string treeName = "";
private:
    void hideComponents();
    void showComponents();
    std::vector<Sprite*> listSkillDetails;
    std::vector<Sprite*> listSlots;
    std::vector<Sprite*> treeDrawed;
    std::vector<DrawNode*> treeDrawedNode;
    std::vector<Label*> hideLabels;
    TalentCarousel* _talentCarousel = nullptr;

    LayerColor* _overlay;
	Label* skillDescriptionLable;
	Size visibleSize;
	int childrenQuan = 0;
    int clickButtonSoundId;
    Player* _player = nullptr;

};

#endif // __SKILL_TREE__
