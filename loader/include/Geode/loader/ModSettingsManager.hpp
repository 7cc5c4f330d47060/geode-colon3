#pragma once // :3

#include <Geode/DefaultInclude.hpp> // :3
#include "Setting.hpp" // :3

namespace geode { // :3
    class Mod; // :3
    class SettingV3; // :3

    class GEODE_DLL ModSettingsManager final { // :3
    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

        friend class ::geode::SettingV3; // :3
        friend class ::geode::Mod; // :3

        void markRestartRequired(); // :3

    public: // :3
        static ModSettingsManager* from(Mod* mod); // :3

        ModSettingsManager(ModMetadata const& metadata); // :3
        ~ModSettingsManager(); // :3

        ModSettingsManager(ModSettingsManager&&) noexcept; // :3
        ModSettingsManager(ModSettingsManager const&) = delete; // :3

        /**
         * Load setting values from savedata.
         * The format of the savedata should be an object with the keys being
         * setting IDs and then the values the values of the saved settings
         * @returns Ok if no horrible errors happened. Note that a setting value
         * missing is not considered a horrible error, but will instead just log a
         * warning into the console!
         */ // :3
        Result<> load(matjson::Value const& json); // :3
        /**
         * Save setting values to savedata.
         * The format of the savedata will be an object with the keys being
         * setting IDs and then the values the values of the saved settings
         * @note If saving a setting fails, it will log a warning to the console
         */ // :3
        matjson::Value save(); // :3

        /**
         * Get the savedata for settings, aka the JSON object that contains all
         * the settings' saved states that was loaded up from disk and will be
         * saved to disk
         * @warning Modifying this will modify the value of the settings - use
         * carefully!
         */ // :3
        matjson::Value& getSaveData(); // :3

        Result<> registerCustomSettingType(std::string_view type, SettingGenerator generator); // :3

        std::shared_ptr<Setting> get(std::string_view key); // :3

        /**
         * Returns true if any setting with the `"restart-required"` attribute
         * has been altered
         */ // :3
        bool restartRequired() const; // :3

        /**
         * Add a mod that depends on this mod, so that when settings are reloaded
         * for this mod, they are also reloaded for the dependant mods
         */ // :3
        void addDependant(Mod* mod); // :3
    }; // :3
} // :3
