#ifndef __ANIMATIC_SCENE__
#define __ANIMATIC_SCENE__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
struct AnimaticData {
    std::string imagePath;
    std::string soundPath;
    std::string engsub;
    float soundTime;
    AnimaticData(std::string path, std::string sPath, std::string sub, float time) : imagePath(path), soundPath(sPath), engsub(sub), soundTime(time) {};
};
class AnimaticScene : public cocos2d::Layer
{
public:
    virtual bool init();
    Sprite* _background;
    Sprite* _backgroundForward;
    void hide();
    void show();
    bool isShow();
    bool onTouchBegan(Touch* touch, Event* event);
    std::vector<std::string> split(const std::string& str, char delimiter);
    void addStoryData(AnimaticData animaticData);
    void nextStory();
    void resetSubs();
    void showStringWordByWord(const std::string& text, float delay, cocos2d::Node* parentNode, const cocos2d::Vec2& position);
    CREATE_FUNC(AnimaticScene);
private:
    std::vector<AnimaticData> story;
    std::vector<Label*> subsLabels;
    int currentStoryIndex = -1;
    Label* nextLabel;
    Label* skipLabel;
    bool _isShow;
    Sprite* _closeButton;
    Node* map0;
    Node* map1;
    Node* map2;
    Node* map3;
};

#endif // __ANIMATIC_SCENE__