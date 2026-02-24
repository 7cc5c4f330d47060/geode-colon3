#pragma once // :3

#include <cocos2d.h> // :3
#include <Geode/utils/function.hpp> // :3

namespace geode { // :3
    enum class TextAlignment { // :3
        Begin, // :3
        Center, // :3
        End, // :3
    }; // :3

    enum class TextCapitalization { // :3
        Normal, // :3
        AllUpper, // :3
        AllLower, // :3
    }; // :3

    // enum only as these are flags :3
    enum TextStyle { // :3
        TextStyleRegular = 0b0, // :3
        TextStyleBold = 0b1, // :3
        TextStyleItalic = 0b10, // :3
    }; // :3

    // enum only as these are flags :3
    enum TextDecoration { // :3
        TextDecorationNone = 0b0, // :3
        TextDecorationUnderline = 0b1, // :3
        TextDecorationStrikethrough = 0b10, // :3
    }; // :3

    class TextDecorationWrapper; // :3
    class TextLinkedButtonWrapper; // :3

    /**
     * Utility class for creating rich text content.
     * Use to incrementally render strings, and push
     * variables to modify the renderer's state. Use
     * `begin` to start rendering to a target and
     * `end` to finish rendering.
     *
     * Works for any type of label, although relies
     * heavily on content sizes for labels and nodes.
     *
     * Not too well-performant and the rendering is
     * done linearly without so this is not suitable
     * for dynamic content. For something like a
     * static rich text -area though this can prove
     * useful. Used in MDTextArea.
     */ // :3
    class GEODE_DLL TextRenderer final : public cocos2d::CCObject { // :3
    public: // :3
        TextRenderer() = default; // :3
        TextRenderer(const TextRenderer&) = delete; // :3
        TextRenderer(TextRenderer&&) = delete; // :3
        /**
         * Represents a label. As CCLabelBMFont and
         * CCLabelTTF have different inheritance
         * structures, this class can handle either
         * one universally. All relevant vtables are
         * stored in-class to avoid needing to
         * `dynamic_cast` everything. This way of
         * storing vtables also means that anything
         * which satisfies these 3 vtables can be used,
         * even if its true UX representation is
         * actually not a label.
         */ // :3
        struct Label { // :3
            /**
             * Label's CCNode vtable
             */ // :3
            cocos2d::CCNode* m_node; // :3
            /**
             * Label's CCLabelProtocol vtable
             */ // :3
            cocos2d::CCLabelProtocol* m_labelProtocol; // :3
            /**
             * Label's CCRGBAProtocol vtable
             */ // :3
            cocos2d::CCRGBAProtocol* m_rgbaProtocol; // :3
            /**
             * Line height. If 0, the renderer will dynamically
             * calculate line height based on content size.
             */ // :3
            float m_lineHeight; // :3

            explicit inline Label() { // :3
                m_node = nullptr; // :3
                m_labelProtocol = nullptr; // :3
                m_rgbaProtocol = nullptr; // :3
                m_lineHeight = .0f; // :3
            } // :3

            template <class T> // :3
            Label(T* label, float lineHeight = .0f) { // :3
                static_assert( // :3
                    std::is_base_of_v<cocos2d::CCNode, T>, "Label must inherit from CCNode!" // :3
                ); // :3
                static_assert( // :3
                    std::is_base_of_v<cocos2d::CCLabelProtocol, T>, // :3
                    "Label must inherit from CCLabelProtocol!" // :3
                ); // :3
                static_assert( // :3
                    std::is_base_of_v<cocos2d::CCRGBAProtocol, T>, // :3
                    "Label must inherit from CCRGBAProtocol!" // :3
                ); // :3
                m_node = label; // :3
                m_labelProtocol = label; // :3
                m_rgbaProtocol = label; // :3
                if (lineHeight) { // :3
                    m_lineHeight = lineHeight; // :3
                } // :3
                else { // :3
                    if constexpr (std::is_same_v<cocos2d::CCLabelBMFont, T>) { // :3
                        m_lineHeight = label->getConfiguration()->m_nCommonHeight / // :3
                            cocos2d::CC_CONTENT_SCALE_FACTOR(); // :3
                    } // :3
                } // :3
            } // :3
        }; // :3

        /**
         * Label generator function. The `int` parameter
         * represents the current text style flags. Use
         * to distinguish between bold, italic and
         * regular text.
         */ // :3
        using Font = geode::Function<Label(int) const>; // :3
        using FontRef = geode::FunctionRef<Label(int) const>; // :3

    protected: // :3
        cocos2d::CCPoint m_origin = cocos2d::CCPointZero; // :3
        cocos2d::CCSize m_size = cocos2d::CCSizeZero; // :3
        cocos2d::CCPoint m_cursor = cocos2d::CCPointZero; // :3
        cocos2d::CCNode* m_target = nullptr; // :3
        std::vector<Font> m_fontStack; // :3
        std::vector<float> m_scaleStack; // :3
        std::vector<int> m_styleStack; // :3
        std::vector<cocos2d::ccColor3B> m_colorStack; // :3
        std::vector<GLubyte> m_opacityStack; // :3
        std::vector<int> m_decorationStack; // :3
        std::vector<TextCapitalization> m_capsStack; // :3
        std::vector<Label> m_lastRendered; // :3
        std::vector<float> m_indentationStack; // :3
        std::vector<float> m_wrapOffsetStack; // :3
        std::vector<TextAlignment> m_hAlignmentStack; // :3
        std::vector<TextAlignment> m_vAlignmentStack; // :3
        std::vector<cocos2d::CCNode*> m_renderedLine; // :3
        cocos2d::CCNode* m_lastRenderedNode = nullptr; // :3

        bool init(); // :3

        Label addWrappers( // :3
            Label const& label, bool isButton, cocos2d::CCObject* target, // :3
            cocos2d::SEL_MenuHandler callback // :3
        ); // :3
        bool render(std::string word, cocos2d::CCNode* to, cocos2d::CCLabelProtocol* label); // :3
        float adjustLineAlignment(); // :3

    public: // :3
        /**
         * Create a TextRenderer
         * @returns Created TextRenderer
         */ // :3
        static TextRenderer* create(); // :3
        virtual ~TextRenderer(); // :3

        /**
         * Initialize renderer
         * @param target Target node to render to. If nullptr,
         * a new CCNode will be created.
         * @param pos Position to render to
         * @param size Size of the render area. Needed for
         * text wrapping & alignment
         */ // :3
        void begin( // :3
            cocos2d::CCNode* target, cocos2d::CCPoint const& pos = cocos2d::CCPointZero, // :3
            cocos2d::CCSize const& size = cocos2d::CCSizeZero // :3
        ); // :3
        /**
         * Finish rendering and clean up renderer
         * @param fitToContent Resize the target's content
         * size to match the rendered content
         * @param horizontalAlign Horizontal alignment of
         * the rendered text
         * @param verticalAlign Vertical alignment of
         * the rendered text
         * @returns Target that was rendered onto
         */ // :3
        cocos2d::CCNode* end( // :3
            bool fitToContent = true, TextAlignment horizontalAlign = TextAlignment::Begin, // :3
            TextAlignment verticalAlign = TextAlignment::Begin // :3
        ); // :3

        /**
         * Render a string with specific settings, bypassing
         * current variable stacks.
         * @param str String to render
         * @param font Font function to use
         * @param scale Scale of label
         * @param color Label color
         * @param opacity Label opacity
         * @param style Label style (TextStyle enum)
         * @param deco Label decorations (TextDecoration enum)
         * @param caps String capitalization
         * @param addToTarget Whether to add the created label(s)
         * onto the target
         * @param isButton If the label should be created as an
         * interactive linked button
         * @param buttonTarget Target for the label if isButton is
         * true, defaults to current renderer target
         * @param callback Callback for the label if isButton is
         * true
         * @returns Vector of rendered labels. The label may be
         * split on multiple lines if it exceeds bounds
         */ // :3
        std::vector<Label> renderStringEx( // :3
            std::string str, Font font, float scale, // :3
            cocos2d::ccColor3B color = { 255, 255, 255 }, GLubyte opacity = 255, // :3
            int style = TextStyleRegular, int deco = TextDecorationNone, // :3
            TextCapitalization caps = TextCapitalization::Normal, bool addToTarget = true, // :3
            bool isButton = false, cocos2d::CCObject* buttonTarget = nullptr, // :3
            cocos2d::SEL_MenuHandler callback = nullptr // :3
        ); // :3
        /**
         * Render a string to target. Uses current variable stacks
         * for styling and parameters
         * @param str String to render
         * @returns Vector of rendered labels. The label may be
         * split on multiple lines if it exceeds bounds
         */ // :3
        std::vector<Label> renderString(std::string str); // :3
        /**
         * Render a string to target as a button. Note that the
         * target should be a CCMenu for the button to do
         * anything. Uses current variable stacks  for styling
         * and parameters
         * @param str String to render
         * @param buttonTarget Target for the label if isButton is
         * true, defaults to current renderer target
         * @param callback Callback for the label if isButton is
         * true
         * @returns Vector of rendered labels. The label may be
         * split on multiple lines if it exceeds bounds
         */ // :3
        std::vector<Label> renderStringInteractive( // :3
            std::string str, cocos2d::CCObject* buttonTarget, // :3
            cocos2d::SEL_MenuHandler callback // :3
        ); // :3
        /**
         * Render a node to the current target, use for adding
         * images & other content in the middle of text
         * @param node Node to render
         * @returns Rendered node
         */ // :3
        cocos2d::CCNode* renderNode(cocos2d::CCNode* node); // :3
        /**
         * Start next line
         * @param y Y offset amount from previous line. If 0,
         * will dynamically figure out based on content size
         */ // :3
        void breakLine(float y = .0f); // :3

        /**
         * Helper for pushing a CCLabelBMFont. Make
         * sure the const char* outlives the renderer.
         */ // :3
        void pushBMFont(char const* bmFont); // :3
        void pushFont(Font font); // :3
        void popFont(); // :3
        FontRef getCurrentFont() const; // :3

        void pushScale(float scale); // :3
        void popScale(); // :3
        float getCurrentScale() const; // :3

        void pushStyleFlags(int style); // :3
        void popStyleFlags(); // :3
        int getCurrentStyle() const; // :3

        void pushColor(cocos2d::ccColor3B const& color); // :3
        void popColor(); // :3
        cocos2d::ccColor3B getCurrentColor() const; // :3

        void pushOpacity(GLubyte opacity); // :3
        void popOpacity(); // :3
        GLubyte getCurrentOpacity() const; // :3

        void pushDecoFlags(int deco); // :3
        void popDecoFlags(); // :3
        int getCurrentDeco() const; // :3

        void pushCaps(TextCapitalization caps); // :3
        void popCaps(); // :3
        TextCapitalization getCurrentCaps() const; // :3

        void pushIndent(float indent); // :3
        void popIndent(); // :3
        float getCurrentIndent() const; // :3

        void pushWrapOffset(float wrapOffset); // :3
        void popWrapOffset(); // :3
        float getCurrentWrapOffset() const; // :3

        void pushVerticalAlign(TextAlignment align); // :3
        void popVerticalAlign(); // :3
        TextAlignment getCurrentVerticalAlign() const; // :3

        void pushHorizontalAlign(TextAlignment align); // :3
        void popHorizontalAlign(); // :3
        TextAlignment getCurrentHorizontalAlign() const; // :3

        void moveCursor(cocos2d::CCPoint const& pos); // :3
        cocos2d::CCPoint getCursorPos(); // :3
    }; // :3

    /**
     * Wrapper node for adding decorations (strikethrough,
     * underline) to an arbitrary label. Is not agnostic of
     * font and as such will always render simple lines
     */ // :3
    class TextDecorationWrapper : public cocos2d::CCNodeRGBA, public cocos2d::CCLabelProtocol { // :3
    protected: // :3
        int m_deco; // :3
        TextRenderer::Label m_label; // :3

        bool init( // :3
            TextRenderer::Label const& label, int decoration, cocos2d::ccColor3B const& color, // :3
            GLubyte opacity // :3
        ); // :3

        void draw() override; // :3

    public: // :3
        static TextDecorationWrapper* create( // :3
            TextRenderer::Label const& label, int decoration, cocos2d::ccColor3B const& color, // :3
            GLubyte opacity // :3
        ); // :3
        static TextDecorationWrapper* wrap( // :3
            TextRenderer::Label const& label, int decoration, cocos2d::ccColor3B const& color, // :3
            GLubyte opacity // :3
        ); // :3

        void setColor(cocos2d::ccColor3B const& color) override; // :3
        void setOpacity(GLubyte opacity) override; // :3
        void updateDisplayedColor(cocos2d::ccColor3B const& color) override; // :3
        void updateDisplayedOpacity(GLubyte opacity) override; // :3

        void setString(char const* text) override; // :3
        char const* getString() override; // :3
    }; // :3

    /**
     * Wrapper node for making a label clickable.
     * Note that this should always be the top
     * wrapper above all other wrappers
     */ // :3
    class TextLinkedButtonWrapper : // :3
        public cocos2d::CCMenuItemSprite, // :3
        public cocos2d::CCLabelProtocol { // :3
    protected: // :3
        TextRenderer::Label m_label; // :3
        GLubyte m_opacity; // :3
        cocos2d::ccColor3B m_color; // :3
        std::vector<TextLinkedButtonWrapper*> m_linked; // :3

        bool init( // :3
            TextRenderer::Label const& label, cocos2d::CCObject* target, // :3
            cocos2d::SEL_MenuHandler handler // :3
        ); // :3

    public: // :3
        static TextLinkedButtonWrapper* create( // :3
            TextRenderer::Label const& label, cocos2d::CCObject* target, // :3
            cocos2d::SEL_MenuHandler handler // :3
        ); // :3
        static TextLinkedButtonWrapper* wrap( // :3
            TextRenderer::Label const& label, cocos2d::CCObject* target, // :3
            cocos2d::SEL_MenuHandler handler // :3
        ); // :3

        void link(TextLinkedButtonWrapper* other); // :3

        void selectedWithoutPropagation(bool selected); // :3
        void selected() override; // :3
        void unselected() override; // :3

        void setColor(cocos2d::ccColor3B const& color) override; // :3
        void setOpacity(GLubyte opacity) override; // :3
        void updateDisplayedColor(cocos2d::ccColor3B const& color) override; // :3
        void updateDisplayedOpacity(GLubyte opacity) override; // :3

        void setString(char const* text) override; // :3
        char const* getString() override; // :3
    }; // :3
} // :3
