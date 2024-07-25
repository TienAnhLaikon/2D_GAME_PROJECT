 #include "SettingsManagerInPauseMenu.h"
#include "ui/UISlider.h"
#include "ui/CocosGUI.h"
#include "SettingsData.h"
#include "Audio/AudioEngine.h"
#include "Game/Game.h"
bool SettingsManagerInPauseMenu::init()
{
    // Ly ra scene hin ti ang chy
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // Nu cha tn ti SettingsManagerInPauseMenu trong scene, th thm mi
    menu = Sprite::create("res/quest_description_container_bg_effect.png");
    menu->setScale(0.3 * Director::getInstance()->getContentScaleFactor());
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    //menu->setScaleX(0.8);
    //menu->setScaleY(0.5);
    //this->addChild(menu);
    this->setVisible(false);

    //close_menu = Sprite::create("res/c_header_close.png");
    //close_menu->setPosition(Vec2(menu->getContentSize().width - 60,
    //menu->getContentSize().height -65));
    //close_menu->setScale(2);
    //menu->addChild(close_menu);


    // To button
    auto closeButton = ui::Button::create("res/cc_char_options_arrow_left.png", "res/cc_char_options_arrow_left-HOVER.png");
    closeButton->setScale(1.5 * Director::getInstance()->getContentScaleFactor());
    // t v tr cho button
    closeButton->setPosition(Vec2(visibleSize.width- 50, visibleSize.height-50));

    // Thm s kin cho button
    closeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            // X l khi bt u chm vo button
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                    hideMenuList();

            break;
        case ui::Widget::TouchEventType::MOVED:
            // X l khi di chuyn trn button
            break;
        case ui::Widget::TouchEventType::ENDED:
            // X l khi kt thc chm vo button
            break;
        case ui::Widget::TouchEventType::CANCELED:
            // X l khi hy b chm vo button
            break;
        default:
            break;
        }
        });

            
    // Thm button vo scene
    this->addChild(closeButton);

    settingsTitle = Label::createWithTTF("SETTINGS", "fonts/Diablo Light.ttf", 60);
    settingsTitle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
    settingsTitle->enableBold();
    this->addChild(settingsTitle);

    // gearPinion Icon
    auto gearPinionLeft = Sprite::create("res/header_icon_settings.png");
    auto gearPinionRight = Sprite::create("res/header_icon_settings.png");
    gearPinionLeft->setPosition(Vec2(visibleSize.width / 2 - 300, visibleSize.height - 50));
    gearPinionRight->setPosition(Vec2(visibleSize.width / 2 + 300, visibleSize.height - 50));
    this->addChild(gearPinionLeft);
    this->addChild(gearPinionRight);




    auto header1 = Sprite::create("res/header_BROWN.png");
    header1->setPosition(Vec2(header1->getContentSize().width / 2 + 10 * Director::getInstance()->getContentScaleFactor(), visibleSize.height * (0.3 * Director::getInstance()->getContentScaleFactor()) + 120));
    this->addChild(header1);

    auto bigSoundTitle = Label::createWithTTF("SOUND", "fonts/Diablo Light.ttf", 55);
    bigSoundTitle->setPosition(header1->getContentSize().width / 2 + 20 * Director::getInstance()->getContentScaleFactor(), visibleSize.height * (0.3 * Director::getInstance()->getContentScaleFactor()) + 120);
    this->addChild(bigSoundTitle);

    auto musicIcon = Sprite::create("res/music_icon.png");
    musicIcon->setPosition(header1->getContentSize().width / 2 - bigSoundTitle->getContentSize().width / 2 - (10 * Director::getInstance()->getContentScaleFactor()), visibleSize.height * (0.3 * Director::getInstance()->getContentScaleFactor()) + 120);
    this->addChild(musicIcon);

            
    // settings Music
    musicTitle = Label::createWithTTF("Music", "fonts/Diablo Light.ttf", 35);
    musicTitle->setPosition(header1->getPosition() - Vec2(musicTitle->getContentSize().width / 2 - 50, musicTitle->getContentSize().height * 2));
    this->addChild(musicTitle);


    // Music Slider
    stoperLeft = Sprite::create("res/scale_bar_stoper.png");
    stoperRight = Sprite::create("res/scale_bar_stoper.png");
    stoperRight->setScale(4 * Director::getInstance()->getContentScaleFactor());
    stoperLeft->setScale(4 * Director::getInstance()->getContentScaleFactor());

    auto sprite = Sprite::create("res/scale_bar.png");
    sprite->setScaleX(visibleSize.width * (0.25 * Director::getInstance()->getContentScaleFactor()) / sprite->getContentSize().width * 1.3);
    sprite->setPosition(musicTitle->getPosition() + Vec2((sprite->getContentSize().width * Director::getInstance()->getContentScaleFactor() / 2) + musicTitle->getContentSize().width, 0));
    //sprite->setScaleY(1.5);

    musicSlider = ui::Slider::create();
    musicSlider->setPercent(SettingsData::getInstance()->getMusicSlider());
    
    musicSlider->loadBarTexture("res/scale_bar_cut.png");
    musicSlider->loadProgressBarTexture("res/scale_bar_fill_red.png");
    musicSlider->loadSlidBallTextureNormal("res/scale_bar_handler.png");
    musicSlider->loadSlidBallTexturePressed("res/scale_bar_handler-HOVER.png");
    musicSlider->setScale(visibleSize.width * (0.25 * Director::getInstance()->getContentScaleFactor()) / sprite->getContentSize().width * 1.3);
    musicSlider->setPosition(musicTitle->getPosition() + Vec2((sprite->getContentSize().width * Director::getInstance()->getContentScaleFactor() / 2) + musicTitle->getContentSize().width, 0));
    Size sliderSize = musicSlider->getContentSize();
    Size stoperSize = stoperLeft->getContentSize();
    float scaleRatio = sliderSize.width / 100;

    stoperLeft->setPosition(Vec2(0, sliderSize.height / 2));
    stoperRight->setPosition(Vec2(sliderSize.width, sliderSize.height / 2));
    musicSlider->addChild(stoperLeft, 10);
    musicSlider->addChild(stoperRight, 10);
    stoperLeft->setVisible(false);
    stoperRight->setVisible(false);

    this->addChild(musicSlider,5);
    this->addChild(sprite, 4);

    musicSlider->addEventListener([=](cocos2d::Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            int percent = musicSlider->getPercent();
            SettingsData::getInstance()->setMusicSlider(percent);
            Audio::getInstance()->setVolume(UserDefault::getInstance()->getIntegerForKey("background_music"),percent/100.0f);
            
        }
        });


    // settings Sound
    soundTitle = Label::createWithTTF("Sound", "fonts/Diablo Light.ttf", 35);
    soundTitle->setPosition(musicTitle->getPosition() - Vec2(0, soundTitle->getContentSize().height * 2));
    this->addChild(soundTitle);

    // sound Slider
    auto stoperLeft2 = Sprite::create("res/scale_bar_stoper.png");
    auto stoperRight2 = Sprite::create("res/scale_bar_stoper.png");
    stoperRight2->setScale(4 * Director::getInstance()->getContentScaleFactor());
    stoperLeft2->setScale(4 * Director::getInstance()->getContentScaleFactor());

    auto sprite2 = Sprite::create("res/scale_bar.png");
    sprite2->setScaleX(visibleSize.width* (0.25 * Director::getInstance()->getContentScaleFactor()) / sprite2->getContentSize().width * 1.3);
    sprite2->setPosition(soundTitle->getPosition() + Vec2((sprite2->getContentSize().width * Director::getInstance()->getContentScaleFactor() / 2) + musicTitle->getContentSize().width, 0));

    soundSlider = ui::Slider::create();
    soundSlider->setPercent(SettingsData::getInstance()->getSoundSlider());

    soundSlider->loadBarTexture("res/scale_bar_cut.png");
    soundSlider->loadProgressBarTexture("res/scale_bar_fill_red.png");
    soundSlider->loadSlidBallTextureNormal("res/scale_bar_handler.png");
    soundSlider->loadSlidBallTexturePressed("res/scale_bar_handler-HOVER.png");
    soundSlider->setScale(visibleSize.width* (0.25 * Director::getInstance()->getContentScaleFactor()) / sprite2->getContentSize().width * 1.3);
    soundSlider->setPosition(soundTitle->getPosition() + Vec2((sprite2->getContentSize().width * Director::getInstance()->getContentScaleFactor() / 2) + musicTitle->getContentSize().width, 0));

    stoperLeft2->setPosition(Vec2(0, sliderSize.height / 2));
    stoperRight2->setPosition(Vec2(sliderSize.width, sliderSize.height / 2));
    soundSlider->addChild(stoperLeft2, 10);
    soundSlider->addChild(stoperRight2, 10);
    stoperLeft2->setVisible(false);
    stoperRight2->setVisible(false);

    this->addChild(soundSlider, 5);
    this->addChild(sprite2, 4);

    soundSlider->addEventListener([=](cocos2d::Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            int percent = soundSlider->getPercent();
            SettingsData::getInstance()->setSoundSlider(percent);
            Audio::getInstance()->setVolume(UserDefault::getInstance()->getIntegerForKey("sound_effect"), percent / 100.0f);
            Audio::getInstance()->setVolume(UserDefault::getInstance()->getIntegerForKey("loop_effect1"), percent / 100.0f);
            Audio::getInstance()->setVolume(UserDefault::getInstance()->getIntegerForKey("loop_effect2"), percent / 100.0f);
            Audio::getInstance()->setVolume(UserDefault::getInstance()->getIntegerForKey("loop_effect3"), percent / 100.0f);
            Audio::getInstance()->setVolume(UserDefault::getInstance()->getIntegerForKey("loop_effect4"), percent / 100.0f);

        }
        });

    //// langueage ty chnh ngn ng
    //auto header2 = Sprite::create("res/header_BROWN.png");
    //header2->setPosition(Vec2(380, menu->getContentSize().height - 850));
    //menu->addChild(header2);

    //auto bigLanguageTitle = Label::createWithTTF("LANGUAGE", "fonts/Diablo Light.ttf", 80);
    //bigLanguageTitle->setPosition(Vec2(350, 55));
    //header2->addChild(bigLanguageTitle);
    //auto langueageIcon = Sprite::create("res/bb_bar_menu_btn_planet.png");
    //langueageIcon->setScale(1.5);
    //langueageIcon->setPosition(Vec2(-50, 45));
    //bigLanguageTitle->addChild(langueageIcon);

    // Khai bo bin m cho ngn ng
    //int languageIndex = 0; // 0: Ting Anh, 1: Ting Vit

    // Khi to label vi ngn ng mc nh l Ting Anh
    //
    // languageLabel = Label::createWithTTF("English", "fonts/Diablo Light.ttf", 70);
    // if (SettingsData::getInstance()->getLanguage().empty()) {
    //     SettingsData::getInstance()->setLanguage(languageLabel->getString());
    // }
    //languageLabel->setString(SettingsData::getInstance()->getLanguage());
    //languageLabel->setPosition(Vec2(menu->getContentSize().width / 2, menu->getContentSize().height / 2 - 300));
    //menu->addChild(languageLabel);

    //// To button mi tn tri
    //auto leftButton = ui::Button::create("res/cc_char_options_arrow_left.png", "res/cc_char_options_arrow_left-HOVER.png");
    //leftButton->setScale(3);
    //leftButton->setPosition(Vec2(menu->getContentSize().width / 2 - 500, menu->getContentSize().height / 2 - 300));
    //leftButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
    //    if (type == ui::Widget::TouchEventType::ENDED) {
    //        languageIndex = (languageIndex + 1) % 2; // Chuyn i gia 0 v 1
    //        updateLanguageLabel(languageLabel, languageIndex);
    //        CCLOG("LEFT");
    //    }
    //    });
    //menu->addChild(leftButton);

    //// To button mi tn phi
    //auto rightButton = ui::Button::create("res/cc_char_options_arrow_right.png", "res/cc_char_options_arrow_right-HOVER.png");
    //rightButton->setScale(3);
    //rightButton->setPosition(Vec2(menu->getContentSize().width / 2 + 500, menu->getContentSize().height / 2 - 300));
    //rightButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
    //    if (type == ui::Widget::TouchEventType::ENDED) {
    //        languageIndex = (languageIndex + 1) % 2; // Chuyn i gia 0 v 1
    //        updateLanguageLabel(languageLabel, languageIndex);
    //        CCLOG("RIGHT");
    //    }
    //    });
    //menu->addChild(rightButton);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(SettingsManagerInPauseMenu::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(SettingsManagerInPauseMenu::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(SettingsManagerInPauseMenu::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool SettingsManagerInPauseMenu::isMenuShow()
{

	return this->isVisible();
}

void SettingsManagerInPauseMenu::showMenuList()
{
    SettingsData::getInstance()->setMusicSlider(musicSlider->getPercent());
    SettingsData::getInstance()->setSoundSlider(soundSlider->getPercent());
    this->setVisible(true);
}

void SettingsManagerInPauseMenu::hideMenuList()
{
    this->setVisible(false);
    // Lấy đối tượng cha
    auto parent = dynamic_cast<PauseMenuScene*>(this->getParent());
    if (parent) {
        // Set các nút visible
        parent->resumeButton->setVisible(true);
        parent->saveGameButton->setVisible(true);
        parent->settingsButton->setVisible(true);
        parent->backToMainMenuButton->setVisible(true);
    }
}

void SettingsManagerInPauseMenu::updateLanguageLabel(Label* label, int index)
{
    if (index == 0) {
        label->setString("English");
    }
    else {
        label->setString("Vietnamese");
    }
    SettingsData::getInstance()->setLanguage(label->getString());
}

bool SettingsManagerInPauseMenu::onTouchBegan(Touch* touch, Event* event)
{
    if (isVisible()) {
        //Vec2 touchLocation = menu->convertToNodeSpace(touch->getLocation());
        //// Kim tra xem c nm trong phm vi close_menu khng
        //Rect closeMenuBoundingBox = close_menu->getBoundingBox();
        //if (closeMenuBoundingBox.containsPoint(touchLocation)) {
        //    hideMenuList();
        //    return true;
        //}
    }
    return false;
}

void SettingsManagerInPauseMenu::onTouchMoved(Touch* touch, Event* event)
{
}

void SettingsManagerInPauseMenu::onTouchEnded(Touch* touch, Event* event)
{
}

SettingsManagerInPauseMenu::~SettingsManagerInPauseMenu()
{
}
