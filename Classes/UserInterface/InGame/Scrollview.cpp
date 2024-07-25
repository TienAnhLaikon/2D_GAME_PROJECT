#include "Scrollview.h"

bool Scrollview::init() {
    if (!Layer::init()) {
        return false;
    }

    scrollView = ui::ScrollView::create();
    scrollView->setBounceEnabled(true);
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(Size(200, 400));
    scrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    scrollView->setPosition(Vec2::ZERO);
    scrollView->setScrollBarAutoHideTime(5.0f);

    addChild(scrollView);

 /*   scrollbar = Sprite::create("res/scroll_handler.png");
    scrollbar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(scrollbar);

    scrollView->addEventListener([=](Ref*, ui::ScrollView::EventType) {
        updateScrollbarPosition();
        });*/

   /* auto customScrollbar = Sprite::create("res/scroll_handler.png");
    customScrollbar->setPosition(Vec2(scrollView->getContentSize().width / 2 - 30, 50));
    this->addChild(customScrollbar);
    scrollView->setScrollBarEnabled(false);

    scrollView->addEventListener([=](Ref*, ui::ScrollView::EventType) {
        float contentHeight = scrollView->getInnerContainerSize().height;
        float scrollViewHeight = scrollView->getContentSize().height;
        float scrollbarHeight = customScrollbar->getContentSize().height;
        float percent = scrollView->getInnerContainerPosition().y / (contentHeight - scrollViewHeight);
        float scrollbarPosY = (scrollViewHeight - scrollbarHeight) * percent;
        customScrollbar->setPositionY(scrollView->getPositionY() + scrollbarPosY);
        });*/


    return true;
}

void Scrollview::updateScrollbarPosition() {
    float percent = scrollView->getInnerContainerPosition().y / (scrollView->getInnerContainerSize().height - scrollView->getContentSize().height);
    float scrollbarPosY = scrollView->getPositionY() - (scrollView->getContentSize().height * 0.5f) + (scrollView->getContentSize().height - scrollbar->getContentSize().height) * percent;
    scrollbar->setPosition(Vec2(scrollView->getPositionX() + scrollView->getContentSize().width * 0.5f + scrollbar->getContentSize().width * 0.5f, scrollbarPosY));
}

void Scrollview::setScrollViewContentSize(const Size& size) {
    scrollView->setContentSize(size);
}

void Scrollview::addToScrollView(Node* node) {
    scrollView->addChild(node);
}

void Scrollview::setScrollViewPosition(const Vec2& position) {
    scrollView->setPosition(position);
}

void Scrollview::setScrollViewVisible(bool visible) {
    scrollView->setVisible(visible);
}

void Scrollview::setScrollViewDirection(ui::ScrollView::Direction direction) {
    scrollView->setDirection(direction);
}

ui::ScrollView* Scrollview::getScrollView() const {
    return scrollView;
}
