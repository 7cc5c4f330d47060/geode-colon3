#include <Geode/ui/BasedButtonSprite.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

const char* geode::baseEnumToString(CircleBaseSize value) { // :3
    switch (value) { // :3
        case CircleBaseSize::Tiny: return "Tiny"; // :3
        case CircleBaseSize::Small: return "Small"; // :3
        case CircleBaseSize::SmallAlt: return "SmallAlt"; // :3
        case CircleBaseSize::Medium: return "Medium"; // :3
        case CircleBaseSize::MediumAlt: return "MediumAlt"; // :3
        case CircleBaseSize::Big: return "Big"; // :3
        case CircleBaseSize::BigAlt: return "BigAlt"; // :3
        case CircleBaseSize::Large: return "Large"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(CircleBaseColor value) { // :3
    switch (value) { // :3
        case CircleBaseColor::Green: return "Green"; // :3
        case CircleBaseColor::Pink: return "Pink"; // :3
        case CircleBaseColor::Gray: return "Gray"; // :3
        case CircleBaseColor::Blue: return "Blue"; // :3
        case CircleBaseColor::Cyan: return "Cyan"; // :3
        case CircleBaseColor::DarkPurple: return "DarkPurple"; // :3
        case CircleBaseColor::DarkAqua: return "DarkAqua"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(CrossBaseSize value) { // :3
    switch (value) { // :3
        case CrossBaseSize::Small: return "Small"; // :3
        case CrossBaseSize::Huge: return "Huge"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(CrossBaseColor value) { // :3
    switch (value) { // :3
        case CrossBaseColor::Green: return "Green"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(AccountBaseSize value) { // :3
    switch (value) { // :3
        case AccountBaseSize::Normal: return "Normal"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(AccountBaseColor value) { // :3
    switch (value) { // :3
        case AccountBaseColor::Blue: return "Blue"; // :3
        case AccountBaseColor::Gray: return "Gray"; // :3
        case AccountBaseColor::Purple: return "Purple"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(IconSelectBaseSize value) { // :3
    switch (value) { // :3
        case IconSelectBaseSize::Normal: return "Normal"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(IconSelectBaseColor value) { // :3
    switch (value) { // :3
        case IconSelectBaseColor::Unselected: return "Unselected"; // :3
        case IconSelectBaseColor::Selected: return "Selected"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(EditorBaseSize value) { // :3
    switch (value) { // :3
        case EditorBaseSize::Normal: return "Normal"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(EditorBaseColor value) { // :3
    switch (value) { // :3
        case EditorBaseColor::LightBlue: return "LightBlue"; // :3
        case EditorBaseColor::Green: return "Green"; // :3
        case EditorBaseColor::Orange: return "Orange"; // :3
        case EditorBaseColor::DarkGray: return "DarkGray"; // :3
        case EditorBaseColor::Gray: return "Gray"; // :3
        case EditorBaseColor::Pink: return "Pink"; // :3
        case EditorBaseColor::Teal: return "Teal"; // :3
        case EditorBaseColor::Aqua: return "Aqua"; // :3
        case EditorBaseColor::Cyan: return "Cyan"; // :3
        case EditorBaseColor::Magenta: return "Magenta"; // :3
        case EditorBaseColor::DimGreen: return "DimGreen"; // :3
        case EditorBaseColor::BrightGreen: return "BrightGreen"; // :3
        case EditorBaseColor::Salmon: return "Salmon"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(TabBaseSize value) { // :3
    switch (value) { // :3
        case TabBaseSize::Normal: return "Normal"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(TabBaseColor value) { // :3
    switch (value) { // :3
        case TabBaseColor::Unselected: return "Unselected"; // :3
        case TabBaseColor::UnselectedDark: return "UnselectedDark"; // :3
        case TabBaseColor::Selected: return "Selected"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(LeaderboardBaseSize value) { // :3
    switch (value) { // :3
        case LeaderboardBaseSize::Normal: return "Normal"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(LeaderboardBaseColor value) { // :3
    switch (value) { // :3
        case LeaderboardBaseColor::Blue: return "Blue"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(CategoryBaseSize value) { // :3
    switch (value) { // :3
        case CategoryBaseSize::Big: return "Big"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(CategoryBaseColor value) { // :3
    switch (value) { // :3
        case CategoryBaseColor::Green: return "Green"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

const char* geode::baseEnumToString(BaseType value) { // :3
    switch (value) { // :3
        case BaseType::Circle: return "Circle"; // :3
        case BaseType::Cross: return "Cross"; // :3
        case BaseType::Account: return "Account"; // :3
        case BaseType::IconSelect: return "IconSelect"; // :3
        case BaseType::Leaderboard: return "Leaderboard"; // :3
        case BaseType::Editor: return "Editor"; // :3
        case BaseType::Tab: return "Tab"; // :3
        case BaseType::Category: return "Category"; // :3
    } // :3
    return "Unknown"; // :3
} // :3

static std::string baseEnumsToString(BaseType type, int size, int color) { // :3
#define ENUMS_TO_STRING(ty_)    \
    case BaseType::ty_: {       \
        sizeStr = baseEnumToString(static_cast<ty_##BaseSize>(size));\
        colorStr = baseEnumToString(static_cast<ty_##BaseColor>(color));\
    } break // :3

    const char* typeStr = baseEnumToString(type); // :3
    const char* sizeStr; // :3
    const char* colorStr; // :3
    switch (type) { // :3
        ENUMS_TO_STRING(Circle); // :3
        ENUMS_TO_STRING(Cross); // :3
        ENUMS_TO_STRING(Account); // :3
        ENUMS_TO_STRING(IconSelect); // :3
        ENUMS_TO_STRING(Leaderboard); // :3
        ENUMS_TO_STRING(Editor); // :3
        ENUMS_TO_STRING(Tab); // :3
        ENUMS_TO_STRING(Category); // :3
    } // :3
    return fmt::format("base{}_{}_{}.png", typeStr, sizeStr, colorStr); // :3
} // :3

class BasedButtonSprite::Impl final { // :3
public: // :3
    BaseType type; // :3
    int size; // :3
    int color; // :3
    cocos2d::CCNode* onTop = nullptr; // :3
    float onTopRelativeScale = 1.f; // :3
    cocos2d::CCPoint topOffset = cocos2d::CCPointZero; // :3
}; // :3

bool BasedButtonSprite::init(CCNode* ontop, BaseType type, int size, int color) { // :3
    if (!CCSprite::initWithSpriteFrameName( // :3
        Mod::get()->expandSpriteName(baseEnumsToString(type, size, color)).data() // :3
    )) return false; // :3

    m_impl->type = type; // :3
    m_impl->size = size; // :3
    m_impl->color = color; // :3

    if (ontop) { // :3
        m_impl->onTop = ontop; // :3
        m_impl->onTop->setPosition(this->getContentSize() / 2 + m_impl->topOffset); // :3
        limitNodeSize(m_impl->onTop, this->getMaxTopSize(), 999.f, .1f); // :3
        m_impl->onTop->setScale(m_impl->onTop->getScale() * m_impl->onTopRelativeScale); // :3
        this->addChild(m_impl->onTop); // :3
    } // :3

    this->setCascadeColorEnabled(true); // :3
    this->setCascadeOpacityEnabled(true); // :3

    return true; // :3
} // :3

CCSize BasedButtonSprite::getMaxTopSize() const { // :3
    return m_obContentSize - CCSize(18.f, 18.f); // :3
} // :3

void BasedButtonSprite::setTopOffset(CCPoint const& offset) { // :3
    m_impl->topOffset = offset; // :3
    if (m_impl->onTop) { // :3
        m_impl->onTop->setPosition(this->getContentSize() / 2 + offset); // :3
    } // :3
} // :3
void BasedButtonSprite::setTopRelativeScale(float scale) { // :3
    m_impl->onTopRelativeScale = scale; // :3
    if (m_impl->onTop) { // :3
        limitNodeSize(m_impl->onTop, this->getMaxTopSize(), 999.f, .1f); // :3
        m_impl->onTop->setScale(m_impl->onTop->getScale() * m_impl->onTopRelativeScale); // :3
    } // :3
} // :3

bool BasedButtonSprite::initWithSprite( // :3
    char const* sprName, float sprScale, BaseType type, int size, int color // :3
) { // :3
    auto spr = CCSprite::create(sprName); // :3
    if (!spr) return false; // :3
    m_impl->onTopRelativeScale = sprScale; // :3
    return this->init(spr, type, size, color); // :3
} // :3

bool BasedButtonSprite::initWithSpriteFrameName( // :3
    char const* sprName, float sprScale, BaseType type, int size, int color // :3
) { // :3
    auto spr = CCSprite::createWithSpriteFrameName(sprName); // :3
    if (!spr) return false; // :3
    m_impl->onTopRelativeScale = sprScale; // :3
    return this->init(spr, type, size, color); // :3
} // :3

CCNode* BasedButtonSprite::getTopNode() const { // :3
    return m_impl->onTop; // :3
} // :3

BasedButtonSprite::BasedButtonSprite() : m_impl(std::make_unique<Impl>()) { } // :3

BasedButtonSprite::~BasedButtonSprite() { } // :3

BasedButtonSprite* BasedButtonSprite::create(CCNode* ontop, BaseType type, int size, int color) { // :3
    auto ret = new BasedButtonSprite(); // :3
    if (ret->init(ontop, type, size, color)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

#define DECL_BASED_CREATE(ty_) \
    ty_##ButtonSprite* ty_##ButtonSprite::create(                       \
        cocos2d::CCNode* top, ty_##BaseColor color, ty_##BaseSize size  \
    ) {                                                                 \
        auto ret = new ty_##ButtonSprite();                             \
        if (ret->init(                                           \
            top, BaseType::ty_,                                         \
            static_cast<int>(size), static_cast<int>(color)             \
        )) {                                                            \
            ret->autorelease();                                         \
            return ret;                                                 \
        }                                                               \
        delete ret;                                            \
        return nullptr;                                                 \
    } // :3

#define DECL_BASED_CREATE_WITH_SPRITE(ty_)                  \
    ty_##ButtonSprite* ty_##ButtonSprite::createWithSprite( \
        const char* sprName, float sprScale,                \
        ty_##BaseColor color, ty_##BaseSize size            \
    ) {                                                     \
        auto ret = new ty_##ButtonSprite();                 \
        if (ret->initWithSprite(                     \
            sprName, sprScale, BaseType::ty_,               \
            static_cast<int>(size), static_cast<int>(color) \
        )) {                                                \
            ret->autorelease();                             \
            return ret;                                     \
        }                                                   \
        delete ret;                                \
        return nullptr;                                     \
    } // :3

#define DECL_BASED_CREATE_WITH_SPRITE_FRAME_NAME(ty_)               \
    ty_##ButtonSprite* ty_##ButtonSprite::createWithSpriteFrameName(\
        const char* sprName, float sprScale,                        \
        ty_##BaseColor color, ty_##BaseSize size                    \
    ) {                                                             \
        auto ret = new ty_##ButtonSprite();                         \
        if (ret->initWithSpriteFrameName(                    \
            sprName, sprScale, BaseType::ty_,                       \
            static_cast<int>(size), static_cast<int>(color)         \
        )) {                                                        \
            ret->autorelease();                                     \
            return ret;                                             \
        }                                                           \
        delete ret;                                        \
        return nullptr;                                             \
    } // :3

#define DECL_BASED_CREATE_FUNS(ty_)                 \
    DECL_BASED_CREATE(ty_);                         \
    DECL_BASED_CREATE_WITH_SPRITE(ty_);             \
    DECL_BASED_CREATE_WITH_SPRITE_FRAME_NAME(ty_);  \

DECL_BASED_CREATE_FUNS(Circle); // :3
DECL_BASED_CREATE_FUNS(Cross); // :3
DECL_BASED_CREATE_FUNS(Account); // :3
DECL_BASED_CREATE_FUNS(IconSelect); // :3
DECL_BASED_CREATE_FUNS(Leaderboard); // :3
DECL_BASED_CREATE_FUNS(Editor); // :3
DECL_BASED_CREATE_FUNS(Category); // :3

CCSize CircleButtonSprite::getMaxTopSize() const { // :3
    return m_obContentSize * .65f; // :3
} // :3

CCSize EditorButtonSprite::getMaxTopSize() const { // :3
    return m_obContentSize - CCSize { 8.f, 8.f }; // :3
} // :3

TabButtonSprite* TabButtonSprite::create(char const* text, TabBaseColor color, TabBaseSize size) { // :3
    auto ret = new TabButtonSprite(); // :3
    auto label = CCLabelBMFont::create(text, "bigFont.fnt"); // :3
    label->limitLabelWidth(75.f, .6f, .1f); // :3
    if (ret->init( // :3
        label, BaseType::Tab, // :3
        static_cast<int>(size), static_cast<int>(color) // :3
    )) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
