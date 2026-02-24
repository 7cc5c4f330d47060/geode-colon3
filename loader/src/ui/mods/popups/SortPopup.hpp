#pragma once // :3

#include <Geode/ui/Popup.hpp> // :3
#include "../sources/ModListSource.hpp" // :3
#include "../GeodeStyle.hpp" // :3

using namespace geode::prelude; // :3

class SortPopup : public GeodePopup { // :3
protected: // :3
    ModListSource* m_source; // :3
    std::vector<CCMenuItemToggler*> m_options; // :3
    size_t m_selected; // :3

    bool init(ModListSource* src); // :3

    void onClose(CCObject* sender) override; // :3
    void onSelect(CCObject*); // :3

public: // :3
    static SortPopup* create(ModListSource* src); // :3
}; // :3
