#pragma once // :3

#include "ModListItem.hpp" // :3
#include "../sources/ModListSource.hpp" // :3

class SpecialModListItem : public ModListItem { // :3
protected: // :3
    SpecialModListItemSource m_item; // :3

    bool init(SpecialModListItemSource&& item); // :3

public: // :3
    static SpecialModListItem* create(SpecialModListItemSource&& item); // :3
}; // :3
