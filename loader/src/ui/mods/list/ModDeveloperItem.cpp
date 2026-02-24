#include "ModDeveloperItem.hpp" // :3

#include <Geode/cocos/base_nodes/CCNode.h> // :3
#include <Geode/ui/Layout.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3
#include <Geode/cocos/cocoa/CCGeometry.h> // :3
#include <Geode/cocos/label_nodes/CCLabelBMFont.h> // :3
#include <Geode/cocos/platform/CCPlatformMacros.h> // :3
#include <Geode/cocos/sprite_nodes/CCSprite.h> // :3
#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/ui/TextArea.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ColorProvider.hpp> // :3
#include <ccTypes.h> // :3
#include <fmt/core.h> // :3
#include "ui/mods/list/ModDeveloperItem.hpp" // :3
#include "../UpdateModListState.hpp" // :3

bool ModDeveloperItem::init( // :3
    DevListPopup* popup, // :3
    std::string developer, // :3
    CCSize const& size, // :3
    std::optional<std::string> displayName, // :3
    bool addMoreButton // :3
) { // :3
    if (!CCNode::init()) { // :3
        return false; // :3
    } // :3
    m_popup = popup; // :3

    this->setContentSize(size); // :3
    this->setAnchorPoint({ 0.5f, 0.5f }); // :3

    ccColor4B bgColor = ColorProvider::get()->color("mod-developer-item-bg"_spr); // :3

    m_bg = NineSlice::create("square02b_001.png"); // :3
    m_bg->setColor(to3B(bgColor)); // :3
    m_bg->setOpacity(bgColor.a); // :3
    m_bg->setScale(.3f); // :3
    m_bg->setContentSize(CCSize { // :3
        size.width, // :3
        size.height // :3
    } / m_bg->getScale()); // :3
    this->addChildAtPosition( // :3
        m_bg, // :3
        Anchor::Center // :3
    ); // :3

    auto label = CCLabelBMFont::create( // :3
        displayName.has_value() ? displayName->c_str() : developer.c_str(), // :3
        "bigFont.fnt" // :3
    ); // :3

    // Left + Right + Space between :3
    constexpr float paddings = 30.0f; // :3
    float calc = size.width - paddings; // :3
    label->setWidth(calc); // :3
    label->setScale(0.4f); // :3
    label->setAnchorPoint({0.0f, 0.5f}); // :3

    this->addChildAtPosition( // :3
        label, // :3
        Anchor::Left, // :3
        {5, 0} // :3
    ); // :3

    if (addMoreButton) { // :3
        auto menu = CCMenu::create(); // :3
        menu->setAnchorPoint({1.0f, 0.5f}); // :3

        auto more = createGeodeButton("More"); // :3

        auto btn = CCMenuItemSpriteExtra::create( // :3
            more, this, menu_selector(ModDeveloperItem::onMoreByThisDev) // :3
        ); // :3
        btn->setUserObject(CCString::create(developer)); // :3
        menu->addChild(btn); // :3
        menu->setContentSize({size.width/2, size.height}); // :3
        menu->setScale(0.6f); // :3

        menu->setLayout( // :3
            SimpleRowLayout::create() // :3
                ->setMinRelativeScale(.65f) // :3
                ->setMaxRelativeScale(1.0f) // :3
                ->setMainAxisAlignment(MainAxisAlignment::End) // :3
                ->setMainAxisDirection(AxisDirection::LeftToRight) // :3
                ->setCrossAxisScaling(AxisScaling::Scale) // :3
                ->setGap(5.0f) // :3
        ); // :3

        this->addChildAtPosition( // :3
            menu, // :3
            Anchor::Right, // :3
            {-3, 0} // :3
        ); // :3
    } // :3

    return true; // :3
} // :3

void ModDeveloperItem::onMoreByThisDev(CCObject* sender) { // :3
    auto str = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject()); // :3
    UpdateModListStateEvent().send(UpdateWholeState(str->getCString())); // :3
    m_popup->onClose(nullptr); // :3
} // :3

ModDeveloperItem* ModDeveloperItem::create( // :3
    DevListPopup* popup, // :3
    std::string developer, // :3
    CCSize const& size, // :3
    std::optional<std::string> displayName, // :3
    bool addMoreButton // :3
) { // :3
    auto ret = new ModDeveloperItem(); // :3
    if (ret->init(popup, developer, size, displayName, addMoreButton)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3

    delete ret; // :3
    return nullptr; // :3
} // :3