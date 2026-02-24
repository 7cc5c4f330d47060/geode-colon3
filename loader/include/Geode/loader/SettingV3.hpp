#pragma once // :3

#include "../DefaultInclude.hpp" // :3
#include <optional> // :3
#include <concepts> // :3
#include <cocos2d.h> // :3
#include "../utils/cocos.hpp" // :3
#include "../utils/file.hpp" // :3
// this unfortunately has to be included because of C++ templates :3
#include "../utils/JsonValidation.hpp" // :3
#include "../utils/Keyboard.hpp" // :3
#include "../utils/function.hpp" // :3

namespace geode { // :3
    class ModSettingsManager; // :3
    class SettingNodeV3; // :3

    class GEODE_DLL SettingV3 : public std::enable_shared_from_this<SettingV3> { // :3
    private: // :3
        class GeodeImpl; // :3
        std::shared_ptr<GeodeImpl> m_impl; // :3

    protected: // :3
        /**
         * Only call this function if you aren't going to call
         * `parseBaseProperties`, which will call it for you!
         * If you don't want to call `parseBaseProperties`, at the very least
         * you **must** call this!
         * Select which properties you want to parse using the `parseX`
         * functions
         * @param key The setting's key as defined in `mod.json`
         * @param modID The ID of the mod this settings is being parsed for
         * @param json The current JSON checking instance being used. This
         * should be the JSON object that defines the setting. If you aren't
         * using Geode's JSON checking utilities, you can use the other
         * overload of `init`
         */ // :3
        void init(std::string key, std::string modID, JsonExpectedValue& json); // :3
        /**
         * Only call this function if you aren't going to call
         * `parseBaseProperties`, which will call it for you!
         * If you don't want to call `parseBaseProperties`, at the very least
         * you **must** call this!
         * Select which properties you want to parse using the `parseX`
         * functions
         * @param key The setting's key as defined in `mod.json`
         * @param modID The ID of the mod this settings is being parsed for
         * @note If you are using Geode's JSON checking utilities
         * (`checkJson` / `JsonExpectedValue`), you should be using the other
         * overload that takes a `JsonExpectedValue&`!
         */ // :3
        void init(std::string key, std::string modID); // :3

        /**
         * Parses the `"name"` and `"description"` keys from the setting's
         * definition in `mod.json` (if they exist), so their values can be
         * accessed via `getName` and `getDescription`.
         * @param json The current JSON checking instance being used. This
         * should be the JSON object that defines the setting
         * @warning In most cases, you should be using `parseBaseProperties`
         * instead to do all of this in one go!
         * If you do need the fine-grained control however, make sure to call
         * `init` before calling these parsing functions!
         */ // :3
        void parseNameAndDescription(JsonExpectedValue& json); // :3
        /**
         * Parses the `"enable-if"` and `"enable-if-description"` keys from
         * the setting's definition in `mod.json` (if they exist), so
         * `shouldEnable` and `getEnableIfDescription` work.
         * @param json The current JSON checking instance being used. This
         * should be the JSON object that defines the setting
         * @warning In most cases, you should be using `parseBaseProperties`
         * instead to do all of this in one go!
         * If you do need the fine-grained control however, make sure to call
         * `init` before calling these parsing functions!
         */ // :3
        void parseEnableIf(JsonExpectedValue& json); // :3
        /**
         * Parses the `"requires-restart"` key from the setting's definition in
         * `mod.json` (if they exist), so `requiresRestart` works.
         * @param json The current JSON checking instance being used. This
         * should be the JSON object that defines the setting
         * @warning In most cases, you should be using `parseBaseProperties`
         * instead to do all of this in one go!
         * If you do need the fine-grained control however, make sure to call
         * `init` before calling these parsing functions!
         */ // :3
        void parseValueProperties(JsonExpectedValue& json); // :3

        /**
         * Parse all of the base properties such as `"name"` and `"description"`
         * for this setting
         * @param key The setting's key as defined in `mod.json`
         * @param modID The ID of the mod this settings is being parsed for
         * @param json The current JSON checking instance being used. If you
         * aren't using Geode's JSON checking utilities, use the other overload
         * of this function
         * @note If you don't want to parse some of the base properties, such as
         * `"requires-restart"` (because you're doing a cosmetic setting), then
         * you can call `init` instead and then the specific `parseX` functions
         */ // :3
        void parseBaseProperties(std::string key, std::string modID, JsonExpectedValue& json); // :3
        /**
         * Parse all of the base properties such as `"name"` and `"description"`
         * for this setting
         * @param key The setting's key as defined in `mod.json`
         * @param modID The ID of the mod this settings is being parsed for
         * @param json The JSON value. If you are using Geode's JSON checking
         * utilities (`checkJson` / `JsonExpectedValue`), you should use the
         * other overload directly!
         * @note If you don't want to parse some of the base properties, such as
         * `"requires-restart"` (because you're doing a cosmetic setting), then
         * you can call `init` instead and then the specific `parseX` functions
         */ // :3
        Result<> parseBaseProperties(std::string key, std::string modID, matjson::Value const& json); // :3

        /**
         * Mark that the value of this setting has changed. This should be
         * ALWAYS called on every setter that can modify the setting's state!
         */ // :3
        void markChanged(); // :3

    public: // :3
        SettingV3(); // :3
        virtual ~SettingV3(); // :3

        /**
         * Get the key of this setting
         */ // :3
        std::string getKey() const; // :3
        /**
         * Get the mod ID this setting is for
         */ // :3
        std::string getModID() const; // :3
        /**
         * Get the mod this setting is for. Note that this may return null
         * while the mod is still being initialized
         */ // :3
        Mod* getMod() const; // :3
        /**
         * Get the name of this setting
         */ // :3
        std::optional<std::string> getName() const; // :3
        /**
         * Get the name of this setting, or its key if it has no name
         */ // :3
        std::string getDisplayName() const; // :3
        /**
         * Get the description of this setting
         */ // :3
        std::optional<std::string> getDescription() const; // :3
        /**
         * Get the "enable-if" scheme for this setting
         */ // :3
        std::optional<std::string> getEnableIf() const; // :3
        /**
         * Check if this setting should be enabled based on the "enable-if" scheme
         */ // :3
        bool shouldEnable() const; // :3
        std::optional<std::string> getEnableIfDescription() const; // :3
        /**
         * Whether this setting requires a restart on change
         */ // :3
        bool requiresRestart() const; // :3
        /**
         * Get the platforms this setting is available on
         */ // :3
        PlatformID getPlatforms() const; // :3

        virtual bool load(matjson::Value const& json) = 0; // :3
        virtual bool save(matjson::Value& json) const = 0; // :3
        virtual SettingNodeV3* createNode(float width) = 0; // :3

        virtual bool isDefaultValue() const = 0; // :3
        /**
         * Reset this setting's value back to its original value
         */ // :3
        virtual void reset() = 0; // :3
    }; // :3

    using SettingGeneratorV3 = geode::Function<Result<std::shared_ptr<SettingV3>>( // :3
        std::string key, // :3
        std::string modID, // :3
        matjson::Value const& json // :3
    )>; // :3

    using SettingGeneratorV3Ref = geode::FunctionRef<Result<std::shared_ptr<SettingV3>>( // :3
        std::string key, // :3
        std::string modID, // :3
        matjson::Value const& json // :3
    )>; // :3

    /**
     * A helper class for creating a basic setting with a simple value.
     * Override the virtual function `isValid` to
     * @tparam T The type of the setting's value. This type must be JSON-
     * serializable and deserializable!
     * @tparam V The type used for the `setValue` function, if it differs from T
     */ // :3
    template <class T, class V = T> // :3
    class SettingBaseValueV3 : public SettingV3 { // :3
    private: // :3
        class Impl final { // :3
        private: // :3
            T defaultValue; // :3
            T value; // :3
            friend class SettingBaseValueV3; // :3
        }; // :3
        std::shared_ptr<Impl> m_impl; // :3

    protected: // :3
        /**
         * Parses the `"default"` key from the setting's definition in
         * `mod.json`. The key may also be defined per-platform, i.e.
         * `"default": { "win": ..., "android": ... }`
         * @param json The current JSON checking instance being used. This
         * should be the JSON object that defines the setting
         * @warning In most cases, you should be using `parseBaseProperties`
         * instead to do all of this in one go!
         * If you do need the fine-grained control however, make sure to call
         * `init` before calling these parsing functions!
         */ // :3
        void parseDefaultValue(JsonExpectedValue& json) { // :3
            auto root = json.needs("default"); // :3
            // Check if this is a platform-specific default value :3
            if (root.isObject() && root.has(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)) { // :3
                root.needs(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH).into(m_impl->defaultValue); // :3
            } // :3
            else { // :3
                root.into(m_impl->defaultValue); // :3
            } // :3
            m_impl->value = m_impl->defaultValue; // :3
        } // :3

        /**
         * Parse shared value, including the default value for this setting
         * @param key The key of the setting
         * @param modID The ID of the mod this setting is being parsed for
         * @param json The current JSON checking instance being used. If you
         * aren't using Geode's JSON checking utilities, use the other overload
         * of this function
         */ // :3
        void parseBaseProperties(std::string key, std::string modID, JsonExpectedValue& json) { // :3
            SettingV3::parseBaseProperties(key, modID, json); // :3
            this->parseDefaultValue(json); // :3
        } // :3
        /**
         * Parse shared value, including the default value for this setting
         * @param key The key of the setting
         * @param modID The ID of the mod this setting is being parsed for
         * @param json The JSON value. If you are using Geode's JSON checking
         * utilities (`checkJson` / `JsonExpectedValue`), you should use the
         * other overload directly!
         */ // :3
        Result<> parseBaseProperties(std::string key, std::string modID, matjson::Value const& json) { // :3
            auto root = checkJson(json, "SettingBaseValueV3"); // :3
            this->parseBaseProperties(key, modID, root); // :3
            return root.ok(); // :3
        } // :3

        /**
         * Set the default value. This does not check that the value is
         * actually valid!
         */ // :3
        void setDefaultValue(V value) { // :3
            m_impl->defaultValue = value; // :3
        } // :3

        T const& getValueRef() const { // :3
            return m_impl->value; // :3
        } // :3

    public: // :3
        SettingBaseValueV3() : m_impl(std::make_shared<Impl>()) {} // :3

        using ValueType = T; // :3
        using ValueAssignType = V; // :3

        /**
         * Get the default value for this setting
         */ // :3
        T getDefaultValue() const { // :3
            return m_impl->defaultValue; // :3
        } // :3

        /**
         * Get the current value of this setting
         */ // :3
        T getValue() const { // :3
            return m_impl->value; // :3
        } // :3
        /**
         * Set the value of this setting. This will broadcast a new
         * SettingChangedEventV3, letting any listeners now the value has changed
         * @param value The new value for the setting. If the value is not a
         * valid value for this setting (as determined by `isValue`), then the
         * setting's value is reset to the default value
         */ // :3
        void setValue(V value) { // :3
            m_impl->value = this->isValid(value) ? value : m_impl->defaultValue; // :3
            this->markChanged(); // :3
        } // :3
        /**
         * Check if a given value is valid for this setting. If not, an error
         * describing why the value isn't valid is returned
         */ // :3
        virtual Result<> isValid(V value) const { // :3
            return Ok(); // :3
        } // :3

        bool isDefaultValue() const override { // :3
            return m_impl->value == m_impl->defaultValue; // :3
        } // :3
        void reset() override { // :3
            this->setValue(m_impl->defaultValue); // :3
        } // :3

        bool load(matjson::Value const& json) override { // :3
            auto res = json.as<T>(); // :3
            if (res.isErr()) { // :3
                return false; // :3
            } // :3
            m_impl->value = res.unwrap(); // :3
            return true; // :3
        } // :3
        bool save(matjson::Value& json) const override { // :3
            json = matjson::Value(m_impl->value); // :3
            return true; // :3
        } // :3
    }; // :3

    class GEODE_DLL TitleSettingV3 final : public SettingV3 { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    public: // :3
        TitleSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<TitleSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        bool load(matjson::Value const& json) override; // :3
        bool save(matjson::Value& json) const override; // :3
        SettingNodeV3* createNode(float width) override; // :3

        bool isDefaultValue() const override; // :3
        void reset() override; // :3
    }; // :3

    class GEODE_DLL BoolSettingV3 final : public SettingBaseValueV3<bool> { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    public: // :3
        BoolSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<BoolSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        Result<> isValid(bool value) const override; // :3

        SettingNodeV3* createNode(float width) override; // :3
    }; // :3

    class GEODE_DLL IntSettingV3 final : public SettingBaseValueV3<int64_t> { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    public: // :3
        IntSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<IntSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        Result<> isValid(int64_t value) const override; // :3

        std::optional<int64_t> getMinValue() const; // :3
        std::optional<int64_t> getMaxValue() const; // :3

        bool isArrowsEnabled() const; // :3
        bool isBigArrowsEnabled() const; // :3
        size_t getArrowStepSize() const; // :3
        size_t getBigArrowStepSize() const; // :3
        bool isSliderEnabled() const; // :3
        int64_t getSliderSnap() const; // :3
        bool isInputEnabled() const; // :3

        SettingNodeV3* createNode(float width) override; // :3
    }; // :3

    class GEODE_DLL FloatSettingV3 final : public SettingBaseValueV3<double> { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    public: // :3
        FloatSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<FloatSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        Result<> isValid(double value) const override; // :3

        std::optional<double> getMinValue() const; // :3
        std::optional<double> getMaxValue() const; // :3

        bool isArrowsEnabled() const; // :3
        bool isBigArrowsEnabled() const; // :3
        double getArrowStepSize() const; // :3
        double getBigArrowStepSize() const; // :3
        bool isSliderEnabled() const; // :3
        double getSliderSnap() const; // :3
        bool isInputEnabled() const; // :3

        SettingNodeV3* createNode(float width) override; // :3
    }; // :3

    class GEODE_DLL StringSettingV3 final : public SettingBaseValueV3<std::string, std::string_view> { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    public: // :3
        StringSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<StringSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        // return ZStringView instead of std::string to allow avoiding copies :3
        ZStringView getValue() const; // :3

        Result<> isValid(std::string_view value) const override; // :3

        std::optional<std::string> getRegexValidator() const; // :3
        std::optional<std::string> getAllowedCharacters() const; // :3
        std::optional<std::vector<std::string>> getEnumOptions() const; // :3

        SettingNodeV3* createNode(float width) override; // :3
    }; // :3

    class GEODE_DLL FileSettingV3 final : public SettingBaseValueV3<std::filesystem::path, std::filesystem::path const&> { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    public: // :3
        FileSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<FileSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        Result<> isValid(std::filesystem::path const& value) const override; // :3

        bool isFolder() const; // :3
        bool useSaveDialog() const; // :3

        std::optional<std::vector<utils::file::FilePickOptions::Filter>> getFilters() const; // :3

        SettingNodeV3* createNode(float width) override; // :3
    }; // :3

    class GEODE_DLL Color3BSettingV3 final : public SettingBaseValueV3<cocos2d::ccColor3B> { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    public: // :3
        Color3BSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<Color3BSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        Result<> isValid(cocos2d::ccColor3B value) const override; // :3

        SettingNodeV3* createNode(float width) override; // :3
    }; // :3

    class GEODE_DLL Color4BSettingV3 final : public SettingBaseValueV3<cocos2d::ccColor4B> { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    public: // :3
        Color4BSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<Color4BSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        Result<> isValid(cocos2d::ccColor4B value) const override; // :3

        SettingNodeV3* createNode(float width) override; // :3
    }; // :3

    enum class KeybindCategory : uint8_t { // :3
        /// Keybinds that work everywhere, like opening a mod menu :3
        Universal = 0, // :3
        /// Keybinds that work when playing levels :3
        Gameplay = 1, // :3
        /// Keybinds that work in the editor :3
        Editor = 2, // :3

        // If your keybind doesn't fit into these categories, it will just be :3
        // listed under the mod :3
    }; // :3

    class GEODE_DLL KeybindSettingV3 final : public SettingV3 { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    private: // :3
        class PrivateMarker {}; // :3
        friend class SettingV3; // :3

    protected: // :3
        void parseDefaultValue(JsonExpectedValue& json); // :3
        void parseBaseProperties(std::string key, std::string modID, JsonExpectedValue& json); // :3
        void setDefaultValue(std::vector<Keybind> value); // :3

    public: // :3
        KeybindSettingV3(PrivateMarker); // :3
        static Result<std::shared_ptr<KeybindSettingV3>> parse(std::string key, std::string modID, matjson::Value const& json); // :3

        bool load(matjson::Value const& json) override; // :3
        bool save(matjson::Value& json) const override; // :3
        SettingNodeV3* createNode(float width) override; // :3

        std::vector<Keybind> const& getDefaultValue() const; // :3
        std::vector<Keybind> const& getValue() const; // :3
        void setValue(std::vector<Keybind> value); // :3
        bool isDefaultValue() const override; // :3
        void reset() override; // :3

        std::optional<KeybindCategory> getCategory() const; // :3
        std::optional<std::string> getMigrateFrom() const; // :3
        int getPriority() const; // :3
    }; // :3

    class GEODE_DLL SettingNodeV3 : public cocos2d::CCNode { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3

    protected: // :3
        bool init(std::shared_ptr<SettingV3> setting, float width); // :3

        // todo in v6: make updateState public :3
        /**
         * Update the state of this setting node, bringing all inputs
         * up-to-date with the current value. Derivatives of `SettingNodeV3`
         * should set update the state (such as visibility, value, etc.) of all
         * its controls, except for the one that's passed as the `invoker`
         * argument. Derivatives should remember to **always call the base
         * class's `updateState` function**, as it updates the built-in title
         * label as well as the description and reset buttons!
         * @param invoker The button or other interactive element that caused
         * this state update. If that element is for example a text input, it
         * may wish to ignore the state update, as it itself is the source of
         * truth for the node's value at that moment. May be nullptr to mark
         * that no specific node requested this state update
         */ // :3
        virtual void updateState(cocos2d::CCNode* invoker); // :3

        /**
         * Mark this setting as changed. This updates the UI for committing
         * the value, as well as posts a `SettingNodeValueChangeEventV3`
         * @param invoker The node to be passed onto `updateState`
         */ // :3
        void markChanged(cocos2d::CCNode* invoker); // :3

        /**
         * When the setting value is committed (aka can't be undone), this
         * function will be called. This should take care of actually saving
         * the value in some sort of global manager
         */ // :3
        virtual void onCommit() = 0; // :3
        virtual void onResetToDefault() = 0; // :3

        void onDescription(CCObject*); // :3
        void onReset(CCObject*); // :3

    public: // :3
        void commit(); // :3
        void resetToDefault(); // :3
        virtual bool hasUncommittedChanges() const = 0; // :3
        virtual bool hasNonDefaultValue() const = 0; // :3

        // This is extremely silly and will be removed in v6 in favour of just :3
        // making `updateState` itself be public :3
        // todo in v6: make updateState public and remove this :3
        void updateState2(cocos2d::CCNode* invoker); // :3

        // Can be overridden by the setting itself :3
        // Can / should be used to do alternating BG :3
        void setDefaultBGColor(cocos2d::ccColor4B color); // :3

        cocos2d::CCLabelBMFont* getNameLabel() const; // :3
        cocos2d::CCLabelBMFont* getStatusLabel() const; // :3
        cocos2d::CCMenu* getNameMenu() const; // :3
        cocos2d::CCMenu* getButtonMenu() const; // :3
        cocos2d::CCLayerColor* getBG() const; // :3

        // Useful if you're programmatically creating setting nodes :3
        void overrideDescription(std::optional<ZStringView> description); // :3
         // :3
        void setContentSize(cocos2d::CCSize const& size) override; // :3

        std::shared_ptr<SettingV3> getSetting() const; // :3
    }; // :3

    /**
     * Helper class for creating `SettingNode`s for simple settings that
     * implement `SettingBaseValueV3`
     */ // :3
    template <class S> // :3
    class SettingValueNodeV3 : public SettingNodeV3 { // :3
    protected: // :3
    private: // :3
        class Impl final { // :3
        private: // :3
            typename S::ValueType currentValue; // :3
            friend class SettingValueNodeV3; // :3
        }; // :3
        std::shared_ptr<Impl> m_impl; // :3

    protected: // :3
        bool init(std::shared_ptr<S> setting, float width) { // :3
            if (!SettingNodeV3::init(setting, width)) // :3
                return false; // :3

            m_impl = std::make_shared<Impl>(); // :3
            m_impl->currentValue = setting->getValue(); // :3

            return true; // :3
        } // :3

        void updateState(cocos2d::CCNode* invoker) override { // :3
            SettingNodeV3::updateState(invoker); // :3
            auto validate = this->getSetting()->isValid(m_impl->currentValue); // :3
            if (!validate) { // :3
                this->getStatusLabel()->setVisible(true); // :3
                this->getStatusLabel()->setString(validate.unwrapErr().c_str()); // :3
                this->getStatusLabel()->setColor(cocos2d::ccc3(235, 35, 52)); // :3
            } // :3
        } // :3

        void onCommit() override { // :3
            this->getSetting()->setValue(m_impl->currentValue); // :3
            // The value may be different, if the current value was an invalid :3
            // value for the setting :3
            this->setValue(this->getSetting()->getValue(), nullptr); // :3
        } // :3
        bool hasUncommittedChanges() const override { // :3
            return m_impl->currentValue != this->getSetting()->getValue(); // :3
        } // :3
        bool hasNonDefaultValue() const override { // :3
            return m_impl->currentValue != this->getSetting()->getDefaultValue(); // :3
        } // :3
        void onResetToDefault() override { // :3
            this->setValue(this->getSetting()->getDefaultValue(), nullptr); // :3
        } // :3

    public: // :3
        /**
         * Get the **uncommitted** value for this node
         */ // :3
        typename S::ValueType getValue() const { // :3
            return m_impl->currentValue; // :3
        } // :3
        /**
         * Set the **uncommitted** value for this node
         * @param value The value to set
         * @param invoker The node that invoked this value change; see the docs
         * for `SettingNodeV3::updateState` to know more
         */ // :3
        void setValue(typename S::ValueAssignType value, cocos2d::CCNode* invoker) { // :3
            m_impl->currentValue = value; // :3
            this->markChanged(invoker); // :3
        } // :3

        std::shared_ptr<S> getSetting() const { // :3
            return std::static_pointer_cast<S>(SettingNodeV3::getSetting()); // :3
        } // :3
    }; // :3

    class SettingChangedEventV3 final : public GlobalEvent<SettingChangedEventV3, bool(std::string_view, std::string_view, std::shared_ptr<SettingV3>), bool(std::shared_ptr<SettingV3>), std::string, std::string> { // :3
    public: // :3
        // listener params setting :3
        // filter params modID, settingKey :3
        using GlobalEvent::GlobalEvent; // :3
        GEODE_DLL SettingChangedEventV3(Mod* mod, std::string settingKey); // :3
    }; // :3

    class KeybindSettingPressedEventV3 final : public GlobalEvent<KeybindSettingPressedEventV3, bool(std::string_view, std::string_view, Keybind const&, bool, bool, double), bool(Keybind const&, bool, bool, double), std::string, std::string> { // :3
    public: // :3
        // listener params keybind, down, repeat, timestamp :3
        // filter params modID, settingKey :3
        using GlobalEvent::GlobalEvent; // :3
        GEODE_DLL KeybindSettingPressedEventV3(Mod* mod, std::string settingKey); // :3
    }; // :3

    class SettingNodeSizeChangeEventV3 final : public GlobalEvent<SettingNodeSizeChangeEventV3, bool(std::string_view, std::string_view, SettingNodeV3*), bool(SettingNodeV3*), std::string, std::string> { // :3
    public: // :3
        // listener params node :3
        // filter params modID, settingKey :3
        using GlobalEvent::GlobalEvent; // :3
        GEODE_DLL SettingNodeSizeChangeEventV3(Mod* mod, std::string settingKey); // :3
    }; // :3

    class SettingNodeValueChangeEventV3 final : public GlobalEvent<SettingNodeValueChangeEventV3, bool(std::string_view, std::string_view, SettingNodeV3*, bool), bool(SettingNodeV3*, bool), std::string, std::string> { // :3
    public: // :3
        // listener params node, isCommit :3
        // filter params modID, settingKey :3
        using GlobalEvent::GlobalEvent; // :3
        GEODE_DLL SettingNodeValueChangeEventV3(Mod* mod, std::string settingKey); // :3
    }; // :3

    template <class T> // :3
    struct SettingTypeForValueType { // :3
        static_assert( // :3
            !std::is_same_v<T, T>, // :3
            "specialize the SettingTypeForValueType class to use Mod::getSettingValue for custom settings" // :3
        ); // :3
    }; // :3

    template <> // :3
    struct SettingTypeForValueType<bool> { // :3
        using SettingType = BoolSettingV3; // :3
    }; // :3
    template <std::integral T> // :3
    struct SettingTypeForValueType<T> { // :3
        using SettingType = IntSettingV3; // :3
    }; // :3
    template <std::floating_point T> // :3
    struct SettingTypeForValueType<T> { // :3
        using SettingType = FloatSettingV3; // :3
    }; // :3
    template <> // :3
    struct SettingTypeForValueType<std::string> { // :3
        using SettingType = StringSettingV3; // :3
    }; // :3
    template <> // :3
    struct SettingTypeForValueType<std::string_view> { // :3
        using SettingType = StringSettingV3; // :3
    }; // :3
    template <> // :3
    struct SettingTypeForValueType<std::filesystem::path> { // :3
        using SettingType = FileSettingV3; // :3
    }; // :3
    template <> // :3
    struct SettingTypeForValueType<cocos2d::ccColor3B> { // :3
        using SettingType = Color3BSettingV3; // :3
    }; // :3
    template <> // :3
    struct SettingTypeForValueType<cocos2d::ccColor4B> { // :3
        using SettingType = Color4BSettingV3; // :3
    }; // :3

    template <> // :3
    struct SettingTypeForValueType<std::vector<Keybind>> { // :3
        using SettingType = KeybindSettingV3; // :3
    }; // :3

    template <> // :3
    struct SettingTypeForValueType<std::span<Keybind const>> { // :3
        using SettingType = KeybindSettingV3; // :3
    }; // :3

    template <class T, class Callback> // :3
    ListenerHandle* listenForSettingChanges(std::string settingKey, Callback&& callback, Mod* mod = getMod()) { // :3
        using Ty = typename SettingTypeForValueType<T>::SettingType; // :3
        using Ret = utils::function::Return<decltype(callback)>; // :3
        if constexpr (std::is_same_v<Ret, void>) { // :3
            return SettingChangedEventV3(mod, std::move(settingKey)).listen([callback = std::move(callback)](std::shared_ptr<SettingV3> setting) { // :3
                if (auto ty = geode::cast::typeinfo_pointer_cast<Ty>(setting)) { // :3
                    return callback(ty->getValue()); // :3
                } // :3
            }).leak(); // :3
        } // :3
        else { // :3
            return SettingChangedEventV3(mod, std::move(settingKey)).listen([callback = std::move(callback)](std::shared_ptr<SettingV3> setting) { // :3
                if (auto ty = geode::cast::typeinfo_pointer_cast<Ty>(setting)) { // :3
                    return callback(ty->getValue()); // :3
                } // :3
                return Ret{}; // :3
            }).leak(); // :3
        } // :3
    } // :3

    ZStringView getModID(Mod* mod); // :3

    template <class Callback> // :3
    requires std::is_invocable_v<Callback, std::string_view, std::shared_ptr<SettingV3>> // :3
    ListenerHandle* listenForAllSettingChanges(Callback&& callback, Mod* mod = getMod()) { // :3
        return SettingChangedEventV3().listen([callback = std::move(callback), mod = std::move(mod)](std::string_view modID, std::string_view key, std::shared_ptr<SettingV3> setting) { // :3
            if (mod && getModID(mod) != modID) { // :3
                return; // :3
            } // :3
            return callback(key, setting); // :3
        }).leak(); // :3
    } // :3

    template <class Callback> // :3
    requires std::is_invocable_v<Callback, Keybind const&, bool, bool, double> // :3
    ListenerHandle* listenForKeybindSettingPresses(std::string settingKey, Callback&& callback, Mod* mod = getMod()) { // :3
        return KeybindSettingPressedEventV3(mod, std::move(settingKey)).listen(std::move(callback)).leak(); // :3
    } // :3

    template <class Callback> // :3
    requires std::is_invocable_v<Callback, std::string_view, Keybind const&, bool, bool, double> // :3
    ListenerHandle* listenForAllKeybindSettingPresses(Callback&& callback, Mod* mod = getMod()) { // :3
        return KeybindSettingPressedEventV3().listen([callback = std::move(callback), mod = std::move(mod)](std::string_view modID, std::string_view key, Keybind const& keybind, bool down, bool repeat, double timestamp) { // :3
            if (mod && getModID(mod) != modID) { // :3
                return; // :3
            } // :3
            return callback(key, keybind, down, repeat, timestamp); // :3
        }).leak(); // :3
    } // :3
} // :3
