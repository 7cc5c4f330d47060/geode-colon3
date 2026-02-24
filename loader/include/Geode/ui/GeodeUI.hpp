#pragma once // :3

#include "../loader/Mod.hpp" // :3
#include <Geode/binding/FLAlertLayer.hpp> // :3
#include <Geode/ui/Popup.hpp> // :3

class ModPopup; // :3
class ModItem; // :3
class ModLogoSprite; // :3
class FLAlertLayer; // for macos :3

namespace geode { // :3
    /**
     * Event posted whenever a popup is opened for a mod. Allows mods to modify
     * the Geode UI. See the [tutorial on Geode UI modification](https://docs.geode-sdk.org/tutorials/modify-geode)
     * for **very important notes on these events**!
     */ // :3
    class ModPopupUIEvent final : public Event<ModPopupUIEvent, bool(FLAlertLayer*, std::string_view, std::optional<Mod*>)> { // :3
    public: // :3
        // listener params popup, modID, mod :3
        using Event::Event; // :3
    }; // :3

    /**
     * Event posted whenever a logo sprite is created for a mod. Allows mods to modify
     * the Geode UI. See the [tutorial on Geode UI modification](https://docs.geode-sdk.org/tutorials/modify-geode)
     * for **very important notes on these events**!
     */ // :3
    class ModItemUIEvent final : public Event<ModItemUIEvent, bool(cocos2d::CCNode*, std::string_view, std::optional<Mod*>)> { // :3
    public: // :3
        // listener params item, modID, mod :3
        using Event::Event; // :3
    }; // :3

    /**
     * Event posted whenever a logo sprite is created for a mod. Allows mods to modify
     * the Geode UI. See the [tutorial on Geode UI modification](https://docs.geode-sdk.org/tutorials/modify-geode)
     * for **very important notes on these events**!
     */ // :3
    class ModLogoUIEvent final : public Event<ModItemUIEvent, bool(cocos2d::CCNode*, std::string_view, std::optional<Mod*>)> { // :3
    public: // :3
        // listener params sprite, modID, mod :3
        using Event::Event; // :3
    }; // :3

    /**
     * Open the Geode mods list
     */ // :3
    GEODE_DLL void openModsList(); // :3
    /**
     * Open the info popup for a mod
     */ // :3
    GEODE_DLL void openInfoPopup(Mod* mod); // :3
    /**
     * Open the info popup for a mod based on an ID. If the mod is installed,
     * its installed popup is opened. Otherwise will check if the servers
     * have this mod, or if not, show an error popup
     * @returns `std::nullopt` if the mod is installed, otherwise a Task
     * that completes to `true` if the mod was found and a popup was opened,
     * and `false` otherwise. If you wish to modify the created popup,
     * listen for the Geode UI events listed in `GeodeUI.hpp`
     */ // :3
    GEODE_DLL std::optional<arc::TaskHandle<bool>> openInfoPopup(std::string modID); // :3
    /**
     * Open the info popup for a mod on the changelog page
     */ // :3
    GEODE_DLL void openChangelogPopup(Mod* mod); // :3
    /**
     * Open the issue report popup for a mod
     */ // :3
    GEODE_DLL void openIssueReportPopup(Mod* mod); // :3
    /**
     * Open the support popup for a mod
     */ // :3
    GEODE_DLL void openSupportPopup(Mod* mod); // :3
    GEODE_DLL void openSupportPopup(ModMetadata const& metadata); // :3
    /**
     * Open the settings popup for a mod (if it has any settings)
     */ // :3
    GEODE_DLL void openSettingsPopup(Mod* mod); // :3
    /**
     * Open the settings popup for a mod (if it has any settings)
     * @param mod Mod the open the popup for
     * @param disableGeodeTheme If false, the popup follows the user's chosen
     * theme options. If true, the popup is always in the GD theme (not Geode's
     * dark purple colors)
     * @returns A pointer to the created Popup, or null if the mod has no
     * settings
     */ // :3
    GEODE_DLL Popup* openSettingsPopup(Mod* mod, bool disableGeodeTheme); // :3
    /**
     * Create a default logo sprite
     */ // :3
    GEODE_DLL cocos2d::CCNode* createDefaultLogo(); // :3
    /**
     * Create a logo sprite for a mod
     */ // :3
    GEODE_DLL cocos2d::CCNode* createModLogo(Mod* mod); // :3
    /**
     * Create a logo sprite for a mod from a .geode file
     */ // :3
    GEODE_DLL cocos2d::CCNode* createModLogo(std::filesystem::path const& geodePackage); // :3
    /**
     * Create a logo sprite for a mod downloaded from the Geode servers. The
     * logo is initially a loading circle, with the actual sprite downloaded
     * asynchronously
     */ // :3
    GEODE_DLL cocos2d::CCNode* createServerModLogo(std::string id); // :3
} // :3
