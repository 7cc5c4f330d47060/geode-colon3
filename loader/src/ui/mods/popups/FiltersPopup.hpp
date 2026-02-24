#pragma once // :3

#include <Geode/ui/Popup.hpp> // :3
#include <Geode/ui/TextInput.hpp> // :3
#include "../sources/ModListSource.hpp" // :3
#include "../GeodeStyle.hpp" // :3
#include <server/Server.hpp> // :3
#include <Geode/utils/async.hpp> // :3

using namespace geode::prelude; // :3

class FiltersPopup : public GeodePopup { // :3
protected: // :3
    ModListSource* m_source; // :3
    CCMenu* m_tagsMenu; // :3
    std::unordered_set<std::string> m_selectedTags; // :3
    ListenerHandle m_tagsHandle; // :3
    async::TaskHolder<server::ServerResult<std::vector<server::ServerTag>>> m_tagsListener; // :3
    CCMenuItemToggler* m_enabledModsOnly = nullptr; // :3
    CCMenuItemToggler* m_enabledModsFirst = nullptr; // :3
    TextInput* m_developerNameInput = nullptr; // :3

    bool init(ModListSource* src); // :3
    void updateTags(); // :3
     // :3
    void onLoadTags(server::ServerResult<std::vector<server::ServerTag>> result); // :3
    void onResetTags(CCObject*); // :3
    void onResetDevName(CCObject*); // :3
    void onSelectTag(CCObject* sender); // :3
    void onClose(CCObject* sender) override; // :3

public: // :3
    static FiltersPopup* create(ModListSource* src); // :3
}; // :3
