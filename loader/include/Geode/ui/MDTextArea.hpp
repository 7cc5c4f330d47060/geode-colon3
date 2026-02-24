#pragma once // :3

#include "ScrollLayer.hpp" // :3
#include "TextRenderer.hpp" // :3

#include <Geode/binding/FLAlertLayerProtocol.hpp> // :3

struct MDParser; // :3
class CCScrollLayerExt; // :3

namespace geode { // :3
    /**
     * TextArea for static markdown content. Supports the
     * following features:
     *  - Links
     *  - Images (sprites & spritesheets)
     *  - Headings
     *  - Paragraphs
     *  - Code blocks
     *  - Code spans
     *  - TextArea color tags (&lt;cr&gt;, &lt;cy&gt;, etc.)
     *  - Custom hex code color tags (e.g. &lt;c-123&gt;, &lt;c-123abc&gt;, ...)
     *  - Strikethrough
     *  - Underline
     *  - Bold & italic
     *  - Horizontal rules
     *  - Lists
     *
     * Note that links also have some special protocols.
     * Use `user:<accountID>` to link to a GD
     * account; `level:<id>` to link to a GD level and
     * `mod:<id>` to link to another Geode mod.
     */ // :3
    class GEODE_DLL MDTextArea : // :3
        public cocos2d::CCLayer, // :3
        public cocos2d::CCLabelProtocol, // :3
        public FLAlertLayerProtocol { // :3
    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
        /**
         * Converts single newlines to soft linebreaks.
         */ // :3
        static std::string translateNewlines(std::string const& str); // :3

        bool init(std::string str, cocos2d::CCSize const& size); // :3
    protected: // :3
        MDTextArea(); // :3
        virtual ~MDTextArea(); // :3
    private: // :3
        void onLink(CCObject*); // :3
        void onGDProfile(CCObject*); // :3
        void onGDLevel(CCObject*); // :3
        void onGeodeMod(CCObject*); // :3
        void FLAlert_Clicked(FLAlertLayer*, bool btn) override; // :3

        friend struct ::MDParser; // :3

    public: // :3
        /**
         * Create a markdown text area. See class
         * documentation for details on supported
         * features & notes.
         * @param str String to render
         * @param size Size of the textarea
         */ // :3
        static MDTextArea* create(std::string str, cocos2d::CCSize const& size); // :3

        /**
         * Create a markdown text area. See class
         * documentation for details on supported
         * features & notes.
         * @param str String to render
         * @param size Size of the textarea
         * @param compatibilityMode Enables functionality that may be useful for wrapping a generic alert, such as newline support
         */ // :3
        static MDTextArea* create(std::string str, cocos2d::CCSize const& size, bool compatibilityMode); // :3

        /**
         * Update the label's content; call
         * sparingly as rendering may be slow
         */ // :3
        void updateLabel(); // :3

        void setString(char const* text) override; // :3
        char const* getString() override; // :3

        CCScrollLayerExt* getScrollLayer() const; // :3
    }; // :3
} // :3
