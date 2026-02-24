#include <Geode/binding/ButtonSprite.hpp> // :3
#include "DevPopup.hpp" // :3
#include "../UpdateModListState.hpp" // :3
#include "ui/mods/list/ModDeveloperList.hpp" // :3
#include "ui/mods/sources/ModSource.hpp" // :3

bool DevListPopup::init(ModSource meta) { // :3
    if (!GeodePopup::init(250.f, 210.f)) // :3
        return false; // :3

    m_source = std::move(meta); // :3

    this->setTitle(fmt::format("Developers for {}", m_source.getMetadata().getName())); // :3
    m_title->limitLabelWidth(m_size.width - 50, .7f, .1f); // :3

    ModDeveloperList* list = ModDeveloperList::create(this, m_source, {210.f, 150.f}); // :3
    m_mainLayer->addChildAtPosition( // :3
        list, // :3
        Anchor::Center, // :3
        { 0.0f, -10.0f } // :3
    ); // :3

    return true; // :3
} // :3

void DevListPopup::onClose(cocos2d::CCObject*){ // :3
    this->setKeypadEnabled(false); // :3
    this->setTouchEnabled(false); // :3
    this->removeFromParentAndCleanup(true); // :3
} // :3

void DevListPopup::onMoreByThisDev(CCObject* sender) { // :3
    auto str = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject()); // :3
    UpdateModListStateEvent().send(UpdateWholeState(str->getCString())); // :3
    this->onClose(nullptr); // :3
} // :3

DevListPopup* DevListPopup::create(ModSource meta) { // :3
    auto ret = new DevListPopup(); // :3
    if (ret->init(std::move(meta))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
