#pragma once // :3

#include <cocos2d.h> // :3

namespace geode { // :3
    class GEODE_DLL Border : public cocos2d::CCLayerColor { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    public: // :3
        struct Padding { // :3
            float top; // :3
            float right; // :3
            float bottom; // :3
            float left; // :3
        }; // :3

        static Border* create(cocos2d::CCNode* node, const cocos2d::ccColor4B& backgroundColor, const cocos2d::CCSize& size = { 0, 0 }, const cocos2d::CCPoint& padding = { 0, 0 }); // :3

        void setPadding(const cocos2d::CCPoint& padding); // :3
        void setPadding(float x, float y); // :3
        void setPadding(float padding); // :3
        Padding getPadding(); // :3
        void setPaddingX(float x); // :3
        float getPaddingX(); // :3
        void setPaddingY(float y); // :3
        float getPaddingY(); // :3
        void setPaddingTop(float top); // :3
        float getPaddingTop(); // :3
        void setPaddingRight(float right); // :3
        float getPaddingRight(); // :3
        void setPaddingBottom(float bottom); // :3
        float getPaddingBottom(); // :3
        void setPaddingLeft(float left); // :3
        float getPaddingLeft(); // :3
        void setBackgroundColor(const cocos2d::ccColor4B& color); // :3
        cocos2d::ccColor4B getBackgroundColor(); // :3
        void setNode(CCNode* node); // :3
        CCNode* getNode(); // :3
        void setSize(const cocos2d::CCSize& size); // :3
    protected: // :3
        Border(const cocos2d::CCPoint& padding = { 0, 0 }); // :3
        ~Border(); // :3
        bool init(const cocos2d::ccColor4B& backgroundColor, const cocos2d::CCSize& size); // :3
        bool init(CCNode* node, const cocos2d::ccColor4B& backgroundColor, const cocos2d::CCSize& size); // :3
    private: // :3
        void updatePadding(); // :3
    }; // :3
} // :3