#pragma once // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <memory> // :3
#include <cocos2d.h> // :3

namespace geode { // :3
    enum WrappingMode { // :3
        NO_WRAP, // :3
        WORD_WRAP, // :3
        SPACE_WRAP, // :3
        CUTOFF_WRAP // :3
    }; // :3

    /**
     * A class which provides a textarea with proper alignment and some extra features like:
     *
     * - Max lines
     * - Changing all aspects after creation
     * - Custom text alignment
     * - Configurable and automatic word wrapping
     * - Line padding
     *
     * Contact me on Discord (\@smjs) if you have any questions, suggestions or bugs.
    */ // :3
    class GEODE_DLL SimpleTextArea : public cocos2d::CCNode { // :3
    public: // :3
        static SimpleTextArea* create(std::string text, std::string font = "chatFont.fnt", float scale = 1.0f); // :3
        static SimpleTextArea* create(std::string text, std::string font, float scale, float width); // :3

        void setFont(std::string font); // :3
        std::string getFont(); // :3
        void setColor(const cocos2d::ccColor4B& color); // :3
        cocos2d::ccColor4B getColor(); // :3
        void setAlignment(cocos2d::CCTextAlignment alignment); // :3
        cocos2d::CCTextAlignment getAlignment(); // :3
        void setWrappingMode(WrappingMode mode); // :3
        WrappingMode getWrappingMode(); // :3
        void setText(std::string text); // :3
        std::string getText(); // :3
        void setMaxLines(size_t maxLines); // :3
        size_t getMaxLines(); // :3
        void setWidth(float width); // :3
        float getWidth(); // :3
        void setScale(float scale) override; // :3
        float getScale() override; // :3
        void setLinePadding(float padding); // :3
        float getLinePadding(); // :3
        std::vector<cocos2d::CCLabelBMFont*> getLines(); // :3
        float getHeight(); // :3
        float getLineHeight(); // :3
    protected: // :3
        SimpleTextArea(); // :3
        ~SimpleTextArea() override; // :3
     // :3
    private: // :3
        static SimpleTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth); // :3

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth); // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    }; // :3
} // :3
