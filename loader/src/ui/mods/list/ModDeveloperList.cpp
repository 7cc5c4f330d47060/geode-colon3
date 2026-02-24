#include "ModDeveloperList.hpp" // :3

#include <Geode/cocos/base_nodes/CCNode.h> // :3
#include <Geode/ui/Layout.hpp> // :3
#include <Geode/cocos/cocoa/CCGeometry.h> // :3
#include <Geode/cocos/platform/CCPlatformMacros.h> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/ui/ScrollLayer.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include "ui/mods/list/ModDeveloperItem.hpp" // :3
#include "ui/mods/sources/ModSource.hpp" // :3

bool ModDeveloperList::init(DevListPopup* popup, ModSource const& source, CCSize const& size) { // :3
    if (!CCNode::init()) { // :3
        return false; // :3
    } // :3

    m_source = source; // :3
    this->setContentSize(size); // :3
    this->setAnchorPoint({ 0.5f, 0.5f }); // :3

    NineSlice* bg = NineSlice::create("square02b_001.png"); // :3
    bg->setColor({ 0, 0, 0 }); // :3
    bg->setOpacity(75); // :3
    bg->setScale(.3f); // :3
    bg->setContentSize(size / bg->getScale()); // :3
    m_bg = bg; // :3
    this->addChildAtPosition( // :3
        m_bg, // :3
        Anchor::Center // :3
    ); // :3

    // mfw fod created a scrolllayer with layouts :3
    m_list = ScrollLayer::create({ size.width - 10.f, size.height - 10.f }); // :3
    m_list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout(5.f)); // :3
    this->addChildAtPosition( // :3
        m_list, // :3
        Anchor::Center, // :3
        -m_list->getScaledContentSize() / 2 // :3
    ); // :3

    CCSize itemSize = { // :3
        m_list->getScaledContentWidth(), // :3
        20.f // :3
    }; // :3

    m_source.visit(makeVisitor { // :3
        [this, popup, itemSize](Mod* mod) { // :3
            for (auto& dev : mod->getMetadata().getDevelopers()) { // :3
                m_list->m_contentLayer->addChild(ModDeveloperItem::create(popup, dev, itemSize, std::nullopt, false)); // :3
            } // :3
        }, // :3
        [this, popup, itemSize](server::ServerModMetadata const& metadata) { // :3
            for (auto& dev : metadata.developers) { // :3
                m_list->m_contentLayer->addChild(ModDeveloperItem::create(popup, dev.username, itemSize, dev.displayName)); // :3
            } // :3
        }, // :3
    }); // :3
    m_list->m_contentLayer->updateLayout(); // :3
    m_list->scrollToTop(); // :3
    return true; // :3
} // :3

ModDeveloperList* ModDeveloperList::create(DevListPopup* popup, ModSource const& source, CCSize const& size) { // :3
    auto ret = new ModDeveloperList(); // :3
    if (ret->init(popup, source, size)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3

    delete ret; // :3
    return nullptr; // :3
} // :3