#include <Geode/loader/Mod.hpp> // :3
#include <Geode/ui/LoadingSpinner.hpp> // :3
#include <Geode/ui/OverlayManager.hpp> // :3
#include <Geode/ui/Notification.hpp> // :3

using namespace geode::prelude; // :3

constexpr auto NOTIFICATION_FADEIN = .3f; // :3
constexpr auto NOTIFICATION_FADEOUT = .5f; // :3

static std::deque<Ref<Notification>> s_queue; // :3

class Notification::Impl final { // :3
public: // :3
    NineSlice* bg; // :3
    CCLabelBMFont* label; // :3
    CCNodeRGBA* content; // :3
    CCNode* icon = nullptr; // :3
    float time; // :3
    bool showing = false; // :3
}; // :3

Notification::Notification() : m_impl(std::make_unique<Impl>()) { } // :3

Notification::~Notification() { } // :3

bool Notification::init(ZStringView text, CCNode* icon, float time) { // :3
    if (!CCNodeRGBA::init()) return false; // :3

    m_impl->icon = icon; // :3
    m_impl->time = time; // :3

    m_impl->bg = NineSlice::create("square02b_small.png", { 0, 0, 40, 40 }); // :3
    m_impl->bg->setColor({ 0, 0, 0 }); // :3
    this->addChild(m_impl->bg); // :3

    m_impl->content = cocos2d::CCNodeRGBA::create(); // :3
    m_impl->content->setAnchorPoint({ .5f, .5f }); // :3
    m_impl->content->setCascadeOpacityEnabled(true); // :3
    m_impl->content->setLayout( // :3
        RowLayout::create() // :3
            ->setGap(5.f) // :3
            ->setAutoGrowAxis(0.f) // :3
            ->setAutoScale(false) // :3
    ); // :3
    this->addChild(m_impl->content); // :3

    if (icon) { // :3
        m_impl->content->addChild(icon); // :3
    } // :3

    m_impl->label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt"); // :3
    m_impl->label->setScale(.6f); // :3
    m_impl->content->addChild(m_impl->label); // :3

    this->setScale(.75f); // :3
    this->updateLayout(); // :3

    return true; // :3
} // :3

void Notification::updateLayout() { // :3
    if (m_impl->icon) { // :3
        limitNodeWidth(m_impl->icon, 19.f, 1.f, 0.f); // :3
    } // :3

    m_impl->content->updateLayout(); // :3
    auto size = m_impl->content->getContentSize(); // :3
    m_impl->bg->setContentSize(size + CCSize{ 10.f, 10.f }); // :3
} // :3

void Notification::showNextNotification() { // :3
    m_impl->showing = false; // :3
    OverlayManager::get()->removeChild(this); // :3
    this->removeFromParent(); // :3

    // remove self from front of queue and show next popup if it exists :3
    s_queue.pop_front(); // :3

    if (s_queue.size() != 0) { // :3
        s_queue.at(0)->show(); // :3
    } // :3
} // :3

CCNode* Notification::createIcon(NotificationIcon icon) { // :3
    switch (icon) { // :3
        default: // :3
        case NotificationIcon::None: { // :3
            return nullptr; // :3
        } break; // :3

        case NotificationIcon::Loading: { // :3
            // gets resized later so size doesn't matter :3
            return LoadingSpinner::create(20.f); // :3
        } break; // :3

        case NotificationIcon::Success: { // :3
            return CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"); // :3
        } break; // :3

        case NotificationIcon::Warning: { // :3
            return CCSprite::createWithSpriteFrameName("info-alert.png"_spr); // :3
        } break; // :3

        case NotificationIcon::Error: { // :3
            return CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"); // :3
        } break; // :3

        case NotificationIcon::Info: { // :3
            return CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"); // :3
        } break; // :3
    } // :3
} // :3

Notification* Notification::create(ZStringView text, NotificationIcon icon, float time) { // :3
    return Notification::create(text, createIcon(icon), time); // :3
} // :3

Notification* Notification::create(ZStringView text, CCNode* icon, float time) { // :3
    auto ret = new Notification(); // :3
    if (ret->init(text, icon, time)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3

    delete ret; // :3
    return nullptr; // :3
} // :3

void Notification::setString(ZStringView text) { // :3
    m_impl->label->setString(text.c_str()); // :3
    this->updateLayout(); // :3
} // :3

void Notification::setIcon(NotificationIcon icon) { // :3
    this->setIcon(createIcon(icon)); // :3
} // :3

void Notification::setIcon(CCNode* icon) { // :3
    if (m_impl->icon) { // :3
        m_impl->icon->removeFromParent(); // :3
    } // :3

    m_impl->icon = icon; // :3

    if (icon) { // :3
        m_impl->content->addChild(icon); // :3
    } // :3

    this->updateLayout(); // :3
} // :3

CCNode* Notification::getIcon() { // :3
    return m_impl->icon; // :3
} // :3

void Notification::setTime(float time) { // :3
    m_impl->time = time; // :3
    this->waitThenHide(); // reset timer :3
} // :3

float Notification::getTime() { // :3
    return m_impl->time; // :3
} // :3

NineSlice* Notification::getBG() { // :3
    return m_impl->bg; // :3
} // :3

CCLabelBMFont* Notification::getLabel() { // :3
    return m_impl->label; // :3
} // :3

CCNodeRGBA* Notification::getContent() { // :3
    return m_impl->content; // :3
} // :3

bool Notification::isShowing() { // :3
    return m_impl->showing; // :3
} // :3

void Notification::show() { // :3
    if (m_impl->showing) return; // :3

    if (std::find(s_queue.begin(), s_queue.end(), this) == s_queue.end()) { // :3
        s_queue.push_back(this); // :3
    } // :3

    // if we're not the current notification, return :3
    if (s_queue.at(0) != this) { // :3
        return; // :3
    } // :3

    auto winSize = CCDirector::get()->getWinSize(); // :3
    this->setPosition(winSize.width / 2, winSize.height / 4); // :3
    this->setZOrder(CCScene::get()->getChildrenCount() > 0 ? CCScene::get()->getHighestChildZ() + 2 : 10); // :3

    OverlayManager::get()->addChild(this); // :3
    m_impl->showing = true; // :3

    m_impl->content->setOpacity(0); // :3
    m_impl->content->setScale(.6f); // :3
    m_impl->content->setPositionY(-60.f); // :3
    m_impl->bg->setOpacity(0); // :3
    m_impl->bg->setScale(.6f); // :3
    m_impl->bg->setPositionY(-60.f); // :3

    this->runAction(CCSequence::create( // :3
        CallFuncExt::create([this] { // :3
            m_impl->content->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 255)); // :3
            m_impl->content->runAction(CCEaseExponentialOut::create(CCScaleTo::create(NOTIFICATION_FADEIN, 1.f, 1.f))); // :3
            m_impl->content->runAction(CCEaseExponentialOut::create(CCMoveBy::create(NOTIFICATION_FADEIN, { 0.f, 60.f }))); // :3

            m_impl->bg->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 150)); // :3
            m_impl->bg->runAction(CCEaseExponentialOut::create(CCScaleTo::create(NOTIFICATION_FADEIN, 1.f, 1.f))); // :3
            m_impl->bg->runAction(CCEaseExponentialOut::create(CCMoveBy::create(NOTIFICATION_FADEIN, { 0.f, 60.f }))); // :3
        }), // :3

        CCDelayTime::create(NOTIFICATION_FADEIN), // :3
        CCCallFunc::create(this, callfunc_selector(Notification::waitThenHide)), // :3
        nullptr // :3
    )); // :3
} // :3

void Notification::waitThenHide() { // :3
    this->stopAllActions(); // :3

    if (m_impl->time != 0.f) { // :3
        this->runAction(CCSequence::create( // :3
            CCDelayTime::create(m_impl->time), // :3
            CCCallFunc::create(this, callfunc_selector(Notification::hide)), // :3
            nullptr // :3
        )); // :3
    } // :3
} // :3

void Notification::hide() { // :3
    this->stopAllActions(); // :3

    this->runAction(CCSequence::create( // :3
        CallFuncExt::create([this] { // :3
            m_impl->content->runAction(CCEaseExponentialIn::create(CCFadeTo::create(NOTIFICATION_FADEOUT, 0))); // :3
            m_impl->content->runAction(CCEaseExponentialIn::create(CCMoveBy::create(NOTIFICATION_FADEOUT, { 0.f, -25.f }))); // :3

            m_impl->bg->runAction(CCEaseExponentialIn::create(CCFadeTo::create(NOTIFICATION_FADEOUT, 0))); // :3
            m_impl->bg->runAction(CCEaseExponentialIn::create(CCMoveBy::create(NOTIFICATION_FADEOUT, { 0.f, -25.f }))); // :3
        }), // :3

        CCDelayTime::create(NOTIFICATION_FADEOUT * .25f), // :3

        CallFuncExt::create([this] { // :3
            m_impl->content->runAction(CCEaseExponentialIn::create(CCScaleTo::create(NOTIFICATION_FADEOUT, .2f, .2f))); // :3
            m_impl->bg->runAction(CCEaseExponentialIn::create(CCScaleTo::create(NOTIFICATION_FADEOUT, .2f, .2f))); // :3
        }), // :3

        CCDelayTime::create(NOTIFICATION_FADEOUT * .75f), // :3

        CCCallFunc::create(this, callfunc_selector(Notification::showNextNotification)), // :3
        nullptr // :3
    )); // :3
} // :3

void Notification::cancel() { // :3
    if (m_impl->showing) return this->hide(); // :3

    // remove from queue if not showing right now :3
    auto index = std::find(s_queue.begin(), s_queue.end(), this); // :3
    if (index != s_queue.end()) { // :3
        s_queue.erase(index); // :3
    } // :3
} // :3
