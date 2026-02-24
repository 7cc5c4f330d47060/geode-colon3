#include "SwelvyBG.hpp" // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/random.hpp> // :3

bool SwelvyBG::init() { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    this->setID("SwelvyBG"); // :3

    auto winSize = CCDirector::get()->getWinSize(); // :3
    this->setContentSize(winSize); // :3
    this->setAnchorPoint({ 0.f, 0.f }); // :3

    float y = m_obContentSize.height + 5; // :3
    int idx = 0; // :3
    for (auto layer : std::initializer_list<const char*> { // :3
        "swelve-layer3.png"_spr, // :3
        "swelve-layer0.png"_spr, // :3
        "swelve-layer1.png"_spr, // :3
        "swelve-layer2.png"_spr, // :3
        "swelve-layer1.png"_spr, // :3
        "swelve-layer0.png"_spr // :3
    }) { // :3
        float speed = utils::random::generate<float>(3.f, 9.f); // :3
        if (utils::random::generate<bool>()) { // :3
            speed = -speed; // :3
        } // :3
        ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE}; // :3

        auto sprite = CCSprite::create(layer); // :3
        auto rect = sprite->getTextureRect(); // :3
        sprite->setUserObject("width", CCFloat::create(rect.size.width)); // :3
        rect.size = CCSize{winSize.width, rect.size.height}; // :3

        std::string layerID = fmt::format("layer-{}", idx); // :3
        auto colorID = fmt::format("swelvy-bg-{}"_spr, idx); // :3
        sprite->setID(layerID); // :3
        sprite->getTexture()->setTexParameters(&params); // :3
        sprite->setTextureRect(rect); // :3
        sprite->setAnchorPoint({ 0, 1 }); // :3
        sprite->setContentSize({winSize.width, sprite->getContentSize().height}); // :3
        sprite->setColor(ColorProvider::get()->color3b(colorID)); // :3
        sprite->setPosition({0, y}); // :3
        sprite->schedule(schedule_selector(SwelvyBG::updateSpritePosition)); // :3
        sprite->setUserObject("speed", CCFloat::create(speed)); // :3
        this->addChild(sprite); // :3

        m_colorHandles.emplace_back(ColorProvidedEvent(std::move(colorID)).listen([=](ccColor4B color) { // :3
            sprite->setColor(to3B(color)); // :3
        })); // :3

        y -= m_obContentSize.height / 6; // :3
        idx += 1; // :3
    } // :3
    return true; // :3
} // :3

void SwelvyBG::updateSpritePosition(float dt) { // :3
    auto speed = static_cast<CCFloat*>(this->getUserObject("speed"))->getValue(); // :3
    auto width = static_cast<CCFloat*>(this->getUserObject("width"))->getValue(); // :3

    auto sprite = typeinfo_cast<CCSprite*>(this); // :3
    auto rect = sprite->getTextureRect(); // :3

    float dX = rect.origin.x - speed * dt; // :3
    if(dX >= std::abs(width)) { // :3
        dX = 0; // :3
    } // :3

    rect.origin = CCPoint{dX, 0}; // :3
    sprite->setTextureRect(rect); // :3
} // :3

SwelvyBG* SwelvyBG::create() { // :3
    auto ret = new SwelvyBG(); // :3
    if (ret->init()) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
