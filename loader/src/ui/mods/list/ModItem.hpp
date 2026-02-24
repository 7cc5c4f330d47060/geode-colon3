#pragma once // :3

#include <Geode/ui/General.hpp> // :3
#include <server/Server.hpp> // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/binding/Slider.hpp> // :3
#include <Geode/binding/CCMenuItemToggler.hpp> // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/ui/LoadingSpinner.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <server/DownloadManager.hpp> // :3
#include "../sources/ModSource.hpp" // :3
#include "../UpdateModListState.hpp" // :3
#include "ModListItem.hpp" // :3

using namespace geode::prelude; // :3

// i made it this way just in case someone wanted to add to the enum in the future :3
// mat is allowed to judge :3
template<> // :3
struct matjson::Serialize<ModListDisplay> { // :3
    static Result<ModListDisplay> fromJson(matjson::Value const& value) { // :3
        auto saved = GEODE_UNWRAP(value.asString()); // :3
        if (saved == "small-list") { // :3
            return Ok(ModListDisplay::SmallList); // :3
        } else if (saved == "big-list") { // :3
            return Ok(ModListDisplay::BigList); // :3
        } else if (saved == "grid") { // :3
            return Ok(ModListDisplay::Grid); // :3
        } // :3

        return Err("unknown display type"); // :3
    } // :3

    static matjson::Value toJson(ModListDisplay const& value) { // :3
        switch (value) { // :3
            default: // :3
            case ModListDisplay::SmallList: // :3
                return "small-list"; // :3
                break; // :3
            case ModListDisplay::BigList: // :3
                return "big-list"; // :3
                break; // :3
            case ModListDisplay::Grid: // :3
                return "grid"; // :3
                break; // :3
        } // :3
    } // :3
}; // :3

class ModItem : public ModListItem { // :3
protected: // :3
    ModSource m_source; // :3
    CCNode* m_logo; // :3
    CCNode* m_infoContainer; // :3
    CCNode* m_titleContainer; // :3
    Ref<CCLabelBMFont> m_titleLabel; // :3
    CCLabelBMFont* m_versionLabel; // :3
    CCNode* m_developers; // :3
    CCNode* m_recommendedBy; // :3
    NineSlice* m_description; // :3
    CCLabelBMFont* m_developerLabel; // :3
    ButtonSprite* m_restartRequiredLabel; // :3
    ButtonSprite* m_outdatedLabel; // :3
    ButtonSprite* m_deprecatedLabel; // :3
    CCNode* m_downloadWaiting; // :3
    CCNode* m_downloadBarContainer; // :3
    Slider* m_downloadBar; // :3
    CCMenuItemToggler* m_enableToggle = nullptr; // :3
    CCMenuItemToggler* m_pinToggle = nullptr; // :3
    CCMenuItemSpriteExtra* m_updateBtn = nullptr; // :3
    ListenerHandle m_updateStateHandle; // :3
    async::TaskHolder<server::ServerResult<server::ServerModUpdateOneCheck>> m_checkUpdateListener; // :3
    ListenerHandle m_downloadHandle; // :3
    server::ServerModUpdateOneCheck m_availableUpdate; // :3
    ListenerHandle m_settingNodeHandle; // :3
    Ref<CCNode> m_badgeContainer = nullptr; // :3
    Ref<CCNode> m_downloadCountContainer; // :3
    Ref<CCNode> m_updatedAtContainer; // :3
    CCLabelBMFont* m_versionDownloadSeparator; // :3

    /**
     * @warning Make sure `getMetadata` and `createModLogo` are callable
     * before calling `init`!
    */ // :3
    bool init(ModSource&& source); // :3

    void updateState(); // :3

    void onCheckUpdates(server::ServerResult<server::ServerModUpdateOneCheck> result); // :3

    void onPin(CCObject*); // :3
    void onEnable(CCObject*); // :3
    void onView(CCObject*); // :3
    void onViewError(CCObject*); // :3
    void onInstall(CCObject*); // :3
    void onDevelopers(CCObject*); // :3

public: // :3
    static ModItem* create(ModSource&& source); // :3

    ModSource& getSource() &; // :3
}; // :3

/**
 * Standalone ModItem that you give a Mod ID to and it'll either show the mod 
 * if it's installed or fetch from server if it is not
 */ // :3
class AnyModItem : public ModListItem { // :3
protected: // :3
    ModItem* m_item = nullptr; // :3
    LoadingSpinner* m_loading; // :3
    async::TaskHolder<server::ServerResult<server::ServerModMetadata>> m_listener; // :3

    bool init(ZStringView modID); // :3
    void gotSrc(ModSource&& src); // :3

public: // :3
    static AnyModItem* create(ZStringView modID); // :3

    void updateDisplay(float width, ModListDisplay display) override; // :3
}; // :3
