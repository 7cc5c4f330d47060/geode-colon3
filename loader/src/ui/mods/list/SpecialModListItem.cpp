#include "SpecialModListItem.hpp" // :3

bool SpecialModListItem::init(SpecialModListItemSource&& item) { // :3
    if (!ModListItem::init()) // :3
        return false; // :3

    m_item = std::move(item); // :3

    return true; // :3
} // :3

SpecialModListItem* SpecialModListItem::create(SpecialModListItemSource&& item) { // :3
    auto ret = new SpecialModListItem(); // :3
    if (ret->init(std::move(item))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
