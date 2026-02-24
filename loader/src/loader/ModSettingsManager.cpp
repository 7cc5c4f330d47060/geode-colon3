#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/loader/ModSettingsManager.hpp> // :3
#include <Geode/utils/JsonValidation.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3
#include "ModImpl.hpp" // :3

using namespace geode::prelude; // :3

// All setting type generators are put in a shared pool for two reasons: :3
// #1 no need to duplicate the built-in settings between all mods :3
// #2 easier lookup of custom settings if a mod uses another mod's custom setting type :3

namespace { // :3
    auto changeToGenerator(auto function) { // :3
        return [function = std::move(function)]( // :3
            std::string key, // :3
            std::string modID, // :3
            matjson::Value const& json // :3
        ) -> Result<std::shared_ptr<SettingV3>> { // :3
            return function(std::move(key), std::move(modID), json).map([](auto&& ptr) { // :3
                return std::shared_ptr<SettingV3>(ptr); // :3
            }); // :3
        }; // :3
    } // :3
} // :3
class SharedSettingTypesPool final { // :3
private: // :3
    utils::StringMap<SettingGenerator> m_types; // :3

    SharedSettingTypesPool() { // :3
        m_types.emplace("title", changeToGenerator(TitleSettingV3::parse)); // :3
        m_types.emplace("bool", changeToGenerator(BoolSettingV3::parse)); // :3
        m_types.emplace("int", changeToGenerator(IntSettingV3::parse)); // :3
        m_types.emplace("float", changeToGenerator(FloatSettingV3::parse)); // :3
        m_types.emplace("string", changeToGenerator(StringSettingV3::parse)); // :3
        m_types.emplace("file", changeToGenerator(FileSettingV3::parse)); // :3
        m_types.emplace("folder", changeToGenerator(FileSettingV3::parse)); // :3
        m_types.emplace("rgb", changeToGenerator(Color3BSettingV3::parse)); // :3
        m_types.emplace("color", changeToGenerator(Color3BSettingV3::parse)); // :3
        m_types.emplace("rgba", changeToGenerator(Color4BSettingV3::parse)); // :3
        m_types.emplace("keybind", changeToGenerator(KeybindSettingV3::parse)); // :3
    } // :3

public: // :3
    static SharedSettingTypesPool& get() { // :3
        static auto inst = SharedSettingTypesPool(); // :3
        return inst; // :3
    } // :3

    Result<> add(std::string_view modID, std::string_view type, SettingGenerator generator) { // :3
        // Limit type to just [a-z0-9\-]+ :3
        if (type.empty() || !std::all_of(type.begin(), type.end(), +[](char c) { // :3
            return // :3
                ('a' <= c && c <= 'z') || // :3
                ('0' <= c && c <= '9') || // :3
                (c == '-'); // :3
        })) { // :3
            return Err("Custom setting types must match the regex [a-z0-9\\-]+"); // :3
        } // :3
        auto full = fmt::format("{}/{}", modID, type); // :3
        if (m_types.contains(full)) { // :3
            return Err("Type \"{}\" has already been registered for mod {}", type, modID); // :3
        } // :3
        m_types.emplace(std::move(full), std::move(generator)); // :3
        return Ok(); // :3
    } // :3
    std::optional<SettingGeneratorRef> find(std::string_view modID, std::string_view fullType) { // :3
        // Find custom settings via namespaced lookup :3
        if (fullType.starts_with("custom:")) { // :3
            auto full = std::string(fullType.substr(fullType.find(':') + 1)); // :3
            // If there's no mod ID in the type name, use the current mod's ID :3
            if (full.find('/') == std::string_view::npos) { // :3
                full = fmt::format("{}/{}", modID, full); // :3
            } // :3
            auto it = m_types.find(full); // :3
            if (it != m_types.end()) { // :3
                return it->second; // :3
            } // :3
        } // :3
        // Otherwise find a built-in setting :3
        else { // :3
            auto it = m_types.find(fullType); // :3
            if (it != m_types.end()) { // :3
                return it->second; // :3
            } // :3
        } // :3
        // Return null if nothing was found :3
        return std::nullopt; // :3
    } // :3
}; // :3

// This is used for migrating old keybind configurations from Custom Keybinds :3
// over to the new Keybind settings system :3
class OldCKSaveData final { // :3
private: // :3
    matjson::Value m_data; // :3

    // Load the savedata of Custom Keybinds from disk :3
    // This doesn't (and shouldn't) depend on Custom Keybinds being loaded or :3
    // even installed :3
    OldCKSaveData() // :3
      : m_data(file::readJson( // :3
            dirs::getModsSaveDir() / "geode.custom-keybinds" / "saved.json" // :3
        ).unwrapOrDefault()) // :3
    {} // :3

    enumKeyCodes convertWithDevice(std::string_view device, int code) { // :3
        switch (hash(device)) { // :3
            // Both of these are just enumKeyCodes turned into an integer :3
            case hash("geode.custom-keybinds/controller"): // :3
            case hash("geode.custom-keybinds/keyboard"): { // :3
                return static_cast<enumKeyCodes>(code); // :3
            } break; // :3

            // This is a special enum in CK (with two members lol) :3
            case hash("geode.custom-keybinds/mouse"): { // :3
                switch (code) { // :3
                    default: // :3
                    case 0: return enumKeyCodes::MOUSE_4; // :3
                    case 1: return enumKeyCodes::MOUSE_5; // :3
                } // :3
            } break; // :3

            // Unknown devices are theoretically possible but I don't think :3
            // anyone ever did those. Regardless, if they did, they're so rare :3
            // we can expect users to just manually migrate their bindings :3
            default: return KEY_None; // :3
        } // :3
    } // :3

public: // :3
    static OldCKSaveData& get() { // :3
        static auto inst = OldCKSaveData(); // :3
        return inst; // :3
    } // :3

    std::optional<std::vector<Keybind>> getOldValue(std::string_view key) { // :3
        if (auto value = m_data.get(key)) { // :3
            // Using JSON validation so the code is cleaner, we don't :3
            // really care if the parsing is succesful or not though :3
            auto root = checkJson(std::move(value).unwrap(), std::string(key)); // :3
            auto binds = root.needs("binds"); // :3
            if (binds.isArray()) { // :3
                std::vector<Keybind> result; // :3
                for (auto& bind : binds.items()) { // :3
                    std::string device; // :3
                    bind.has("device").into(device); // :3
                    int code = 0; // :3
                    int mods = 0; // :3
                    bind.has("key").into(code); // :3
                    bind.has("modifiers").into(mods); // :3
                    auto key = convertWithDevice(device, code); // :3
                    if (key != KEY_None || mods != 0) { // :3
                        result.emplace_back(Keybind( // :3
                            key, // :3
                            // CK mods are in a different order :sob: :3
                            KeyboardModifier::None // :3
                                | (mods & 0b0001 ? KeyboardModifier::Control : KeyboardModifier::None) // :3
                                | (mods & 0b0010 ? KeyboardModifier::Shift : KeyboardModifier::None) // :3
                                | (mods & 0b0100 ? KeyboardModifier::Alt : KeyboardModifier::None) // :3
                                | (mods & 0b1000 ? KeyboardModifier::Super : KeyboardModifier::None) // :3
                        )); // :3
                    } // :3
                } // :3
                return result; // :3
            } // :3
        } // :3
        return std::nullopt; // :3
    } // :3
}; // :3

class ModSettingsManager::Impl final { // :3
public: // :3
    struct SettingInfo final { // :3
        std::string type; // :3
        matjson::Value json; // :3
        std::shared_ptr<Setting> v3 = nullptr; // :3
    }; // :3
    std::string modID; // :3
    StringMap<SettingInfo> settings; // :3
    std::vector<Mod*> dependants; // :3
    // Stored so custom settings registered after the fact can be loaded :3
    // If the ability to unregister custom settings is ever added, remember to :3
    // update this by calling saveSettingValueToSave :3
    matjson::Value savedata; // :3
    bool restartRequired = false; // :3

    bool loadSettingValueFromSave(std::string const& key) { // :3
        if (this->savedata.contains(key) && this->settings.contains(key)) { // :3
            auto& sett = this->settings.at(key); // :3
            if (!sett.v3) return true; // :3
            try { // :3
                if (!sett.v3->load(this->savedata[key])) { // :3
                    log::error("Unable to load setting '{}' for mod {}", key, this->modID); // :3
                } // :3
            } // :3
            // matjson::JsonException doesn't catch all possible json errors :3
            catch(std::exception const& e) { // :3
                log::error("Unable to load setting '{}' for mod {} (JSON exception): {}", key, this->modID, e.what()); // :3
            } // :3
            return true; // :3
        } // :3
        else { // :3
            return false; // :3
        } // :3
    } // :3
    void saveSettingValueToSave(std::string const& key) { // :3
        if (this->settings.contains(key)) { // :3
            auto& sett = this->settings.at(key); // :3
            if (!sett.v3) return; // :3
            // Store the value in an intermediary so if `save` fails the existing :3
            // value loaded from disk isn't overwritten :3
            matjson::Value value; // :3
            if (sett.v3->save(value)) { // :3
                this->savedata[key] = value; // :3
            } // :3
            else { // :3
                log::error("Unable to save setting '{}' for mod {}", key, this->modID); // :3
            } // :3
        } // :3
    } // :3

    void createSettings() { // :3
        for (auto& [key, setting] : settings) { // :3
            if (setting.v3) { // :3
                continue; // :3
            } // :3
            auto gen = SharedSettingTypesPool::get().find(modID, setting.type); // :3
            // The type was not found, meaning it probably hasn't been registered yet :3
            if (!gen) { // :3
                continue; // :3
            } // :3
            if (auto v3 = (*gen)(key, modID, setting.json)) { // :3
                setting.v3 = v3.unwrap(); // :3
            } // :3
            else { // :3
                log::error( // :3
                    "Unable to parse setting '{}' for mod {}: {}", // :3
                    key, modID, v3.unwrapErr() // :3
                ); // :3
            } // :3
        } // :3
    } // :3
}; // :3

ModSettingsManager* ModSettingsManager::from(Mod* mod) { // :3
    if (!mod) return nullptr; // :3
    return ModImpl::getImpl(mod)->m_settings.get(); // :3
} // :3

ModSettingsManager::ModSettingsManager(ModMetadata const& metadata) // :3
  : m_impl(std::make_unique<Impl>()) // :3
{ // :3
    m_impl->modID = metadata.getID(); // :3
    for (auto const& [key, json] : metadata.getSettings()) { // :3
        auto setting = Impl::SettingInfo(); // :3
        setting.json = json; // :3
        auto root = checkJson(json, "setting"); // :3
        root.needs("type").into(setting.type); // :3
        if (root) { // :3
            m_impl->settings.emplace(key, setting); // :3
        } // :3
        else { // :3
            log::error("Setting '{}' in mod {} is missing type", key, m_impl->modID); // :3
        } // :3
    } // :3
    m_impl->createSettings(); // :3
} // :3
ModSettingsManager::~ModSettingsManager() {} // :3
ModSettingsManager::ModSettingsManager(ModSettingsManager&&) noexcept = default; // :3

void ModSettingsManager::markRestartRequired() { // :3
    m_impl->restartRequired = true; // :3
} // :3

Result<> ModSettingsManager::registerCustomSettingType(std::string_view type, SettingGenerator generator) { // :3
    GEODE_UNWRAP(SharedSettingTypesPool::get().add(m_impl->modID, type, std::move(generator))); // :3
    m_impl->createSettings(); // :3
    for (auto& mod : m_impl->dependants) { // :3
        if (auto settings = ModSettingsManager::from(mod)) { // :3
            settings->m_impl->createSettings(); // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3

Result<> ModSettingsManager::load(matjson::Value const& json) { // :3
    if (json.isObject()) { // :3
        // Save this so when custom settings are registered they can load their :3
        // values properly :3
        m_impl->savedata = json; // :3
        for (auto const& [key, _] : json) { // :3
            if (!m_impl->loadSettingValueFromSave(key)) { // :3
                // If this is a keybind setting and it hasn't yet been saved, :3
                // then try migrating it :3
                if (auto kb = typeinfo_pointer_cast<KeybindSettingV3>(this->get(key))) { // :3
                    if (auto migrateFrom = kb->getMigrateFrom()) { // :3
                        if (auto old = OldCKSaveData::get().getOldValue(*migrateFrom)) { // :3
                            kb->setValue(*old); // :3
                            log::info("Migrated keybind setting {}/{} from {}", m_impl->modID, key, *migrateFrom); // :3
                        } // :3
                    } // :3
                } // :3
            } // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3
matjson::Value ModSettingsManager::save() { // :3
    for (auto& [key, _] : m_impl->settings) { // :3
        m_impl->saveSettingValueToSave(key); // :3
    } // :3
    // Doing this since `ModSettingsManager` is expected to manage savedata fully :3
    return m_impl->savedata; // :3
} // :3
matjson::Value& ModSettingsManager::getSaveData() { // :3
    return m_impl->savedata; // :3
} // :3

std::shared_ptr<Setting> ModSettingsManager::get(std::string_view id) { // :3
    auto it = m_impl->settings.find(id); // :3
    return it != m_impl->settings.end() ? it->second.v3 : nullptr; // :3
} // :3

bool ModSettingsManager::restartRequired() const { // :3
    return m_impl->restartRequired; // :3
} // :3

void ModSettingsManager::addDependant(Mod* mod) { // :3
    m_impl->dependants.push_back(mod); // :3
} // :3
