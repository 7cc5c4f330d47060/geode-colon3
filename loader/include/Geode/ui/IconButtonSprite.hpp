#pragma once // :3

#include <cocos2d.h> // :3
#include <Geode/ui/NineSlice.hpp> // :3

namespace geode { // :3
    class GEODE_DLL IconButtonSprite : public cocos2d::CCSprite, public cocos2d::CCLabelProtocol { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        bool init( // :3
            char const* bg, bool bgIsFrame, cocos2d::CCNode* icon, char const* text, // :3
            char const* font // :3
        ); // :3

        void updateLayout(); // :3

        IconButtonSprite(); // :3
        ~IconButtonSprite(); // :3
        IconButtonSprite(IconButtonSprite&&) = delete; // :3
        IconButtonSprite& operator=(IconButtonSprite&&) = delete; // :3

    public: // :3
        static IconButtonSprite* create( // :3
            char const* bg, cocos2d::CCNode* icon, char const* text, char const* font // :3
        ); // :3
        static IconButtonSprite* createWithSpriteFrameName( // :3
            char const* bg, cocos2d::CCNode* icon, char const* text, char const* font // :3
        ); // :3

        void setBG(char const* bg, bool isFrame); // :3

        void setIcon(cocos2d::CCNode* icon); // :3
        cocos2d::CCNode* getIcon() const; // :3

        void setString(char const* label) override; // :3
        char const* getString() override; // :3

        void setColor(cocos2d::ccColor3B const& color) override; // :3
        void setOpacity(GLubyte opacity) override; // :3
        NineSlice* getBg(); // :3
        cocos2d::CCLabelBMFont* getLabel(); // :3
        cocos2d::CCNode* getIcon(); // :3
    }; // :3
} // :3
