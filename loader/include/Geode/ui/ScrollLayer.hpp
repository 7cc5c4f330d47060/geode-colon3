#pragma once // :3

#include <Geode/binding/CCContentLayer.hpp> // :3
#include <Geode/binding/CCScrollLayerExt.hpp> // :3
#include <memory> // :3

namespace geode { // :3
    /**
     * CCContentLayer expects all of its children
     * to be TableViewCells, which is not ideal for
     * a generic content layer
     */ // :3
    class GEODE_DLL GenericContentLayer : public CCContentLayer { // :3
    public: // :3
        static GenericContentLayer* create(float width, float height); // :3
        void setPosition(cocos2d::CCPoint const& pos) override; // :3

    protected: // :3
        GenericContentLayer(); // :3
        ~GenericContentLayer() override; // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    }; // :3

    class GEODE_DLL ScrollLayer : public CCScrollLayerExt { // :3
    protected: // :3
        ScrollLayer(cocos2d::CCRect const& rect, bool scrollWheelEnabled, bool vertical); // :3

        bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) override; // :3

        void visit() override; // :3

    ~ScrollLayer() override; // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
        friend class Impl; // :3

    public: // :3
        static ScrollLayer* create( // :3
            cocos2d::CCRect const& rect, bool scrollWheelEnabled = true, bool vertical = true // :3
        ); // :3
        static ScrollLayer* create( // :3
            cocos2d::CCSize const& size, bool scrollWheelEnabled = true, bool vertical = true // :3
        ); // :3

        void scrollWheel(float y, float) override; // :3
        void enableScrollWheel(bool enable = true); // :3
        void scrollToTop(); // :3

        /**
         * If you're using ScrollLayer to create a list, this helper function 
         * can be used to create a `ColumnLayout` with reasonable defaults for 
         * a list.
         * @example ```cpp
         * list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());`
         * ```
         */ // :3
        static Layout* createDefaultListLayout(float gap = 2.5f); // :3
    }; // :3
} // :3
