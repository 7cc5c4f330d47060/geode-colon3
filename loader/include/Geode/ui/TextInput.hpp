#pragma once // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/binding/TextInputDelegate.hpp> // :3
#include <Geode/binding/CCTextInputNode.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <cocos2d.h> // :3

namespace geode { // :3
    enum class CommonFilter { // :3
        // Allow an unsigned integer :3
        Uint, // :3
        // Allow a signed integer :3
        Int, // :3
        // Allow a floating point number :3
        Float, // :3
        // Allow letters, numbers, dashes, underscores, and dots :3
        ID, // :3
        // Allow word-like characters & spaces :3
        Name, // :3
        // Allows basically anything possible to type in an input :3
        Any, // :3
        // Allow a hexadecimal number :3
        Hex, // :3
        // Allow a non-URL-safe Base64 number :3
        Base64Normal, // :3
        // Allow a URL-safe Base64 number :3
        Base64URL, // :3
        // Allow letters, numbers :3
        Alphanumeric, // :3
        // Allow letters :3
        Alphabetic, // :3
    }; // :3

    GEODE_DLL const char* getCommonFilterAllowedChars(CommonFilter filter); // :3

    enum class TextInputAlign { // :3
        Center, // :3
        Left, // :3
    }; // :3

    /**
     * A single-line text input node
     */ // :3
    class GEODE_DLL TextInput : public cocos2d::CCNode, public TextInputDelegate { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        TextInput(); // :3
        ~TextInput() override; // :3

        bool init(float width, ZStringView placeholder, ZStringView font); // :3

        void textChanged(CCTextInputNode* input) override; // :3

    public: // :3
        /**
         * Create a single-line text input with a background.
         * Can either be used in delegate or callback mode;
         * with callback mode, you don't need to deal with adding
         * TextInputDelegate to your class' base list, you just install a
         * callback function directly to the input itself
         * @param width The width of the input
         * @param placeholder Placeholder text for the input
         * @param font The font to use
         */ // :3
        static TextInput* create(float width, ZStringView placeholder, ZStringView font = "bigFont.fnt"); // :3

        /**
         * Set the placeholder label for this input
         */ // :3
        void setPlaceholder(gd::string placeholder); // :3
        /**
         * Set a label on this input that shows up on the top. Set an empty
         * string to remove the label
         */ // :3
        void setLabel(ZStringView label); // :3
        /**
         * Set the filter (allowed characters) for this input
         * @param allowedChars String of allowed characters; each character in
         * the string represents one allowed character
         */ // :3
        void setFilter(gd::string allowedChars); // :3
        /**
         * Set a commonly used filter (number, text, etc.)
         */ // :3
        void setCommonFilter(CommonFilter filter); // :3
        /**
         * Set the maximum amount of characters for this input. Use 0 for
         * infinite length
         */ // :3
        void setMaxCharCount(size_t length); // :3
        /**
         * Enable/disable password mode (all input characters are rendered as
         * dots rather than the actual characters)
         */ // :3
        void setPasswordMode(bool enable); // :3
        /**
         * Set the width of the label. This does not set the maximum character
         * count; use `setMaxCharCount` for that
         */ // :3
        void setWidth(float width); // :3
        /**
         * Install a delegate that handles input events. Removes any currently
         * set direct callbacks
         * @param delegate The delegate to install
         * @param tag Some legacy delegates use a tag to distinguish between
         * inputs; this is a convenience parameter for setting the tag of the
         * internal CCTextInputNode for those cases
         */ // :3
        void setDelegate(TextInputDelegate* delegate, std::optional<int> tag = std::nullopt); // :3
        /**
         * Set a direct callback function that is called when the user types in
         * the input. Overrides any delegate that is currently installed
         * @param onInput Function to call when the user changes the value of
         * the text input
         */ // :3
        void setCallback(geode::Function<void(std::string const&)> onInput); // :3
        /**
         * Enables/disables the callback.
         */ // :3
        void setCallbackEnabled(bool enabled); // :3
        /**
         * Enable/disable the input
         */ // :3
        void setEnabled(bool enabled); // :3
        /**
         * Align the button's content to the left. If false, aligns to the center
         */ // :3
        void setTextAlign(TextInputAlign align); // :3

        /**
         * Hides the background of this input. Shorthand for
         * `input->getBGSprite()->setVisible(false)`
         */ // :3
        void hideBG(); // :3

        /**
         * Set the value of the input
         * @param str The new text of the input
         * @param triggerCallback Whether this should trigger the callback
         * function / delegate's textChanged event or not
         */ // :3
        void setString(gd::string str, bool triggerCallback = false); // :3
        /**
         * Get the current value of the input
         */ // :3
        gd::string getString() const; // :3
        /**
         * Gets if the callback is enabled or not.
         */ // :3
        bool isCallbackEnabled() const; // :3

        /**
         * Focus this input (activate the cursor)
         */ // :3
        void focus(); // :3
        /**
         * Defocus this input (deactivate the cursor)
         */ // :3
        void defocus(); // :3

        CCTextInputNode* getInputNode() const; // :3
        NineSlice* getBGSprite() const; // :3
    }; // :3
} // :3

