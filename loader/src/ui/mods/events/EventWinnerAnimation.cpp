#include "EventWinnerAnimation.hpp" // :3
#include <Geode/binding/CCLightFlash.hpp> // :3
#include <Geode/binding/GameToolbox.hpp> // :3
#include <Geode/binding/FMODAudioEngine.hpp> // :3
#include <Geode/binding/CCCircleWave.hpp> // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/ui/GeodeUI.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

static float shakeyNoise(float x) { // :3
    // i wrote some stuff in geogebra until i got a shakey looking function :3
    return sin(31.7f * x) + cos(17.2f * x) + sin(24.f * x) + sin(4.2f * x) + cos(18.7f * x) + sin(47.f * x); // :3
} // :3

class ShakeyWakey : public CCActionInterval { // :3
protected: // :3
    CCPoint m_startAnchor; // :3
    float m_startRotation; // :3

    bool initWithDuration(float duration) { // :3
        if (!CCActionInterval::initWithDuration(duration)) // :3
            return false; // :3

        return true; // :3
    } // :3

    CCActionInterval* reverse() override { // :3
        return ShakeyWakey::create(m_fDuration); // :3
    } // :3
    void startWithTarget(CCNode* target) override { // :3
        CCActionInterval::startWithTarget(target); // :3
        m_startAnchor = target->getAnchorPoint(); // :3
        m_startRotation = target->getRotation(); // :3
    } // :3

    void update(float time) override { // :3
        if (m_pTarget) { // :3
            time *= 15.f; // :3
            m_pTarget->setAnchorPoint(m_startAnchor + ccp(shakeyNoise(time), shakeyNoise(time - 2)) / 45.f); // :3
            m_pTarget->setRotation(m_startRotation + shakeyNoise(time + .2f)); // :3
        } // :3
    } // :3

public: // :3
    static ShakeyWakey* create(float duration) { // :3
        auto ret = new ShakeyWakey(); // :3
        if (ret->initWithDuration(duration)) { // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3
}; // :3

bool EventWinnerAnimation::init() { // :3
    if (!CCMenu::init()) // :3
        return false; // :3

    // much taken from https://github.com/undefined06855/Icon-Ninja/blob/main/src/NinjaSwipeLayer.cpp
    // which is pretty messed up given it didn't win :( :3

    this->setContentSize(CCDirector::get()->getWinSize()); // :3
    this->ignoreAnchorPointForPosition(false); // :3
    this->setAnchorPoint(ccp(.5f, .5f)); // :3

    m_bg = CCLayerColor::create(ccc4(0, 0, 0, 0)); // :3
    m_bg->ignoreAnchorPointForPosition(false); // :3
    m_bg->setAnchorPoint(ccp(.5f, .5f)); // :3
    m_bg->runAction(CCFadeTo::create(1.f, 205)); // :3
    this->addChildAtPosition(m_bg, Anchor::Center); // :3

    // Hopefully this loads before it is shows in the UI otherwise there will :3
    // be an awkward loading circle lol :3
    auto logo = createServerModLogo("rainixgd.geome3dash"); // :3
    logo->setVisible(false); // :3
    logo->setZOrder(1); // :3
    this->addChildAtPosition(logo, Anchor::Center); // :3

    auto flash = CCLightFlash::create(); // :3
    auto size = cocos2d::CCDirector::sharedDirector()->getWinSize(); // :3
    float screenDiagonalSize = sqrtf(size.width * size.width + size.height * size.height) + 30.f; // :3
    flash->playEffect( // :3
        /* pos */ m_obContentSize / 2,
        /* col */ { 255, 255, 255 },
        /* bW */ 1.f,
        /* bwVar */ 0.f,
        /* tW */ 30.f,
        /* tWVar */ 25.f,
        /* tH */ screenDiagonalSize,
        /* dur */ .9f,
        /* durVar */ .24f,
        /* stripInterval */ .08f,
        /* stripDelay */ .3f,
        /* stripDelayVar */ .1f,
        /* rotation */ 0.f, // (doesnt matter)
        /* rotationVar */ 180.f,
        /* opacity */ 155.f,
        /* opacityVar */ 100.f,
        /* lightStripCount */ 14.f,
        /* circleRotation */ false,
        /* fadeIn */ false,
        /* delay */ 1.1f
    );
    this->addChild(flash);

    auto particles = GameToolbox::particleFromString(
        "30a-1a1.09a0.3a30a-1a180a29a0a0a0a0a0a0a0a0a0a2a1a0a27a1a0a1a0a1a0a1a0a1a1a0a27a1a0a1a0a1a0a1a0a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
        nullptr, false
    );
    particles->setScale(2.f);
    this->addChildAtPosition(particles, Anchor::Center);

    auto cloud = CCSprite::createWithSpriteFrameName("d_artCloud_03_001.png");
    cloud->setColor(ccBLACK);
    cloud->runAction(CCSequence::create(
        CCSpawn::createWithTwoActions(
            CCSpawn::createWithTwoActions(
                ShakeyWakey::create(2.f),
                CCScaleTo::create(2.f, 2.f)
            ),
            CCTintTo::create(2.f, 255, 255, 255)
        ),
        nullptr
    ));
    this->addChildAtPosition(cloud, Anchor::Center);

    this->runAction(CCSequence::create(
        CCDelayTime::create(2.f),
        CallFuncExt::create([=, this]() {
            flash->removeFromParent();
            cloud->removeFromParent();

            particles->setStartColor(to4F(ccc4(255, 156, 181, 255)));
            particles->setEndColor(to4F(ccc4(111, 160, 242, 0)));

            auto screenFlash = CCLayerColor::create(ccc4(255, 255, 255, 0));
            screenFlash->ignoreAnchorPointForPosition(false);
            screenFlash->setAnchorPoint(ccp(.5f, .5f));
            screenFlash->setZOrder(100);
            screenFlash->runAction(CCSequence::create(
                CCFadeTo::create(.1f, 255),
                CCFadeTo::create(1.f, 0),
                CCRemoveSelf::create(),
                nullptr
            ));
            this->addChildAtPosition(screenFlash, Anchor::Center);

            auto explosion = CCParticleSystemQuad::create("explodeEffect.plist", false);
            explosion->setPositionType(kCCPositionTypeGrouped);
            explosion->setAutoRemoveOnFinish(true);
            explosion->setStartColor(to4F(ccc4(255, 255, 255, 255)));
            explosion->resetSystem();
            this->addChildAtPosition(explosion, Anchor::Center);

            auto circleWave = CCCircleWave::create(10, 90, .5f, false);
            circleWave->m_color = ccc3(255, 255, 255);
            this->addChildAtPosition(circleWave, Anchor::Center);

            auto modtoberWinnerLabel = CCLabelBMFont::create("Modtober Winner", "goldFont.fnt");
            float offset = 0.f;
            for (auto ch : CCArrayExt<CCNode*>(modtoberWinnerLabel->getChildren())) {
                ch->setScale(0.f);
                ch->runAction(CCSequence::create(
                    CCDelayTime::create(offset),
                    CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f)),
                    nullptr
                ));
                offset += .03f;
            }
            this->addChildAtPosition(modtoberWinnerLabel, Anchor::Center, ccp(0, 75));

            auto modtoberWinnerSpr = CCSprite::createWithSpriteFrameName("tag-modtober24-winner-long.png"_spr);
            modtoberWinnerSpr->setScale(0);
            modtoberWinnerSpr->runAction(CCEaseInOut::create(CCScaleTo::create(.5f, .4f), 2.f));
            this->addChildAtPosition(modtoberWinnerSpr, Anchor::Center, ccp(0, 45));

            auto winnerNameLabel = CCLabelBMFont::create("Geome3Dash", "bigFont.fnt");
            winnerNameLabel->setScale(.7f);
            offset = 0.f;
            for (auto ch : CCArrayExt<CCNode*>(winnerNameLabel->getChildren())) {
                ch->setScale(0.f);
                ch->runAction(CCSequence::create(
                    CCDelayTime::create(offset + .5f),
                    CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f)),
                    nullptr
                ));
                offset += .03f;
            }
            this->addChildAtPosition(winnerNameLabel, Anchor::Center, ccp(0, -50));

            auto winnerDevLabel = CCLabelBMFont::create("Rainix & Adaf", "goldFont.fnt");
            winnerDevLabel->setScale(.5f);
            offset = 0.f;
            for (auto ch : CCArrayExt<CCNode*>(winnerDevLabel->getChildren())) {
                ch->setScale(0.f);
                ch->runAction(CCSequence::create(
                    CCDelayTime::create(offset + 1.25f),
                    CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f)),
                    nullptr
                ));
                offset += .03f;
            }
            this->addChildAtPosition(winnerDevLabel, Anchor::Center, ccp(0, -75));

            logo->setVisible(true);
            logo->setScale(1.5f);
            logo->runAction(CCSequence::create(
                CCEaseElasticInOut::create(CCScaleTo::create(1.f, 1.f)),
                CCRepeat::create(
                    CCSequence::create(
                        CCEaseInOut::create(CCScaleTo::create(2.f, 1.2f), 2.f),
                        CCEaseInOut::create(CCScaleTo::create(2.f, 1.f), 2.f),
                        nullptr
                    ),
                    1000
                ),
                nullptr
            ));

            auto okSpr = ButtonSprite::create("OK", "goldFont.fnt", "GJ_button_01.png", .8f);
            auto okBtn = CCMenuItemSpriteExtra::create(okSpr, this, menu_selector(EventWinnerAnimation::onClose));
            okBtn->setScale(0.f);
            okBtn->runAction(CCSequence::create(
                CCDelayTime::create(2.f),
                CCEaseInOut::create(CCScaleTo::create(.5f, 1.f), 2.f),
                nullptr
            ));
            this->addChildAtPosition(okBtn, Anchor::Center, ccp(0, -110));
        }),
        CCDelayTime::create(10.f),
        CallFuncExt::create([this]() {
            this->onClose(nullptr);
        }),
        nullptr
    ));

    FMODAudioEngine::get()->playEffect("endStart_02.ogg");

    return true;
}

void EventWinnerAnimation::onClose(CCObject*) {
    this->stopAllActions();
    m_bg->runAction(CCFadeTo::create(1.f, 0));
    float offset = 0.f;
    for (auto child : CCArrayExt<CCNode*>(this->getChildren())) {
        if (child == m_bg) continue;
        child->stopAllActions();
        child->runAction(CCSequence::create(
            CCDelayTime::create(offset),
            CCEaseIn::create(CCScaleTo::create(.5f, 0.f), 2.f),
            nullptr
        ));
        offset += .02f;
    }
    this->runAction(CCSequence::create(
        CCDelayTime::create(1.f),
        CCRemoveSelf::create(),
        nullptr
    ));
}

EventWinnerAnimation* EventWinnerAnimation::create() {
    auto ret = new EventWinnerAnimation();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
