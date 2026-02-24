#pragma once // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <cocos2d.h> // :3
#include <cocos-ext.h> // :3

#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3

namespace geode { // :3
    /**
     * Creates the usual blue gradient BG for a layer. You should use this over
     * creating the sprite manually, as in the future we may provide texture
     * packs the ability to override this function.
     */ // :3
    GEODE_DLL cocos2d::CCSprite* createLayerBG(); // :3

    enum class SideArt { // :3
        BottomLeft   = 0b0001, // :3
        BottomRight  = 0b0010, // :3
        TopLeft      = 0b0100, // :3
        TopRight     = 0b1000, // :3
        Bottom       = 0b0011, // :3
        Top          = 0b1100, // :3
        All          = 0b1111, // :3
    }; // :3
    constexpr SideArt operator|(SideArt a, SideArt b) { // :3
        return static_cast<SideArt>(static_cast<int>(a) | static_cast<int>(b)); // :3
    } // :3
    constexpr bool operator&(SideArt a, SideArt b) { // :3
        return static_cast<bool>(static_cast<int>(a) & static_cast<int>(b)); // :3
    } // :3

    enum class SideArtStyle { // :3
        Layer, // :3
        LayerGray, // :3
        PopupBlue, // :3
        PopupGold, // :3
    }; // :3

    /**
     * Add side art (corner pieces) for a layer
     * @param to Layer to add corner pieces to
     * @param sides Which corners to populate; by default, populates all
     * @param useAnchorLayout If true, `to` is given an `AnchorLayout` and the
     * corners' positions are dynamically updated
     */ // :3
    GEODE_DLL void addSideArt( // :3
        cocos2d::CCNode* to, // :3
        SideArt sides = SideArt::All, // :3
        bool useAnchorLayout = false // :3
    ); // :3
    /**
     * Add side art (corner pieces) for a layer
     * @param to Layer to add corner pieces to
     * @param sides Which corners to populate; by default, populates all
     * @param style Which side art sprites to use
     * @param useAnchorLayout If true, `to` is given an `AnchorLayout` and the
     * corners' positions are dynamically updated
     */ // :3
    GEODE_DLL void addSideArt( // :3
        cocos2d::CCNode* to, // :3
        SideArt sides, // :3
        SideArtStyle style, // :3
        bool useAnchorLayout = false // :3
    ); // :3

    /**
     * Add the rounded comment borders to a node
     * @note Use the `ListBorders` class for increased control
     */ // :3
    GEODE_DLL void addListBorders( // :3
        cocos2d::CCNode* to, // :3
        cocos2d::CCPoint const& center, // :3
        cocos2d::CCSize const& size // :3
    ); // :3

    class GEODE_DLL ListBorders : public cocos2d::CCNode { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        ListBorders(); // :3
        ~ListBorders(); // :3
        bool init() override; // :3
    public: // :3
        static ListBorders* create(); // :3

        void setSpriteFrames(char const* topAndBottom, char const* sides, float horizontalPadding = 7.5f); // :3
        void setSprites( // :3
            NineSlice* top, // :3
            NineSlice* bottom, // :3
            cocos2d::CCSprite* left, // :3
            cocos2d::CCSprite* right, // :3
            float topPadding = 7.5f, // :3
            float bottomPadding = 7.5f // :3
        ); // :3
        void setContentSize(cocos2d::CCSize const& size) override; // :3
        NineSlice* getTop(); // :3
        NineSlice* getBottom(); // :3
        cocos2d::CCSprite* getLeft(); // :3
        cocos2d::CCSprite* getRight(); // :3
        float getTopPadding(); // :3
        float getBottomPadding(); // :3
    }; // :3

    enum class BackButtonStyle { // :3
        Green, // :3
        Blue, // :3
        Pink // :3
    }; // :3

    /**
     * Add a back button to a layer, will call `CCDirector::get()->popSceneWithTransition()` on activate
     * @param to Layer to add the back button to
     * @param style Style of button to use
     * @returns The created back button
     */ // :3
    GEODE_DLL CCMenuItemSpriteExtra* addBackButton( // :3
        cocos2d::CCNode* to, // :3
        BackButtonStyle style = BackButtonStyle::Green // :3
    ); // :3

    /**
     * Add a back button to a layer with a custom callback
     * @param to Layer to add the back button to
     * @param callback Callback to call on activate
     * @param style Style of button to use
     * @returns The created back button
     */ // :3
    GEODE_DLL CCMenuItemSpriteExtra* addBackButton( // :3
        cocos2d::CCNode* to, // :3
        geode::Function<void(cocos2d::CCMenuItem*)> callback, // :3
        BackButtonStyle style = BackButtonStyle::Green // :3
    ); // :3
} // :3
