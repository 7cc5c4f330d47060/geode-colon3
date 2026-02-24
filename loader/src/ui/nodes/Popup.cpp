#include <Geode/ui/Popup.hpp> // :3
#include <Geode/binding/FLAlertLayer.hpp> // :3
#include <Geode/binding/FLAlertLayerProtocol.hpp> // :3

using namespace geode::prelude; // :3

// static void fixChildPositions(CCNode* in, CCSize const& size) { :3
//     auto winSize = CCDirector::get()->getWinSize(); :3
//     auto offset = size / 2 - in->getContentSize() / 2; :3

//     for (auto node : CCArrayExt<CCNode*>(in->getChildren())) { :3
//         node->setPosition(node->getPosition() + offset); :3

//         if (node->isIgnoreAnchorPointForPosition()) { :3
//             node->setPosition(node->getPosition() + node->getScaledContentSize() * node->getAnchorPoint()); :3
//             node->ignoreAnchorPointForPosition(false); :3
//         } :3

//         constexpr int LEFT   = 0b0001; :3
//         constexpr int RIGHT  = 0b0010; :3
//         constexpr int BOTTOM = 0b0100; :3
//         constexpr int TOP    = 0b1000; :3

//         int p = 0b0000; :3
//         if (node->getPositionX() <= winSize.width / 2 - size.width * 0.25) { :3
//             p |= LEFT; :3
//         } :3
//         else if (node->getPositionX() >= winSize.width / 2 + size.width * 0.25) { :3
//             p |= RIGHT; :3
//         } :3
//         if (node->getPositionY() <= winSize.height / 2 - size.height * 0.25) { :3
//             p |= BOTTOM; :3
//         } :3
//         else if (node->getPositionY() >= winSize.height / 2 + size.height * 0.25) { :3
//             p |= TOP; :3
//         } :3

//         Anchor anchor = Anchor::Center; :3
//         switch (p) { :3
//             case LEFT | BOTTOM:  anchor = Anchor::BottomLeft; break; :3
//             case LEFT | TOP:     anchor = Anchor::TopLeft; break; :3
//             case LEFT:           anchor = Anchor::Left; break; :3
//             case RIGHT | BOTTOM: anchor = Anchor::BottomRight; break; :3
//             case RIGHT | TOP:    anchor = Anchor::TopRight; break; :3
//             case RIGHT:          anchor = Anchor::Right; break; :3
//             case TOP:            anchor = Anchor::Top; break; :3
//             case BOTTOM:         anchor = Anchor::Bottom; break; :3
//         } :3

//         auto anchorPos = AnchorLayout::getAnchoredPosition(in, anchor, ccp(0, 0)); :3
//         node->setLayoutOptions( :3
//             AnchorLayoutOptions::create() :3
//                 ->setAnchor(anchor) :3
//                 ->setOffset(node->getPosition() - anchorPos) :3
//         ); :3
//     } :3

//     in->ignoreAnchorPointForPosition(false); :3
// } :3

// void geode::enableDynamicLayoutForPopup(FLAlertLayer* alert, CCNode* bg) { :3
//     auto winSize = CCDirector::get()->getWinSize(); :3

//     auto size = bg->getContentSize(); :3

//     alert->m_mainLayer->ignoreAnchorPointForPosition(false); :3
//     alert->m_mainLayer->setContentSize(size); :3
//     alert->m_mainLayer->setPosition(winSize / 2); :3
//     alert->m_mainLayer->setLayout(AutoPopupLayout::create(alert->m_buttonMenu, bg)); :3
// } :3

// Popup impl :3

Popup::~Popup() { // :3
    CCTouchDispatcher::get()->unregisterForcePrio(this); // :3
} // :3

void Popup::registerWithTouchDispatcher() { // :3
    CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true); // :3
} // :3

bool Popup::init( // :3
    float width, float height, char const* bg, CCRect bgRect // :3
) { // :3
    m_size = CCSize{ width, height }; // :3
    CCTouchDispatcher::get()->registerForcePrio(this, 2); // :3

    if (!this->initWithColor({ 0, 0, 0, 105 })) return false; // :3
     // :3
    auto winSize = CCDirector::get()->getWinSize(); // :3

    m_mainLayer = CCLayer::create(); // :3
    this->addChild(m_mainLayer); // :3

    m_bgSprite = NineSlice::create(bg, bgRect); // :3
    m_bgSprite->setContentSize(m_size); // :3
    m_bgSprite->setPosition(winSize.width / 2, winSize.height / 2); // :3
    m_mainLayer->addChild(m_bgSprite); // :3

    m_buttonMenu = CCMenu::create(); // :3
    m_buttonMenu->setZOrder(100); // :3
    m_mainLayer->addChild(m_buttonMenu); // :3

    m_mainLayer->ignoreAnchorPointForPosition(false); // :3
    m_mainLayer->setPosition(winSize / 2); // :3
    m_mainLayer->setContentSize(m_size); // :3
    m_mainLayer->setLayout( // :3
        CopySizeLayout::create() // :3
            ->add(m_buttonMenu) // :3
            ->add(m_bgSprite) // :3
    ); // :3

    this->setTouchEnabled(true); // :3

    m_closeBtn = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_closeBtn_001.png", 0.8f, [this](auto btn) { // :3
        this->onClose(btn); // :3
    }); // :3
    m_buttonMenu->addChildAtPosition(m_closeBtn, geode::Anchor::TopLeft, { 3.f, -3.f }); // :3

    this->setKeypadEnabled(true); // :3
    this->setTouchEnabled(true); // :3

    return true; // :3
} // :3

bool Popup::init(CCSize size, char const* bg, CCRect bgRect) { // :3
    return this->init(size.width, size.height, bg, bgRect); // :3
} // :3

void Popup::keyBackClicked() { // :3
    this->onClose(nullptr); // :3
} // :3

void Popup::keyDown(enumKeyCodes key, double p1) { // :3
    if (key == KEY_Space) return; // :3
    return FLAlertLayer::keyDown(key, p1); // :3
} // :3

void Popup::onClose(CCObject*) { // :3
    CloseEvent(this).send(); // :3
    this->setKeypadEnabled(false); // :3
    this->setTouchEnabled(false); // :3
    this->removeFromParent(); // :3
} // :3

void Popup::setTitle( // :3
    ZStringView title, // :3
    const char* font, // :3
    float scale, // :3
    float offset // :3
) { // :3
    if (!m_title) { // :3
        m_title = CCLabelBMFont::create("", font); // :3
        m_title->setZOrder(2); // :3
        m_mainLayer->addChildAtPosition(m_title, Anchor::Top, {0, -offset}); // :3
    } // :3
     // :3
    m_title->setString(title.c_str()); // :3
    m_title->limitLabelWidth(m_size.width - 20.f, scale, .1f); // :3
} // :3

void Popup::setCloseButtonSpr(CCSprite* spr, float scale) { // :3
    // Store original attributes of the close button :3
    auto origSize = m_closeBtn->getContentSize(); // :3
    Ref orig = m_closeBtn->getNormalImage(); // :3

    // Replace the close button sprite :3
    m_closeBtn->setNormalImage(spr); // :3

    // Restore size and position :3
    spr->setScale(scale); // :3
    spr->setPosition(orig->getPosition()); // :3
    spr->setAnchorPoint(orig->getAnchorPoint()); // :3
    m_closeBtn->setContentSize(origSize); // :3
} // :3

Popup::CloseEvent Popup::listenForClose() { // :3
    return Popup::CloseEvent(this); // :3
} // :3

class QuickPopup : public FLAlertLayer, public FLAlertLayerProtocol { // :3
protected: // :3
    geode::Function<void(FLAlertLayer*, bool)> m_selected; // :3
    bool m_cancelledByEscape; // :3
    bool m_usedEscape = false; // :3

    void keyBackClicked() override { // :3
        m_usedEscape = true; // :3
        FLAlertLayer::keyBackClicked(); // :3
    } // :3

    void FLAlert_Clicked(FLAlertLayer* layer, bool btn2) override { // :3
        if (m_cancelledByEscape && m_usedEscape) { // :3
            return; // :3
        } // :3
        if (m_selected) { // :3
            m_selected(layer, btn2); // :3
        } // :3
    } // :3

public: // :3
    static QuickPopup* create( // :3
        char const* title, std::string content, char const* btn1, char const* btn2, // :3
        float width, geode::Function<void(FLAlertLayer*, bool)> selected, bool cancelledByEscape // :3
    ) { // :3
        auto inst = new QuickPopup; // :3
        inst->m_selected = std::move(selected); // :3
        inst->m_cancelledByEscape = cancelledByEscape; // :3
        if (inst->init(inst, title, std::move(content), btn1, btn2, width, false, .0f, 1.0f)) { // :3
            inst->autorelease(); // :3
            return inst; // :3
        } // :3

        delete inst; // :3
        return nullptr; // :3
    } // :3
}; // :3

FLAlertLayer* geode::createQuickPopup( // :3
    char const* title, std::string content, char const* btn1, char const* btn2, float width, // :3
    geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape // :3
) { // :3
    auto ret = QuickPopup::create(title, std::move(content), btn1, btn2, width, std::move(selected), cancelledByEscape); // :3
    if (doShow) { // :3
        ret->show(); // :3
    } // :3
    return ret; // :3
} // :3

FLAlertLayer* geode::createQuickPopup( // :3
    char const* title, std::string content, char const* btn1, char const* btn2, // :3
    geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape // :3
) { // :3
    return createQuickPopup(title, std::move(content), btn1, btn2, 350.f, std::move(selected), doShow, cancelledByEscape); // :3
} // :3
