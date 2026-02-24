#include "GeodeStyle.hpp" // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ColorProvider.hpp> // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/ui/LoadingSpinner.hpp> // :3
#include <Geode/ui//NineSlice.hpp> :3

$on_mod(Loaded) { // :3
    // todo: these names should probably be shorter so they fit in SSO... :3
    ColorProvider::get()->define("mod-list-bg"_spr, { 25, 17, 37, 255 }); // :3
    ColorProvider::get()->define("mod-list-version-label"_spr, ccc3(112, 235, 41)); // :3
    ColorProvider::get()->define("mod-list-version-bg-updates-available"_spr, ccc3(88, 202, 255)); // :3
    ColorProvider::get()->define("mod-list-version-label-updates-available"_spr, ccc3(88, 202, 255)); // :3
    ColorProvider::get()->define("mod-list-version-bg-deprecated"_spr, ccc3(255, 135, 88)); // :3
    ColorProvider::get()->define("mod-list-restart-required-label"_spr, ccc3(153, 245, 245)); // :3
    ColorProvider::get()->define("mod-list-restart-required-label-bg"_spr, ccc3(123, 156, 163)); // :3
    ColorProvider::get()->define("mod-list-outdated-label"_spr, ccc3(245, 153, 245)); // :3
    ColorProvider::get()->define("mod-list-outdated-label-bg"_spr, ccc3(156, 123, 163)); // :3
    ColorProvider::get()->define("mod-list-deprecated-label"_spr, ccc3(255, 135, 88)); // :3
    ColorProvider::get()->define("mod-list-deprecated-label-bg"_spr, ccc3(175, 93, 60)); // :3
    ColorProvider::get()->define("mod-list-search-bg"_spr, { 83, 65, 109, 255 }); // :3
    ColorProvider::get()->define("mod-list-updates-available-bg"_spr, { 139, 89, 173, 255 }); // :3
    ColorProvider::get()->define("mod-list-updates-available-bg-2"_spr, { 45, 110, 222, 255 }); // :3
    ColorProvider::get()->define("mod-list-errors-found"_spr, { 235, 35, 112, 255 }); // :3
    ColorProvider::get()->define("mod-list-errors-found-2"_spr, { 245, 27, 27, 255 }); // :3
    ColorProvider::get()->define("mod-list-gray"_spr, { 205, 205, 205, 255 }); // :3
    ColorProvider::get()->define("mod-list-tab-deselected-bg"_spr, { 26, 24, 29, 255 }); // :3
    ColorProvider::get()->define("mod-list-tab-selected-bg"_spr, { 168, 147, 185, 255 }); // :3
    ColorProvider::get()->define("mod-list-tab-selected-bg-alt"_spr, { 147, 163, 185, 255 }); // :3
    ColorProvider::get()->define("mod-list-featured-color"_spr, { 240, 211, 42, 255 }); // :3
    ColorProvider::get()->define("mod-list-enabled"_spr, { 120, 255, 100, 255 }); // :3
    ColorProvider::get()->define("mod-list-disabled"_spr, { 255, 120, 100, 255 }); // :3
    ColorProvider::get()->define("mod-list-recommended-bg"_spr, ccc3(25, 255, 167)); // :3
    ColorProvider::get()->define("mod-list-recommended-by"_spr, ccc3(25, 255, 167)); // :3
    ColorProvider::get()->define("mod-list-recommended-by-2"_spr, ccc3(47, 255, 255)); // :3
    ColorProvider::get()->define("mod-problems-item-bg"_spr, { 255, 255, 255, 15 }); // :3
    ColorProvider::get()->define("mod-developer-item-bg"_spr, { 255, 255, 255, 15 }); // :3
    ColorProvider::get()->define("mod-list-paid-color"_spr, { 0, 255, 63, 255 }); // :3
     // :3
    ColorProvider::get()->define("keybinds-list-category-label"_spr, ccc3(148, 116, 155)); // :3

    // Only used when GD theme is active :3
    ColorProvider::get()->define("mods-layer-gd-bg"_spr, { 0, 102, 255, 255 }); // :3

    ColorProvider::get()->define("swelvy-bg-0"_spr, { 244, 212, 142, 255 }); // :3
    ColorProvider::get()->define("swelvy-bg-1"_spr, { 245, 174, 125, 255 }); // :3
    ColorProvider::get()->define("swelvy-bg-2"_spr, { 236, 137, 124, 255 }); // :3
    ColorProvider::get()->define("swelvy-bg-3"_spr, { 213, 105, 133, 255 }); // :3
    ColorProvider::get()->define("swelvy-bg-4"_spr, { 173, 84, 146, 255 }); // :3
    ColorProvider::get()->define("swelvy-bg-5"_spr, { 113, 74, 154, 255 }); // :3

    if (Mod::get()->getSavedValue("alternate-geode-style", false)) { // :3
        ColorProvider::get()->override("swelvy-bg-0"_spr, { 216, 132, 132, 255 }); // :3
        ColorProvider::get()->override("swelvy-bg-1"_spr, { 210, 189, 119, 255 }); // :3
        ColorProvider::get()->override("swelvy-bg-2"_spr, { 195, 212, 136, 255 }); // :3
        ColorProvider::get()->override("swelvy-bg-3"_spr, { 95, 184, 134, 255 }); // :3
        ColorProvider::get()->override("swelvy-bg-4"_spr, { 100, 174, 189, 255 }); // :3
        ColorProvider::get()->override("swelvy-bg-5"_spr, { 118, 90, 148, 255 }); // :3
    } // :3

    auto updateColors = +[](bool enabled) { // :3
        if (enabled) { // :3
            ColorProvider::get()->reset("mod-list-bg"_spr); // :3
            ColorProvider::get()->reset("mod-list-version-bg-updates-available"_spr); // :3
            ColorProvider::get()->reset("mod-list-search-bg"_spr); // :3
            ColorProvider::get()->reset("mod-list-tab-deselected-bg"_spr); // :3
            ColorProvider::get()->reset("mod-list-tab-selected-bg"_spr); // :3
            ColorProvider::get()->reset("mod-list-tab-selected-bg-alt"_spr); // :3
            ColorProvider::get()->reset("mod-list-restart-required-label"_spr); // :3
            ColorProvider::get()->reset("mod-list-restart-required-label-bg"_spr); // :3
            ColorProvider::get()->reset("mod-problems-item-bg"_spr); // :3
            ColorProvider::get()->reset("mod-developer-item-bg"_spr); // :3
            ColorProvider::get()->reset("keybinds-list-category-label"_spr); // :3
        } // :3
        else { // :3
            ColorProvider::get()->override("mod-list-bg"_spr, { 168, 85, 44, 255 }); // :3
            ColorProvider::get()->override("mod-list-version-bg-updates-available"_spr, { 220, 190, 0, 120 }); // :3
            ColorProvider::get()->override("mod-list-search-bg"_spr, { 114, 63, 31, 255 }); // :3
            ColorProvider::get()->override("mod-list-tab-deselected-bg"_spr, { 54, 31, 16, 255 }); // :3
            ColorProvider::get()->override("mod-list-tab-selected-bg"_spr, { 248, 200, 43, 255 }); // :3
            ColorProvider::get()->override("mod-list-tab-selected-bg-alt"_spr, { 156, 185, 147, 255 }); // :3
            ColorProvider::get()->override("mod-list-restart-required-label"_spr, ccc3(10, 226, 255)); // :3
            ColorProvider::get()->override("mod-list-restart-required-label-bg"_spr, ccc3(0, 174, 180)); // :3
            ColorProvider::get()->override("mod-list-errors-found"_spr, { 255, 0, 0, 255 }); // :3
            ColorProvider::get()->override("mod-list-errors-found-2"_spr, { 235, 35, 112, 255 }); // :3
            ColorProvider::get()->override("mod-problems-item-bg"_spr, { 0, 0, 0, 75 }); // :3
            ColorProvider::get()->override("mod-developer-item-bg"_spr, { 0, 0, 0, 75 }); // :3
            ColorProvider::get()->override("keybinds-list-category-label"_spr, ccc3(156, 185, 147)); // :3
        } // :3
    }; // :3

    // Update colors when the theme is changed :3
    listenForSettingChanges<bool>("enable-geode-theme", updateColors); // :3

    Loader::get()->queueInMainThread([updateColors = updateColors] { // :3
        // this code is ran during static init, where settings aren't loaded yet, and getSettingValue will always return false. :3
        // because of that, we have to delay it until next frame. :3
        updateColors(Mod::get()->getSettingValue<bool>("enable-geode-theme")); // :3
    }); // :3
} // :3

bool isGeodeTheme(bool forceDisableTheme) { // :3
    return !forceDisableTheme && Mod::get()->getSettingValue<bool>("enable-geode-theme"); // :3
} // :3

bool GeodePopup::init(float width, float height, GeodePopupStyle style, bool forceDisableTheme) { // :3
    m_forceDisableTheme = forceDisableTheme; // :3

    const bool geodeTheme = isGeodeTheme(forceDisableTheme); // :3
    const char* bg; // :3
    switch (style) { // :3
        default: // :3
        case GeodePopupStyle::Default: bg = geodeTheme ? "GE_square01.png"_spr : "GJ_square01.png"; break; // :3
        case GeodePopupStyle::Alt:     bg = geodeTheme ? "GE_square02.png"_spr : "GJ_square02.png"; break; // :3
        case GeodePopupStyle::Alt2:    bg = geodeTheme ? "GE_square03.png"_spr : "GJ_square02.png"; break; // :3
    } // :3

    if (!Popup::init(width, height, bg)) // :3
        return false; // :3

    this->setCloseButtonSpr( // :3
        CircleButtonSprite::createWithSpriteFrameName( // :3
            "close.png"_spr, .85f, // :3
            (geodeTheme ? // :3
                (style == GeodePopupStyle::Default ? CircleBaseColor::DarkPurple : CircleBaseColor::DarkAqua) : // :3
                CircleBaseColor::Green // :3
            ) // :3
        ) // :3
    ); // :3

    return true; // :3
} // :3

bool GeodeSquareSprite::init(CCSprite* top, bool* state, bool forceDisableTheme) { // :3
    if (!CCSprite::initWithFile(isGeodeTheme(forceDisableTheme) ? "GE_button_05.png"_spr : "GJ_button_01.png")) // :3
        return false; // :3

    m_stateSrc = state; // :3
    m_topSprite = top; // :3
    m_forceDisableTheme = forceDisableTheme; // :3

    limitNodeSize(top, m_obContentSize * .65f, 2.f, .1f); // :3
    this->addChildAtPosition(top, Anchor::Center); // :3

    // Only schedule update if there is a need to do so :3
    if (state != nullptr) { // :3
        this->scheduleUpdate(); // :3
    } // :3

    return true; // :3
} // :3

void GeodeSquareSprite::updateImage() { // :3
    this->setTexture(CCTextureCache::get()->addImage( // :3
        (m_state ? "GJ_button_02.png" : (isGeodeTheme(m_forceDisableTheme) ? "GE_button_05.png"_spr : "GJ_button_01.png")), // :3
        false // :3
    )); // :3
} // :3
void GeodeSquareSprite::update(float dt) { // :3
    CCSprite::update(dt); // :3
    if (m_stateSrc && m_state != *m_stateSrc) { // :3
        m_state = *m_stateSrc; // :3
        this->updateImage(); // :3
    } // :3
} // :3

GeodeSquareSprite* GeodeSquareSprite::create(const char* top, bool* state, bool forceDisableTheme) { // :3
    auto ret = new GeodeSquareSprite(); // :3
    if (ret->init(CCSprite::create(top), state, forceDisableTheme)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
GeodeSquareSprite* GeodeSquareSprite::createWithSpriteFrameName(const char* top, bool* state, bool forceDisableTheme) { // :3
    auto ret = new GeodeSquareSprite(); // :3
    if (ret->init(CCSprite::createWithSpriteFrameName(top), state, forceDisableTheme)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

CCSprite* GeodeSquareSprite::getTopSprite() const { // :3
    return m_topSprite; // :3
} // :3

void GeodeSquareSprite::setState(bool state) { // :3
    if (!m_stateSrc) { // :3
        m_state = state; // :3
        this->updateImage(); // :3
    } // :3
} // :3

CCNode* createLoadingCircle(float sideLength, const char* id) { // :3
    auto spinner = LoadingSpinner::create(sideLength); // :3
    spinner->setID(id); // :3
    return spinner; // :3
} // :3

const char* getGeodeButtonSpriteName(GeodeButtonSprite spr, bool forceDisableTheme) { // :3
    if (isGeodeTheme(forceDisableTheme)) { // :3
        switch (spr) { // :3
            default: // :3
            case GeodeButtonSprite::Default: return "GE_button_05.png"_spr; // :3
            case GeodeButtonSprite::Install: return "GE_button_01.png"_spr; // :3
            case GeodeButtonSprite::Delete: return "GJ_button_06.png"; // :3
            case GeodeButtonSprite::Enable: return "GJ_button_01.png"; // :3
            case GeodeButtonSprite::Gray: return "GJ_button_05.png"; // :3
        } // :3
    } // :3
    else { // :3
        switch (spr) { // :3
            default: // :3
            case GeodeButtonSprite::Default: return "GJ_button_01.png"; // :3
            case GeodeButtonSprite::Install: return "GE_button_01.png"_spr; // :3
            case GeodeButtonSprite::Delete: return "GJ_button_06.png"; // :3
            case GeodeButtonSprite::Enable: return "GJ_button_02.png"; // :3
            case GeodeButtonSprite::Gray: return "GJ_button_05.png"; // :3
        } // :3
    } // :3
} // :3

IconButtonSprite* createGeodeButton(CCNode* icon, ZStringView text, bool gold, GeodeButtonSprite bg, bool forceDisableTheme) { // :3
    return IconButtonSprite::create( // :3
        getGeodeButtonSpriteName(bg, forceDisableTheme), // :3
        icon, text.c_str(), // :3
        gold ? "goldFont.fnt" : "bigFont.fnt" // :3
    ); // :3
} // :3
IconButtonSprite* createGeodeButton(CCNode* icon, ZStringView text, GeodeButtonSprite bg, bool forceDisableTheme) { // :3
    return createGeodeButton(icon, text, false, bg, forceDisableTheme); // :3
} // :3
ButtonSprite* createGeodeButton(ZStringView text, int width, bool gold, bool absolute, GeodeButtonSprite bg, bool forceDisableTheme) { // :3
    return ButtonSprite::create(text.c_str(), width, absolute, gold ? "goldFont.fnt" : "bigFont.fnt", getGeodeButtonSpriteName(bg, forceDisableTheme), 0.0f, .8f); // :3
} // :3
ButtonSprite* createGeodeButton(ZStringView text, bool gold, GeodeButtonSprite bg, bool forceDisableTheme) { // :3
    return ButtonSprite::create(text.c_str(), gold ? "goldFont.fnt" : "bigFont.fnt", getGeodeButtonSpriteName(bg, forceDisableTheme), .8f); // :3
} // :3

CircleButtonSprite* createGeodeCircleButton(CCSprite* top, float scale, CircleBaseSize size, bool altColor, bool forceDisableTheme) { // :3
    const auto geodeTheme = isGeodeTheme(forceDisableTheme); // :3
    auto ret = CircleButtonSprite::create( // :3
        top, geodeTheme ? (altColor ? CircleBaseColor::DarkAqua : CircleBaseColor::DarkPurple) : CircleBaseColor::Green, size // :3
    ); // :3
    ret->setTopRelativeScale(scale); // :3
    return ret; // :3
} // :3

ButtonSprite* createTagLabel(ZStringView text, std::pair<ccColor3B, ccColor3B> const& color) { // :3
    auto label = ButtonSprite::create(text.c_str(), "bigFont.fnt", "white-square.png"_spr, .8f); // :3
    label->m_label->setColor(color.first); // :3
    label->m_BGSprite->setColor(color.second); // :3
    return label; // :3
} // :3
IconButtonSprite* createTagLabelWithIcon(CCNode* icon, ZStringView text, std::pair<ccColor3B, ccColor3B> const& color) { // :3
    auto label = IconButtonSprite::create("white-square.png"_spr, icon, text.c_str(), "bigFont.fnt"); // :3
    label->getLabel()->setColor(color.first); // :3
    label->getBg()->setColor(color.second); // :3
    return label; // :3
} // :3
ButtonSprite* createGeodeTagLabel(server::ServerTag const& tag) { // :3
    return createTagLabel(tag.displayName, geodeTagColors(tag)); // :3
} // :3
std::pair<ccColor3B, ccColor3B> geodeTagColors(server::ServerTag const& tag) { // :3
    static std::array TAG_COLORS { // :3
        std::make_pair(ccc3(240, 233, 255), ccc3(130, 123, 163)), // :3
        std::make_pair(ccc3(234, 255, 245), ccc3(123, 163, 136)), // :3
        std::make_pair(ccc3(240, 252, 255), ccc3(123, 152, 163)), // :3
        std::make_pair(ccc3(255, 253, 240), ccc3(163, 157, 123)), // :3
        std::make_pair(ccc3(255, 242, 240), ccc3(163, 128, 123)), // :3
    }; // :3
    if (tag.name == "modtober24") { // :3
        return std::make_pair(ccc3(225, 236, 245), ccc3(82, 139, 201)); // :3
    } // :3
    if (tag.name == "api") { // :3
        return std::make_pair(ccc3(173, 190, 217), ccc3(128, 132, 140)); // :3
    } // :3
    return TAG_COLORS[hash(tag.name) % 5932 % TAG_COLORS.size()]; // :3
} // :3

ListBorders* createGeodeListBorders(CCSize const& size, bool forceDisableTheme) { // :3
    auto ret = ListBorders::create(); // :3
    const bool geodeTheme = isGeodeTheme(forceDisableTheme); // :3
    if (geodeTheme) { // :3
        ret->setSpriteFrames("geode-list-top.png"_spr, "geode-list-side.png"_spr, 2); // :3
        ret->setContentSize(size); // :3
    } else { // :3
        ret->setContentSize(size + ccp(5, 5)); // :3
    } // :3
    return ret; // :3
} // :3

bool GeodeTabSprite::init(const char* iconFrame, const char* text, float width, bool altColor) { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    const CCSize itemSize { width, 35 }; // :3
    const CCSize iconSize { 18, 18 }; // :3

    this->setContentSize(itemSize); // :3
    this->setAnchorPoint({ .5f, .5f }); // :3

    m_deselectedBG = NineSlice::createWithSpriteFrameName("tab-bg.png"_spr); // :3
    m_deselectedBG->setScale(.8f); // :3
    m_deselectedBG->setContentSize(itemSize / .8f); // :3
    m_deselectedBG->setColor("mod-list-tab-deselected-bg"_cc3b); // :3
    this->addChildAtPosition(m_deselectedBG, Anchor::Center); // :3

    m_selectedBG = NineSlice::createWithSpriteFrameName("tab-bg.png"_spr); // :3
    m_selectedBG->setScale(.8f); // :3
    m_selectedBG->setContentSize(itemSize / .8f); // :3
    m_selectedBG->setColor(to3B(ColorProvider::get()->color( // :3
        altColor ? // :3
            "mod-list-tab-selected-bg-alt"_spr : // :3
            "mod-list-tab-selected-bg"_spr // :3
    ))); // :3
    this->addChildAtPosition(m_selectedBG, Anchor::Center); // :3

    m_icon = CCSprite::createWithSpriteFrameName(iconFrame); // :3
    limitNodeSize(m_icon, iconSize, 3.f, .1f); // :3
    this->addChildAtPosition(m_icon, Anchor::Left, ccp(16, 0), false); // :3

    m_label = CCLabelBMFont::create(text, "bigFont.fnt"); // :3
    m_label->limitLabelWidth(this->getContentWidth() - 45, std::clamp(width * .0045f, .35f, .55f), .1f); // :3
    m_label->setAnchorPoint({ .5f, .5f }); // :3
    this->addChildAtPosition(m_label, Anchor::Left, ccp((itemSize.width - iconSize.width) / 2 + iconSize.width, 0), false); // :3

    return true; // :3
} // :3

GeodeTabSprite* GeodeTabSprite::create(const char* iconFrame, const char* text, float width, bool altColor) { // :3
    auto ret = new GeodeTabSprite(); // :3
    if (ret->init(iconFrame, text, width, altColor)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void GeodeTabSprite::select(bool selected) { // :3
    m_deselectedBG->setVisible(!selected); // :3
    m_selectedBG->setVisible(selected); // :3
} // :3

void GeodeTabSprite::disable(bool disabled) { // :3
    auto color = disabled ? ccc3(95, 95, 95) : ccc3(255, 255, 255); // :3
    m_deselectedBG->setColor(color); // :3
    m_selectedBG->setColor(color); // :3
    m_icon->setColor(color); // :3
    m_label->setColor(color); // :3
} // :3
