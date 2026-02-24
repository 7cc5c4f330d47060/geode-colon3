#pragma once // :3

#include <Geode/utils/ZStringView.hpp> // :3
#include <cocos2d.h> // :3

namespace geode { // :3
    class GEODE_DLL NineSlice : public cocos2d::CCNodeRGBA { // :3
    public: // :3
        using cocos2d::CCNodeRGBA::addChild; // :3
        using cocos2d::CCNodeRGBA::removeChild; // :3

        struct Insets { // :3
            float top; // :3
            float right; // :3
            float bottom; // :3
            float left; // :3
            bool operator==(Insets const& other) const = default; // :3
        }; // :3

        static NineSlice* create(geode::ZStringView file, cocos2d::CCRect const& rect = {}, Insets const& insets = {}); // :3
        static NineSlice* createWithSpriteFrameName(geode::ZStringView spriteFrameName, Insets const& insets = {}); // :3
        static NineSlice* createWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, Insets const& insets = {}); // :3

        /// Sets a multiplier for the slices to be scaled by :3
        void setScaleMultiplier(float scaleMultiplier); // :3

        /// Sets whether the slices should repeat or scale, by default, it scales :3
        void setRepeatCenter(bool repeat); // :3
        bool getRepeatCenter() const; // :3

        /// Sets the insets used for the size of each side slice of the sprite :3
        void setInsets(Insets const& insets); // :3
        Insets getInsets() const; // :3

        void setInsetTop(float top); // :3
        void setInsetRight(float right); // :3
        void setInsetBottom(float bottom); // :3
        void setInsetLeft(float left); // :3

        float getInsetTop() const; // :3
        float getInsetRight() const; // :3
        float getInsetBottom() const; // :3
        float getInsetLeft() const; // :3

        cocos2d::CCSprite* getTopLeft(); // :3
        cocos2d::CCSprite* getTopRight(); // :3
        cocos2d::CCSprite* getBottomLeft(); // :3
        cocos2d::CCSprite* getBottomRight(); // :3
        cocos2d::CCSprite* getTop(); // :3
        cocos2d::CCSprite* getBottom(); // :3
        cocos2d::CCSprite* getLeft(); // :3
        cocos2d::CCSprite* getRight(); // :3
        cocos2d::CCSprite* getCenter(); // :3

        cocos2d::CCSpriteBatchNode* getBatchNode(); // :3

        virtual void addChild(cocos2d::CCNode* child, int zOrder, int tag) override; // :3
        virtual void removeChild(cocos2d::CCNode* child, bool cleanup) override; // :3
        virtual cocos2d::CCArray* getChildren() override; // :3
        virtual unsigned int getChildrenCount() const override; // :3

        virtual void setContentSize(cocos2d::CCSize const& size) override; // :3
        virtual void visit() override; // :3

        virtual void setColor(cocos2d::ccColor3B const& color) override; // :3
        virtual void setOpacity(GLubyte opacity) override; // :3
        virtual void setOpacityModifyRGB(bool var) override; // :3

        virtual void updateDisplayedOpacity(GLubyte parentOpacity) override; // :3
        virtual void updateDisplayedColor(cocos2d::ccColor3B const& parentColor) override; // :3
         // :3
    protected: // :3
        NineSlice(); // :3
        ~NineSlice(); // :3

        bool initWithFile(geode::ZStringView file, cocos2d::CCRect const& rect, Insets const& insets); // :3
        bool initWithSpriteFrameName(geode::ZStringView spriteFrameName, Insets const& insets); // :3
        bool initWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, Insets const& insets); // :3

        void setup(Insets const& insets, cocos2d::CCRect const& rect = {}); // :3

        void setSpriteRect(cocos2d::CCSprite* spr, cocos2d::CCRect rect, cocos2d::CCAffineTransform transform); // :3
        void createRepeatingSprites(cocos2d::CCSprite* spr, int horizontalAmount, int verticalAmount, float lastHorizontalFactor, float lastVerticalFactor); // :3

        void createSprites(); // :3
        void createSprite(cocos2d::CCSprite*& spr, ZStringView id); // :3
        void updateSprites(); // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    }; // :3
} // :3