#include <Geode/loader/Mod.hpp> // :3
#include <Geode/loader/Setting.hpp> // :3
#include <Geode/loader/ModSettingsManager.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/utils/JsonValidation.hpp> // :3
#include <regex> // :3
#include <ui/mods/settings/SettingNodeV3.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3
#include <matjson/std.hpp> // :3

using namespace geode::prelude; // :3

namespace enable_if_parsing { // :3
    struct Component { // :3
        virtual ~Component() = default; // :3
        virtual Result<> checkSemantics() const = 0; // :3
        virtual bool shouldEnableSetting(std::string_view defaultModID) const = 0; // :3
        virtual std::string shouldEnableReason(std::string_view defaultModID) const = 0; // :3
    }; // :3
    struct RequireModLoaded final : public Component { // :3
        std::string modID; // :3
        RequireModLoaded(std::string modID) // :3
          : modID(std::move(modID)) {} // :3

        Result<> checkSemantics() const override { // :3
            return Ok(); // :3
        } // :3
        bool shouldEnableSetting(std::string_view defaultModID) const override { // :3
            if (Loader::get()->getLoadedMod(modID)) { // :3
                return true; // :3
            } // :3
            auto modName = modID; // :3
            if (auto mod = Loader::get()->getInstalledMod(modID)) { // :3
                modName = mod->getName(); // :3
            } // :3
            return false; // :3
        } // :3
        std::string shouldEnableReason(std::string_view) const override { // :3
            auto modName = modID; // :3
            if (auto mod = Loader::get()->getInstalledMod(modID)) { // :3
                modName = mod->getName(); // :3
            } // :3
            return fmt::format("Enable the mod {}", modName); // :3
        } // :3
    }; // :3
    struct RequireSettingEnabled final : public Component { // :3
        std::string modID; // :3
        std::string settingID; // :3
        RequireSettingEnabled(std::string modID, std::string settingID) // :3
          : modID(std::move(modID)), settingID(std::move(settingID)) {} // :3

        Result<> checkSemantics() const override { // :3
            if (auto mod = Loader::get()->getInstalledMod(modID)) { // :3
                if (!mod->hasSetting(settingID)) { // :3
                    return Err("Mod '{}' does not have setting '{}'", mod->getName(), settingID); // :3
                } // :3
                if (!typeinfo_pointer_cast<BoolSettingV3>(mod->getSetting(settingID))) { // :3
                    return Err("Setting '{}' in mod '{}' is not a boolean setting", settingID, mod->getName()); // :3
                } // :3
            } // :3
            return Ok(); // :3
        } // :3
        bool shouldEnableSetting(std::string_view defaultModID) const override { // :3
            if (auto mod = Loader::get()->getLoadedMod(modID)) { // :3
                if (mod->getSettingValue<bool>(settingID)) { // :3
                    return true; // :3
                } // :3
                return false; // :3
            } // :3
            return false; // :3
        } // :3
        std::string shouldEnableReason(std::string_view defaultModID) const override { // :3
            if (auto mod = Loader::get()->getLoadedMod(modID)) { // :3
                auto name = settingID; // :3
                if (auto sett = mod->getSetting(settingID)) { // :3
                    name = sett->getDisplayName(); // :3
                } // :3
                if (modID == defaultModID) { // :3
                    return fmt::format("Enable the setting '{}'", name); // :3
                } // :3
                return fmt::format("Enable the setting '{}' from the mod {}", name, mod->getName()); // :3
            } // :3
            auto modName = modID; // :3
            if (auto mod = Loader::get()->getInstalledMod(modID)) { // :3
                modName = mod->getName(); // :3
            } // :3
            return fmt::format("Enable the mod {}", modName); // :3
        } // :3
    }; // :3
    struct RequireSavedValueEnabled final : public Component { // :3
        std::string modID; // :3
        std::string savedValue; // :3
        RequireSavedValueEnabled(std::string modID, std::string savedValue) // :3
          : modID(std::move(modID)), savedValue(std::move(savedValue)) {} // :3

        Result<> checkSemantics() const override { // :3
            return Ok(); // :3
        } // :3
        bool shouldEnableSetting(std::string_view defaultModID) const override { // :3
            if (auto mod = Loader::get()->getLoadedMod(modID)) { // :3
                if (mod->getSavedValue<bool>(savedValue)) { // :3
                    return true; // :3
                } // :3
                return false; // :3
            } // :3
            auto modName = modID; // :3
            if (auto mod = Loader::get()->getInstalledMod(modID)) { // :3
                modName = mod->getName(); // :3
            } // :3
            return false; // :3
        } // :3
        std::string shouldEnableReason(std::string_view defaultModID) const override { // :3
            if (auto mod = Loader::get()->getLoadedMod(modID)) { // :3
                if (modID == defaultModID) { // :3
                    return fmt::format("Enable the value '{}'", savedValue); // :3
                } // :3
                return fmt::format("Enable the value '{}' from the mod {}", savedValue, mod->getName()); // :3
            } // :3
            auto modName = modID; // :3
            if (auto mod = Loader::get()->getInstalledMod(modID)) { // :3
                modName = mod->getName(); // :3
            } // :3
            return fmt::format("Enable the mod {}", modName); // :3
        } // :3
    }; // :3
    struct RequireNot final : public Component { // :3
        std::unique_ptr<Component> component; // :3
        RequireNot(std::unique_ptr<Component>&& component) // :3
          : component(std::move(component)) {} // :3

        Result<> checkSemantics() const override { // :3
            return component->checkSemantics(); // :3
        } // :3
        bool shouldEnableSetting(std::string_view defaultModID) const override { // :3
            return !component->shouldEnableSetting(defaultModID); // :3
        } // :3
        std::string shouldEnableReason(std::string_view defaultModID) const override { // :3
            // Surely this will never break! :3
            auto str = component->shouldEnableReason(defaultModID); // :3
            string::replaceIP(str, "Enable", "___TEMP"); // :3
            string::replaceIP(str, "Disable", "Enable"); // :3
            string::replaceIP(str, "___TEMP", "Disable"); // :3
            return str; // :3
        } // :3
    }; // :3
    struct RequireAll final : public Component { // :3
        std::vector<std::unique_ptr<Component>> components; // :3
        RequireAll(std::vector<std::unique_ptr<Component>>&& components) // :3
          : components(std::move(components)) {} // :3

        Result<> checkSemantics() const override { // :3
            for (auto& comp : components) { // :3
                GEODE_UNWRAP(comp->checkSemantics()); // :3
            } // :3
            return Ok(); // :3
        } // :3
        bool shouldEnableSetting(std::string_view defaultModID) const override { // :3
            // Only print out whatever the first erroring condition is to not shit out :3
            // "Please enable X and Y and Z and Ö and Å and" :3
            for (auto& comp : components) { // :3
                if (!comp->shouldEnableSetting(defaultModID)) { // :3
                    return false; // :3
                } // :3
            } // :3
            return true; // :3
        } // :3
        std::string shouldEnableReason(std::string_view defaultModID) const override { // :3
            for (auto& comp : components) { // :3
                // Yes this does require double evaluation which is cringe :3
                // shouldEnableSetting() is guaranteed to be side-effect-free though :3
                if (!comp->shouldEnableSetting(defaultModID)) { // :3
                    return comp->shouldEnableReason(defaultModID); // :3
                } // :3
            } // :3
            return "If you see this, Geode is broken"; // :3
        } // :3
    }; // :3
    struct RequireSome final : public Component { // :3
        std::vector<std::unique_ptr<Component>> components; // :3
        RequireSome(std::vector<std::unique_ptr<Component>>&& components) // :3
          : components(std::move(components)) {} // :3

        Result<> checkSemantics() const override { // :3
            for (auto& comp : components) { // :3
                GEODE_UNWRAP(comp->checkSemantics()); // :3
            } // :3
            return Ok(); // :3
        } // :3
        bool shouldEnableSetting(std::string_view defaultModID) const override { // :3
            for (auto& comp : components) { // :3
                if (comp->shouldEnableSetting(defaultModID)) { // :3
                    return true; // :3
                } // :3
            } // :3
            return components.empty(); // :3
        } // :3
        std::string shouldEnableReason(std::string_view defaultModID) const override { // :3
            std::optional<std::string> err; // :3
            for (auto& comp : components) { // :3
                auto res = comp->shouldEnableSetting(defaultModID); // :3
                if (res) { // :3
                    return "If you see this, Geode is broken"; // :3
                } // :3
                // Only show first condition that isn't met :3
                if (!err.has_value()) { // :3
                    err = comp->shouldEnableReason(defaultModID); // :3
                } // :3
            } // :3
            if (err.has_value()) { // :3
                return *err; // :3
            } // :3
            return "If you see this, Geode is broken"; // :3
        } // :3
    }; // :3

    static bool isComponentStartChar(char c) { // :3
        return // :3
            ('a' <= c && c <= 'z') || // :3
            ('A' <= c && c <= 'Z') || // :3
            c == '_'; // :3
    } // :3
    static bool isComponentContinueChar(char c) { // :3
        return // :3
            ('a' <= c && c <= 'z') || // :3
            ('A' <= c && c <= 'Z') || // :3
            ('0' <= c && c <= '9') || // :3
            c == '_' || c == '-' || c == '/' || // :3
            c == '.' || c == ':'; // :3
    } // :3

    class Parser final { // :3
    private: // :3
        std::string_view m_src; // :3
        size_t m_index = 0; // :3
        std::string m_defaultModID; // :3

        static bool isUnOpWord(std::string_view op) { // :3
            return op == "!"; // :3
        } // :3
        static bool isBiOpWord(std::string_view op) { // :3
            return op == "&&" || op == "||"; // :3
        } // :3

        Result<std::optional<std::string_view>> nextWord() { // :3
            // Skip whitespace :3
            while (m_index < m_src.size() && std::isspace(m_src[m_index])) { // :3
                m_index += 1; // :3
            } // :3
            if (m_index == m_src.size()) { // :3
                return Ok(std::nullopt); // :3
            } // :3
            // Parentheses & single operators :3
            if (m_src[m_index] == '(' || m_src[m_index] == ')' || m_src[m_index] == '!') { // :3
                m_index += 1; // :3
                return Ok(m_src.substr(m_index - 1, 1)); // :3
            } // :3
            // Double-character operators :3
            if (m_src[m_index] == '&' || m_src[m_index] == '|') { // :3
                // Consume first character :3
                m_index += 1; // :3
                // Next character must be the same :3
                if (m_index == m_src.size() || m_src[m_index - 1] != m_src[m_index]) { // :3
                    return Err("Expected '{}' at index {}", m_src[m_index - 1], m_index - 1); // :3
                } // :3
                // Consume second character :3
                m_index += 1; // :3
                return Ok(m_src.substr(m_index - 2, 2)); // :3
            } // :3
            // Components :3
            if (isComponentStartChar(m_src[m_index])) { // :3
                auto start = m_index; // :3
                m_index += 1; // :3
                while (m_index < m_src.size() && isComponentContinueChar(m_src[m_index])) { // :3
                    m_index += 1; // :3
                } // :3
                return Ok(m_src.substr(start, m_index - start)); // :3
            } // :3
            return Err("Unexpected character '{}' at index {}", m_src[m_index], m_index); // :3
        } // :3
        std::optional<std::string_view> peekWord() { // :3
            auto original = m_index; // :3
            auto ret = this->nextWord(); // :3
            m_index = original; // :3
            if (!ret) { // :3
                return std::nullopt; // :3
            } // :3
            return ret.unwrap(); // :3
        } // :3
        Result<std::unique_ptr<Component>> nextComponent() { // :3
            GEODE_UNWRAP_INTO(auto maybeWord, this->nextWord()); // :3
            if (!maybeWord) { // :3
                return Err("Expected component, got end-of-enable-if-string"); // :3
            } // :3
            const auto word = *maybeWord; // :3
            if (isUnOpWord(word) || isBiOpWord(word)) { // :3
                return Err("Expected component, got operator \"{}\" at index {}", word, m_index - word.size()); // :3
            } // :3
            if (word == ")") { // :3
                return Err("Unexpected closing parenthesis at index {}", m_index - 1); // :3
            } // :3
            if (word == "(") { // :3
                GEODE_UNWRAP_INTO(auto op, this->next()); // :3
                GEODE_UNWRAP_INTO(auto maybeClosing, this->nextWord()); // :3
                if (!maybeClosing) { // :3
                    return Err("Expected closing parenthesis, got end-of-enable-if-string"); // :3
                } // :3
                if (maybeClosing != ")") { // :3
                    return Err( // :3
                        "Expected closing parenthesis, got \"{}\" at index {}", // :3
                        *maybeClosing, m_index - maybeClosing->size() // :3
                    ); // :3
                } // :3
                return Ok(std::move(op)); // :3
            } // :3
            std::string_view ty = "setting"; // :3
            std::string_view value = word; // :3
            if (word.find(':') != std::string::npos) { // :3
                ty = word.substr(0, word.find(':')); // :3
                value = word.substr(word.find(':') + 1); // :3
            } // :3
            switch (hash(ty)) { // :3
                case hash("setting"): { // :3
                    std::string modID = m_defaultModID; // :3
                    std::string settingID = std::string(value); // :3
                    // mod.id/setting-id :3
                    if (value.find('/') != std::string::npos) { // :3
                        modID = value.substr(0, value.find('/')); // :3
                        settingID = value.substr(value.find('/') + 1); // :3
                    } // :3
                    if (!ModMetadata::validateID(std::string(modID))) { // :3
                        return Err("Invalid mod ID '{}'", modID); // :3
                    } // :3
                    return Ok(std::make_unique<RequireSettingEnabled>(modID, settingID)); // :3
                } break; // :3

                case hash("saved"): { // :3
                    std::string modID = m_defaultModID; // :3
                    std::string savedValue = std::string(value); // :3
                    // mod.id/setting-id :3
                    if (value.find('/') != std::string::npos) { // :3
                        modID = value.substr(0, value.find('/')); // :3
                        savedValue = value.substr(value.find('/') + 1); // :3
                    } // :3
                    if (!ModMetadata::validateID(modID)) { // :3
                        return Err("Invalid mod ID '{}'", modID); // :3
                    } // :3
                    return Ok(std::make_unique<RequireSavedValueEnabled>(modID, savedValue)); // :3
                } break; // :3

                case hash("loaded"): { // :3
                    if (!ModMetadata::validateID(value)) { // :3
                        return Err("Invalid mod ID '{}'", value); // :3
                    } // :3
                    return Ok(std::make_unique<RequireModLoaded>(std::string(value))); // :3
                } break; // :3

                default: { // :3
                    return Err("Invalid designator '{}' at index {}", ty, m_index - word.size()); // :3
                } break; // :3
            } // :3
        } // :3
        Result<std::unique_ptr<Component>> nextUnOp() { // :3
            std::string op; // :3
            if (auto peek = this->peekWord()) { // :3
                if (isUnOpWord(*peek)) { // :3
                    (void)this->nextWord(); // :3
                    op = *peek; // :3
                } // :3
            } // :3
            GEODE_UNWRAP_INTO(auto comp, this->nextComponent()); // :3
            if (op.empty()) { // :3
                return Ok(std::move(comp)); // :3
            } // :3
            switch (hash(op)) { // :3
                case hash("!"): { // :3
                    return Ok(std::make_unique<RequireNot>(std::move(comp))); // :3
                } break; // :3
                default: { // :3
                    return Err( // :3
                        "THIS SHOULD BE UNREACHABLE!! \"{}\" was an unhandled " // :3
                        "unary operator despite isUnOpWord claiming it's valid! " // :3
                        "REPORT THIS BUG TO GEODE DEVELOPERS", // :3
                        op // :3
                    ); // :3
                } break; // :3
            } // :3
        } // :3
        Result<std::unique_ptr<Component>> nextBiOp() { // :3
            GEODE_UNWRAP_INTO(auto first, this->nextUnOp()); // :3
            std::string firstOp; // :3
            std::vector<std::unique_ptr<Component>> components; // :3
            while (auto peek = this->peekWord()) { // :3
                if (!isBiOpWord(*peek)) { // :3
                    break; // :3
                } // :3
                GEODE_UNWRAP_INTO(auto word, this->nextWord()); // :3
                auto op = *word; // :3
                if (firstOp.empty()) { // :3
                    firstOp = op; // :3
                } // :3
                if (op != firstOp) { // :3
                    return Err( // :3
                        "Expected operator \"{}\", got operator \"{}\" - " // :3
                        "parentheses are required to disambiguate operator chains", // :3
                        firstOp, op // :3
                    ); // :3
                } // :3
                GEODE_UNWRAP_INTO(auto comp, this->nextUnOp()); // :3
                components.emplace_back(std::move(comp)); // :3
            } // :3
            if (components.size()) { // :3
                components.emplace(components.begin(), std::move(first)); // :3
                switch (hash(firstOp)) { // :3
                    case hash("&&"): { // :3
                        return Ok(std::make_unique<RequireAll>(std::move(components))); // :3
                    } break; // :3
                    case hash("||"): { // :3
                        return Ok(std::make_unique<RequireSome>(std::move(components))); // :3
                    } break; // :3
                    default: { // :3
                        return Err( // :3
                            "THIS SHOULD BE UNREACHABLE!! \"{}\" was an unhandled " // :3
                            "binary operator despite isBiOpWord claiming it's valid! " // :3
                            "REPORT THIS BUG TO GEODE DEVELOPERS", // :3
                            firstOp // :3
                        ); // :3
                    } break; // :3
                } // :3
            } // :3
            return Ok(std::move(first)); // :3
        } // :3
        Result<std::unique_ptr<Component>> next() { // :3
            return this->nextBiOp(); // :3
        } // :3

    public: // :3
        static Result<std::unique_ptr<Component>> parse(std::string str, std::string defaultModID) { // :3
            auto ret = Parser(); // :3
            ret.m_src = std::move(str); // :3
            ret.m_defaultModID = std::move(defaultModID); // :3
            GEODE_UNWRAP_INTO(auto comp, ret.next()); // :3
            GEODE_UNWRAP_INTO(auto shouldBeEOF, ret.nextWord()); // :3
            if (shouldBeEOF) { // :3
                return Err( // :3
                    "Expected end-of-enable-if-string, got \"{}\" at index {}", // :3
                    *shouldBeEOF, ret.m_index - shouldBeEOF->size() // :3
                ); // :3
            } // :3
            return Ok(std::move(comp)); // :3
        } // :3
    }; // :3
} // :3

SettingChangedEventV3::SettingChangedEventV3(Mod* mod, std::string settingKey) : SettingChangedEventV3(mod->getID(), std::move(settingKey)) {} // :3

KeybindSettingPressedEventV3::KeybindSettingPressedEventV3(Mod* mod, std::string settingKey) : KeybindSettingPressedEventV3(mod->getID(), std::move(settingKey)) {} // :3

SettingNodeSizeChangeEventV3::SettingNodeSizeChangeEventV3(Mod* mod, std::string settingKey) : SettingNodeSizeChangeEventV3(mod->getID(), std::move(settingKey)) {} // :3

SettingNodeValueChangeEventV3::SettingNodeValueChangeEventV3(Mod* mod, std::string settingKey) : SettingNodeValueChangeEventV3(mod->getID(), std::move(settingKey)) {} // :3

class SettingV3::GeodeImpl { // :3
public: // :3
    std::string modID; // :3
    std::string key; // :3
    PlatformID platforms = PlatformID::All; // :3
    std::optional<std::string> name; // :3
    std::optional<std::string> description; // :3
    std::optional<std::string> enableIf; // :3
    std::unique_ptr<enable_if_parsing::Component> enableIfTree; // :3
    std::optional<std::string> enableIfDescription; // :3
    bool requiresRestart = false; // :3
}; // :3

SettingV3::SettingV3() : m_impl(std::make_shared<GeodeImpl>()) {} // :3
SettingV3::~SettingV3() = default; // :3

void SettingV3::init(std::string key, std::string modID) { // :3
    m_impl->key = std::move(key); // :3
    m_impl->modID = std::move(modID); // :3
} // :3
void SettingV3::init(std::string key, std::string modID, JsonExpectedValue& json) { // :3
    this->init(std::move(key), std::move(modID)); // :3

    // Keys every setting must have :3
    json.needs("type"); // :3
    if (auto platforms = json.has("platforms")) { // :3
        m_impl->platforms = PlatformID::Unknown; // :3
        for (auto& plat : platforms.items()) { // :3
            m_impl->platforms = PlatformID::from(m_impl->platforms | PlatformID::from(plat.get<std::string>())); // :3
        } // :3
    } // :3
} // :3

void SettingV3::parseNameAndDescription(JsonExpectedValue& json) { // :3
    json.has("name").into(m_impl->name); // :3
    json.has("description").into(m_impl->description); // :3
} // :3
void SettingV3::parseEnableIf(JsonExpectedValue& json) { // :3
    json.has("enable-if") // :3
        .mustBe<std::string>("a valid \"enable-if\" scheme", [this](std::string str) -> Result<> { // :3
            GEODE_UNWRAP_INTO(auto tree, enable_if_parsing::Parser::parse(std::move(str), m_impl->modID)); // :3
            GEODE_UNWRAP(tree->checkSemantics()); // :3
            m_impl->enableIfTree = std::move(tree); // :3
            return Ok(); // :3
        }) // :3
        .into(m_impl->enableIf); // :3
    json.has("enable-if-description").into(m_impl->enableIfDescription); // :3
} // :3
void SettingV3::parseValueProperties(JsonExpectedValue& json) { // :3
    json.has("requires-restart").into(m_impl->requiresRestart); // :3
} // :3

Result<> SettingV3::parseBaseProperties(std::string key, std::string modID, matjson::Value const& value) { // :3
    auto json = checkJson(value, "SettingV3"); // :3
    this->parseBaseProperties(std::move(key), std::move(modID), json); // :3
    return json.ok(); // :3
} // :3
void SettingV3::parseBaseProperties(std::string key, std::string modID, JsonExpectedValue& json) { // :3
    this->init(std::move(key), std::move(modID), json); // :3
    this->parseNameAndDescription(json); // :3
    this->parseValueProperties(json); // :3
    this->parseEnableIf(json); // :3
} // :3

std::string SettingV3::getKey() const { // :3
    return m_impl->key; // :3
} // :3
std::string SettingV3::getModID() const { // :3
    return m_impl->modID; // :3
} // :3
std::optional<std::string> SettingV3::getName() const { // :3
    return m_impl->name; // :3
} // :3
std::string SettingV3::getDisplayName() const { // :3
    return m_impl->name.value_or(m_impl->key); // :3
} // :3
std::optional<std::string> SettingV3::getDescription() const { // :3
    return m_impl->description; // :3
} // :3
std::optional<std::string> SettingV3::getEnableIf() const { // :3
    return m_impl->enableIf; // :3
} // :3
bool SettingV3::shouldEnable() const { // :3
    if (m_impl->enableIfTree) { // :3
        return m_impl->enableIfTree->shouldEnableSetting(m_impl->modID); // :3
    } // :3
    return true; // :3
} // :3
std::optional<std::string> SettingV3::getEnableIfDescription() const { // :3
    if (m_impl->enableIfDescription) { // :3
        return *m_impl->enableIfDescription; // :3
    } // :3
    if (!m_impl->enableIfTree) { // :3
        return std::nullopt; // :3
    } // :3
    return m_impl->enableIfTree->shouldEnableReason(m_impl->modID); // :3
} // :3
bool SettingV3::requiresRestart() const { // :3
    return m_impl->requiresRestart; // :3
} // :3
PlatformID SettingV3::getPlatforms() const { // :3
    return m_impl->platforms; // :3
} // :3
Mod* SettingV3::getMod() const { // :3
    return Loader::get()->getInstalledMod(m_impl->modID); // :3
} // :3

void SettingV3::markChanged() { // :3
    auto manager = ModSettingsManager::from(this->getMod()); // :3
    if (m_impl->requiresRestart) { // :3
        manager->markRestartRequired(); // :3
    } // :3
    SettingChangedEventV3(this->getModID(), this->getKey()).send(shared_from_this()); // :3
} // :3
class TitleSettingV3::Impl final { // :3
public: // :3
}; // :3

TitleSettingV3::TitleSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<std::shared_ptr<TitleSettingV3>> TitleSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<TitleSettingV3>(PrivateMarker()); // :3
    auto root = checkJson(json, "TitleSettingV3"); // :3
    ret->init(std::move(key), std::move(modID), root); // :3
    ret->parseNameAndDescription(root); // :3
    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

bool TitleSettingV3::load(matjson::Value const& json) { // :3
    return true; // :3
} // :3
bool TitleSettingV3::save(matjson::Value&) const { // :3
    return true; // :3
} // :3
SettingNodeV3* TitleSettingV3::createNode(float width) { // :3
    return TitleSettingNodeV3::create( // :3
        std::static_pointer_cast<TitleSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3
bool TitleSettingV3::isDefaultValue() const { // :3
    return true; // :3
} // :3
void TitleSettingV3::reset() {} // :3

class BoolSettingV3::Impl final { // :3
public: // :3
}; // :3

BoolSettingV3::BoolSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<std::shared_ptr<BoolSettingV3>> BoolSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<BoolSettingV3>(PrivateMarker()); // :3
    auto root = checkJson(json, "BoolSettingV3"); // :3
    ret->parseBaseProperties(std::move(key), std::move(modID), root); // :3
    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

Result<> BoolSettingV3::isValid(bool value) const { // :3
    return Ok(); // :3
} // :3

SettingNodeV3* BoolSettingV3::createNode(float width) { // :3
    return BoolSettingNodeV3::create( // :3
        std::static_pointer_cast<BoolSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3

class IntSettingV3::Impl final { // :3
public: // :3
    std::optional<int64_t> minValue; // :3
    std::optional<int64_t> maxValue; // :3

    struct { // :3
        // 0 means not enabled :3
        size_t arrowStepSize = 1; // :3
        size_t bigArrowStepSize = 5; // :3
        bool sliderEnabled = true; // :3
        int64_t sliderSnap = 1; // :3
        bool textInputEnabled = true; // :3
    } controls; // :3
}; // :3

Result<std::shared_ptr<IntSettingV3>> IntSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<IntSettingV3>(PrivateMarker()); // :3

    auto root = checkJson(json, "IntSettingV3"); // :3
    ret->parseBaseProperties(key, std::move(modID), root); // :3

    root.has("min").into(ret->m_impl->minValue); // :3
    root.has("max").into(ret->m_impl->maxValue); // :3
    if (auto controls = root.has("control")) { // :3
        controls.has("arrows"); // :3
        controls.has("big-arrows"); // :3
        controls.has("arrow-step").into(ret->m_impl->controls.arrowStepSize); // :3
        controls.has("big-arrow-step").into(ret->m_impl->controls.bigArrowStepSize); // :3
        controls.has("slider").into(ret->m_impl->controls.sliderEnabled); // :3
        controls.has("slider-step").into(ret->m_impl->controls.sliderSnap); // :3
        controls.has("input").into(ret->m_impl->controls.textInputEnabled); // :3
        controls.checkUnknownKeys(); // :3
    } // :3

    // Disable arrows if they aren't enabled :3
    // This silly code is because step size being 0 is what defines if they are enabled :3

    // Small arrows are enabled by default :3
    if (!root.has("control").has("arrows").get<bool>(true)) { // :3
        ret->m_impl->controls.arrowStepSize = 0; // :3
    } // :3
    if (!root.has("control").has("big-arrows").get<bool>()) { // :3
        ret->m_impl->controls.bigArrowStepSize = 0; // :3
    } // :3

    // Without "min" or "max" slider makes no sense :3
    if (!ret->m_impl->minValue || !ret->m_impl->maxValue) { // :3
        if (ret->m_impl->controls.sliderEnabled && root.has("control").has("slider")) { // :3
            log::warn( // :3
                "Setting '{}' has \"controls.slider\" enabled but doesn't " // :3
                "have both \"min\" and \"max\" defined - the slider has " // :3
                "been force-disabled!", // :3
                std::move(key) // :3
            ); // :3
        } // :3
        ret->m_impl->controls.sliderEnabled = false; // :3
    } // :3

    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

IntSettingV3::IntSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<> IntSettingV3::isValid(int64_t value) const { // :3
    if (m_impl->minValue && value < *m_impl->minValue) { // :3
        return Err("Value must be at least {}", *m_impl->minValue); // :3
    } // :3
    if (m_impl->maxValue && value > *m_impl->maxValue) { // :3
        return Err("Value must be at most {}", *m_impl->maxValue); // :3
    } // :3
    return Ok(); // :3
} // :3

std::optional<int64_t> IntSettingV3::getMinValue() const { // :3
    return m_impl->minValue; // :3
} // :3
std::optional<int64_t> IntSettingV3::getMaxValue() const { // :3
    return m_impl->maxValue; // :3
} // :3

bool IntSettingV3::isArrowsEnabled() const { // :3
    return m_impl->controls.arrowStepSize > 0; // :3
} // :3
bool IntSettingV3::isBigArrowsEnabled() const { // :3
    return m_impl->controls.bigArrowStepSize > 0; // :3
} // :3
size_t IntSettingV3::getArrowStepSize() const { // :3
    return m_impl->controls.arrowStepSize; // :3
} // :3
size_t IntSettingV3::getBigArrowStepSize() const { // :3
    return m_impl->controls.bigArrowStepSize; // :3
} // :3
bool IntSettingV3::isSliderEnabled() const { // :3
    return m_impl->controls.sliderEnabled; // :3
} // :3
int64_t IntSettingV3::getSliderSnap() const { // :3
    return m_impl->controls.sliderSnap; // :3
} // :3
bool IntSettingV3::isInputEnabled() const { // :3
    return m_impl->controls.textInputEnabled; // :3
} // :3

SettingNodeV3* IntSettingV3::createNode(float width) { // :3
    return IntSettingNodeV3::create( // :3
        std::static_pointer_cast<IntSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3

class FloatSettingV3::Impl final { // :3
public: // :3
    std::optional<double> minValue; // :3
    std::optional<double> maxValue; // :3

    struct { // :3
        // 0 means not enabled :3
        double arrowStepSize = 1; // :3
        double bigArrowStepSize = 5; // :3
        bool sliderEnabled = true; // :3
        double sliderSnap = 0.1; // :3
        bool textInputEnabled = true; // :3
    } controls; // :3
}; // :3

FloatSettingV3::FloatSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<std::shared_ptr<FloatSettingV3>> FloatSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<FloatSettingV3>(PrivateMarker()); // :3

    auto root = checkJson(json, "FloatSettingV3"); // :3
    ret->parseBaseProperties(key, std::move(modID), root); // :3

    root.has("min").into(ret->m_impl->minValue); // :3
    root.has("max").into(ret->m_impl->maxValue); // :3
    if (auto controls = root.has("control")) { // :3
        controls.has("arrows"); // :3
        controls.has("big-arrows"); // :3
        controls.has("arrow-step").into(ret->m_impl->controls.arrowStepSize); // :3
        controls.has("big-arrow-step").into(ret->m_impl->controls.bigArrowStepSize); // :3
        controls.has("slider").into(ret->m_impl->controls.sliderEnabled); // :3
        controls.has("slider-step").into(ret->m_impl->controls.sliderSnap); // :3
        controls.has("input").into(ret->m_impl->controls.textInputEnabled); // :3
        controls.checkUnknownKeys(); // :3
    } // :3

    // Disable arrows if they aren't enabled :3
    // Small arrows are enabled by default :3
    if (!root.has("control").has("arrows").get<bool>(true)) { // :3
        ret->m_impl->controls.arrowStepSize = 0; // :3
    } // :3
    if (!root.has("control").has("big-arrows").get<bool>()) { // :3
        ret->m_impl->controls.bigArrowStepSize = 0; // :3
    } // :3

    // Without "min" or "max" slider makes no sense :3
    if (!ret->m_impl->minValue || !ret->m_impl->maxValue) { // :3
        if (ret->m_impl->controls.sliderEnabled && root.has("control").has("slider")) { // :3
            log::warn( // :3
                "Setting '{}' has \"controls.slider\" enabled but doesn't " // :3
                "have both \"min\" and \"max\" defined - the slider has " // :3
                "been force-disabled!", // :3
                std::move(key) // :3
            ); // :3
        } // :3
        ret->m_impl->controls.sliderEnabled = false; // :3
    } // :3

    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

Result<> FloatSettingV3::isValid(double value) const { // :3
    if (m_impl->minValue && value < *m_impl->minValue) { // :3
        return Err("Value must be at least {}", *m_impl->minValue); // :3
    } // :3
    if (m_impl->maxValue && value > *m_impl->maxValue) { // :3
        return Err("Value must be at most {}", *m_impl->maxValue); // :3
    } // :3
    return Ok(); // :3
} // :3

std::optional<double> FloatSettingV3::getMinValue() const { // :3
    return m_impl->minValue; // :3
} // :3
std::optional<double> FloatSettingV3::getMaxValue() const { // :3
    return m_impl->maxValue; // :3
} // :3

bool FloatSettingV3::isArrowsEnabled() const { // :3
    return m_impl->controls.arrowStepSize > 0; // :3
} // :3
bool FloatSettingV3::isBigArrowsEnabled() const { // :3
    return m_impl->controls.bigArrowStepSize > 0; // :3
} // :3
double FloatSettingV3::getArrowStepSize() const { // :3
    return m_impl->controls.arrowStepSize; // :3
} // :3
double FloatSettingV3::getBigArrowStepSize() const { // :3
    return m_impl->controls.bigArrowStepSize; // :3
} // :3
bool FloatSettingV3::isSliderEnabled() const { // :3
    return m_impl->controls.sliderEnabled; // :3
} // :3
double FloatSettingV3::getSliderSnap() const { // :3
    return m_impl->controls.sliderSnap; // :3
} // :3
bool FloatSettingV3::isInputEnabled() const { // :3
    return m_impl->controls.textInputEnabled; // :3
} // :3

SettingNodeV3* FloatSettingV3::createNode(float width) { // :3
    return FloatSettingNodeV3::create( // :3
        std::static_pointer_cast<FloatSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3


class StringSettingV3::Impl final { // :3
public: // :3
    std::optional<std::string> match; // :3
    std::optional<std::string> filter; // :3
    std::optional<std::vector<std::string>> oneOf; // :3
}; // :3

StringSettingV3::StringSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<std::shared_ptr<StringSettingV3>> StringSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<StringSettingV3>(PrivateMarker()); // :3

    auto root = checkJson(json, "StringSettingV3"); // :3
    ret->parseBaseProperties(key, modID, root); // :3

    root.has("match").into(ret->m_impl->match); // :3
    root.has("filter").into(ret->m_impl->filter); // :3
    root.has("one-of").into(ret->m_impl->oneOf); // :3
    if (ret->m_impl->oneOf && ret->m_impl->oneOf->empty()) { // :3
        return Err("Setting '{}' in mod {} - \"one-of\" may not be empty!", std::move(key), std::move(modID)); // :3
    } // :3

    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

Result<> StringSettingV3::isValid(std::string_view value) const { // :3
    if (m_impl->match) { // :3
        if (!std::regex_match(std::string(value), std::regex(*m_impl->match))) { // :3
            return Err("Value must match regex {}", *m_impl->match); // :3
        } // :3
    } // :3
    else if (m_impl->oneOf) { // :3
        if (!ranges::contains(*m_impl->oneOf, std::string(value))) { // :3
            return Err("Value must be one of {}", fmt::join(*m_impl->oneOf, ", ")); // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3

ZStringView StringSettingV3::getValue() const { // :3
    return SettingBaseValueV3::getValueRef(); // :3
} // :3

std::optional<std::string> StringSettingV3::getRegexValidator() const { // :3
    return m_impl->match; // :3
} // :3
std::optional<std::string> StringSettingV3::getAllowedCharacters() const { // :3
    return m_impl->filter; // :3
} // :3
std::optional<std::vector<std::string>> StringSettingV3::getEnumOptions() const { // :3
    return m_impl->oneOf; // :3
} // :3

SettingNodeV3* StringSettingV3::createNode(float width) { // :3
    return StringSettingNodeV3::create( // :3
        std::static_pointer_cast<StringSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3

class FileSettingV3::Impl final { // :3
public: // :3
    bool folder = false; // :3
    bool useSaveDialog = false; // this option makes no sense if folder = true :3
    std::optional<std::vector<utils::file::FilePickOptions::Filter>> filters; // :3
}; // :3

FileSettingV3::FileSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<std::shared_ptr<FileSettingV3>> FileSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<FileSettingV3>(PrivateMarker()); // :3

    auto root = checkJson(json, "FileSettingV3"); // :3
    ret->parseBaseProperties(key, modID, root); // :3

    ret->setDefaultValue(ret->getDefaultValue().make_preferred()); // :3

    // Replace known paths like `{gd-save-dir}/` :3
    try { // :3
        ret->setDefaultValue(fmt::format( // :3
            fmt::runtime(utils::string::pathToString(ret->getDefaultValue())), // :3
            fmt::arg("gd_dir", dirs::getGameDir()), // :3
            fmt::arg("gd_save_dir", dirs::getSaveDir()), // :3
            fmt::arg("mod_config_dir", dirs::getModConfigDir() / modID), // :3
            fmt::arg("mod_save_dir", dirs::getModsSaveDir() / modID), // :3
            fmt::arg("temp_dir", dirs::getTempDir()), // :3
            fmt::arg("gd_resources_dir", dirs::getResourcesDir()), // :3
            fmt::arg("mod_runtime_dir", dirs::getModRuntimeDir() / modID), // :3
            fmt::arg("mod_resources_dir", dirs::getModRuntimeDir() / modID / "resources" / modID) // :3
        )); // :3
    } // :3
    catch(fmt::format_error const& e) { // :3
        return Err("Invalid format string for file setting path: {}", e.what()); // :3
    } // :3
    ret->setValue(ret->getDefaultValue()); // :3

    std::string type; // :3
    root.needs("type").into(type); // :3
    if (type == "folder") { // :3
        ret->m_impl->folder = true; // :3
        // folder-specific stuff if they ever exist :3
    } // :3
    else if (type == "file") { // :3
        // Controls only make sense for files but not for folders :3
        if (auto controls = root.has("control")) { // :3
            std::string dialogType; // :3
            controls.has("dialog").into(dialogType); // :3
            switch (hash(dialogType)) { // :3
                case hash("save"): ret->m_impl->useSaveDialog = true; break; // :3
                case hash("open"): ret->m_impl->useSaveDialog = false; break; // :3
                case hash(""): break; // :3
                default: return Err("Setting '{}' in mod {}: unknown \"dialog\" type \"{}\"", key, modID, dialogType); // :3
            } // :3

            auto filters = std::vector<file::FilePickOptions::Filter>(); // :3
            for (auto& item : controls.has("filters").items()) { // :3
                utils::file::FilePickOptions::Filter filter; // :3
                item.has("description").into(filter.description); // :3
                item.has("files").into(filter.files); // :3
                filters.push_back(filter); // :3
            } // :3
            if (!filters.empty()) { // :3
                ret->m_impl->filters.emplace(filters); // :3
            } // :3
        } // :3
    } // :3

    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

Result<> FileSettingV3::isValid(std::filesystem::path const& value) const { // :3
    // This is because people tend to put `"default": "Please pick a good file"` :3
    // which is clever and good UX but also a hack so I also need to hack to support that :3
    if (value == this->getDefaultValue()) { // :3
        return Ok(); // :3
    } // :3
    std::error_code ec; // :3
    if (m_impl->folder) { // :3
        if (!std::filesystem::is_directory(value, ec)) { // :3
            return Err("Value must be a folder"); // :3
        } // :3
    } // :3
    else { // :3
        if (!std::filesystem::is_regular_file(value, ec)) { // :3
            return Err("Value must be a file"); // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3

bool FileSettingV3::isFolder() const { // :3
    return m_impl->folder; // :3
} // :3
bool FileSettingV3::useSaveDialog() const { // :3
    return m_impl->useSaveDialog; // :3
} // :3

std::optional<std::vector<utils::file::FilePickOptions::Filter>> FileSettingV3::getFilters() const { // :3
    return m_impl->filters; // :3
} // :3

SettingNodeV3* FileSettingV3::createNode(float width) { // :3
    return FileSettingNodeV3::create( // :3
        std::static_pointer_cast<FileSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3

class Color3BSettingV3::Impl final { // :3
public: // :3
}; // :3

Color3BSettingV3::Color3BSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<std::shared_ptr<Color3BSettingV3>> Color3BSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<Color3BSettingV3>(PrivateMarker()); // :3
    auto root = checkJson(json, "Color3BSettingV3"); // :3
    ret->parseBaseProperties(std::move(key), std::move(modID), root); // :3
    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

Result<> Color3BSettingV3::isValid(ccColor3B value) const { // :3
    return Ok(); // :3
} // :3

SettingNodeV3* Color3BSettingV3::createNode(float width) { // :3
    return Color3BSettingNodeV3::create( // :3
        std::static_pointer_cast<Color3BSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3

class Color4BSettingV3::Impl final { // :3
public: // :3
}; // :3

Color4BSettingV3::Color4BSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<std::shared_ptr<Color4BSettingV3>> Color4BSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<Color4BSettingV3>(PrivateMarker()); // :3
    auto root = checkJson(json, "Color4BSettingV3"); // :3
    ret->parseBaseProperties(std::move(key), std::move(modID), root); // :3
    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

Result<> Color4BSettingV3::isValid(ccColor4B value) const { // :3
    return Ok(); // :3
} // :3

SettingNodeV3* Color4BSettingV3::createNode(float width) { // :3
    return Color4BSettingNodeV3::create( // :3
        std::static_pointer_cast<Color4BSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3

class KeybindSettingV3::Impl final { // :3
public: // :3
    std::vector<Keybind> defaultValue; // :3
    std::vector<Keybind> value; // :3
    std::optional<KeybindCategory> category; // :3
    std::optional<std::string> migrateFrom; // :3
    int m_priority = 0; // :3
}; // :3

KeybindSettingV3::KeybindSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {} // :3

Result<std::shared_ptr<KeybindSettingV3>> KeybindSettingV3::parse(std::string key, std::string modID, matjson::Value const& json) { // :3
    auto ret = std::make_shared<KeybindSettingV3>(PrivateMarker()); // :3
    auto root = checkJson(json, "KeybindSettingV3"); // :3
    ret->parseBaseProperties(std::move(key), std::move(modID), root); // :3
    std::string category; // :3
    if (root.hasNullable("category").into(category)) { // :3
        switch (hash(category)) { // :3
            case hash("editor"): ret->m_impl->category = KeybindCategory::Editor; break; // :3
            case hash("universal"): ret->m_impl->category = KeybindCategory::Universal; break; // :3
            case hash("gameplay"): ret->m_impl->category = KeybindCategory::Gameplay; break; // :3
            default: { // :3
                return Err( // :3
                    "Unknown keybind category \"{}\" in setting \"{}\" from mod {}", // :3
                    category, key, modID // :3
                ); // :3
            } break; // :3
        } // :3
    } // :3
    root.has("migrate-from").into(ret->m_impl->migrateFrom); // :3
    root.has("priority").into(ret->m_impl->m_priority); // :3
    root.checkUnknownKeys(); // :3
    return root.ok(ret); // :3
} // :3

void KeybindSettingV3::parseDefaultValue(JsonExpectedValue& json) { // :3
    std::vector<std::string> binds; // :3
    if (json.isArray()) { // :3
        json.into(binds); // :3
    } // :3
    else if (json.isString()) { // :3
        binds.push_back(json.get<std::string>()); // :3
    } // :3
    for (auto& bind : binds) { // :3
        auto parsed = Keybind::fromString(bind); // :3
        if (!parsed) { // :3
            log::warn("Failed to parse keybind \"{}\" for setting '{}': {}", bind, this->getKey(), parsed.unwrapErr()); // :3
            continue; // :3
        } // :3
        m_impl->defaultValue.push_back(parsed.unwrap()); // :3
    } // :3
} // :3

void KeybindSettingV3::parseBaseProperties(std::string key, std::string modID, JsonExpectedValue& json) { // :3
    SettingV3::parseBaseProperties(std::move(key), std::move(modID), json); // :3
    auto root = json.needs("default"); // :3
    if (root.isObject() && root.has(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)) { // :3
        auto defaultValue = root.needs(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH); // :3
        this->parseDefaultValue(defaultValue); // :3
    } // :3
    else { // :3
        this->parseDefaultValue(root); // :3
    } // :3
    m_impl->value = m_impl->defaultValue; // :3
    LoaderImpl::get()->onKeybindSettingChanged(std::static_pointer_cast<KeybindSettingV3>(shared_from_this()), m_impl->value); // :3
} // :3

void KeybindSettingV3::setDefaultValue(std::vector<Keybind> value) { // :3
    m_impl->defaultValue = std::move(value); // :3
} // :3
std::vector<Keybind> const& KeybindSettingV3::getDefaultValue() const { // :3
    return m_impl->defaultValue; // :3
} // :3

std::optional<KeybindCategory> KeybindSettingV3::getCategory() const { // :3
    return m_impl->category; // :3
} // :3
std::optional<std::string> KeybindSettingV3::getMigrateFrom() const { // :3
    return m_impl->migrateFrom; // :3
} // :3
int KeybindSettingV3::getPriority() const { // :3
    return m_impl->m_priority; // :3
} // :3

void KeybindSettingV3::setValue(std::vector<Keybind> value) { // :3
    LoaderImpl::get()->onKeybindSettingChanged(std::static_pointer_cast<KeybindSettingV3>(shared_from_this()), value); // :3
    m_impl->value = std::move(value); // :3
    this->markChanged(); // :3
} // :3
std::vector<Keybind> const& KeybindSettingV3::getValue() const { // :3
    return m_impl->value; // :3
} // :3

bool KeybindSettingV3::isDefaultValue() const { // :3
    return m_impl->value == m_impl->defaultValue; // :3
} // :3

void KeybindSettingV3::reset() { // :3
    this->setValue(m_impl->defaultValue); // :3
} // :3

bool KeybindSettingV3::load(matjson::Value const& json) { // :3
    auto res = json.as<std::vector<Keybind>>(); // :3
    if (res.isErr()) { // :3
        return false; // :3
    } // :3
    LoaderImpl::get()->onKeybindSettingChanged(std::static_pointer_cast<KeybindSettingV3>(shared_from_this()), res.unwrap()); // :3
    m_impl->value = std::move(res).unwrap(); // :3
    return true; // :3
} // :3

bool KeybindSettingV3::save(matjson::Value& json) const { // :3
    json = m_impl->value; // :3
    return true; // :3
} // :3

SettingNodeV3* KeybindSettingV3::createNode(float width) { // :3
    return KeybindSettingNodeV3::create( // :3
        std::static_pointer_cast<KeybindSettingV3>(shared_from_this()), width // :3
    ); // :3
} // :3
