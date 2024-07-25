#ifndef __SCROLL_VIEW__
#define __SCROLL_VIEW__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class Scrollview : public cocos2d::Layer
{
public:
    virtual bool init();
    void setScrollViewContentSize(const Size& size);
    void addToScrollView(Node* node);
    void setScrollViewPosition(const Vec2& position);
    void setScrollViewVisible(bool visible);
    void setScrollViewDirection(ui::ScrollView::Direction direction);
    ui::ScrollView* getScrollView() const;
    void updateScrollbarPosition();
    CREATE_FUNC(Scrollview);
private:
    ui::ScrollView* scrollView;
    Sprite* scrollbar;
};

#endif // __SCROLL_VIEW__