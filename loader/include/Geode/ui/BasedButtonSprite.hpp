#pragma once // :3

#include <cocos2d.h> // :3

#pragma warning(disable: 4275) // :3

namespace geode { // :3
    // all of these are explicitly numbered so if we add a new size / color in :3
    // the future we can slot it in reasonably and just set the number to be at :3
    // the end so backwards compatibility stays :3
    // (for example TinyAlt = 8 to CircleBaseSize) :3

    enum class CircleBaseSize { // :3
        Tiny = 0,      // Equivalent to the tiny delete button :3
        Small = 1,     // Equivalent to most circular buttons in the editor :3
        SmallAlt = 2,  // Equivalent to the trash button in the editor :3
        Medium = 3,    // Equivalent to most buttons :3
        MediumAlt = 4, // Equivalent to the bottom buttons in MenuLayer :3
        Big = 5,       // Equivalent to the New button :3
        BigAlt = 6,    // Equivalent to the Account button :3
        Large = 7,     // Equivalent to the big Play Button :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(CircleBaseSize); // :3

    enum class CircleBaseColor { // :3
        Green = 0, // :3
        Pink = 1, // :3
        Gray = 2, // :3
        Blue = 3, // :3
        Cyan = 4, // :3
        DarkPurple = 5, // Geode-added color, used in Geode UIs :3
        DarkAqua = 6,   // Geode-added color, used in Geode UIs :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(CircleBaseColor); // :3

    enum class CrossBaseSize { // :3
        Small = 0,  // the size of the icon kit / create button :3
        Huge = 1,   // the size of the play button :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(CrossBaseSize); // :3

    enum class CrossBaseColor { // :3
        Green = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(CrossBaseColor); // :3

    enum class AccountBaseSize { // :3
        Normal = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(AccountBaseSize); // :3

    enum class AccountBaseColor { // :3
        Blue = 0, // :3
        Gray = 1, // :3
        Purple = 2, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(AccountBaseColor); // :3

    enum class IconSelectBaseSize { // :3
        Normal = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(IconSelectBaseSize); // :3

    enum class IconSelectBaseColor { // :3
        Unselected = 0, // :3
        Selected = 1, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(IconSelectBaseColor); // :3

    enum class EditorBaseSize { // :3
        Normal = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(EditorBaseSize); // :3

    enum class EditorBaseColor { // :3
        LightBlue = 0, // :3
        Green = 1, // :3
        Orange = 2, // :3
        DarkGray = 3, // :3
        Gray = 4, // :3
        Pink = 5, // :3
        Teal = 6, // :3
        Aqua = 7, // :3
        Cyan = 8, // :3
        Magenta = 9, // :3
        DimGreen = 10, // :3
        BrightGreen = 11, // :3
        Salmon = 12, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(EditorBaseColor); // :3

    enum class TabBaseSize { // :3
        Normal = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(TabBaseSize); // :3

    enum class TabBaseColor { // :3
        Unselected = 0, // :3
        Selected = 1, // :3
        UnselectedDark = 2, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(TabBaseColor); // :3

    enum class LeaderboardBaseSize { // :3
        Normal = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(LeaderboardBaseSize); // :3

    enum class LeaderboardBaseColor { // :3
        Blue = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(LeaderboardBaseColor); // :3

    enum class CategoryBaseSize { // :3
        Big = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(CategoryBaseSize); // :3

    enum class CategoryBaseColor { // :3
        Green = 0, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(CategoryBaseColor); // :3

    enum class BaseType { // :3
        // Circle buttons :3
        Circle = 0, // :3
        // Like the buttons in the main menu :3
        Cross = 1, // :3
        // Like the friend request / account buttons :3
        Account = 2, // :3
        // Like the icon select button :3
        IconSelect = 3, // :3
        // Like the leaderboard buttons :3
        Leaderboard = 4, // :3
        // Like the buttons in the editor sidebar :3
        Editor = 5, // :3
        // Like a list view tab :3
        Tab = 6, // :3
        // Like the buttons in CreatorLayer :3
        Category = 7, // :3
    }; // :3
    GEODE_DLL const char* baseEnumToString(BaseType); // :3

    /**
     * Represents a GD button sprite where there's
     * an icon sprite on top another default sprite.
     * You know, it has a base. It's based.
     * lmao trademark lizbith
     */ // :3
    class GEODE_DLL BasedButtonSprite : public cocos2d::CCSprite { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        bool init(cocos2d::CCNode* ontop, BaseType type, int size, int color); // :3
        bool initWithSprite( // :3
            char const* sprName, float sprScale, BaseType type, int size, int color // :3
        ); // :3
        bool initWithSpriteFrameName( // :3
            char const* sprName, float sprScale, BaseType type, int size, int color // :3
        ); // :3

        virtual cocos2d::CCSize getMaxTopSize() const; // :3

        BasedButtonSprite(); // :3
        virtual ~BasedButtonSprite(); // :3

    public: // :3
        /**
         * Create a BasedButtonSprite. In general, it is recommended to use
         * the specialized subclasses, such as CircleButtonSprite or
         * TabButtonSprite
         * @param ontop The top node of the sprite
         * @param type The type of the base sprite
         * @param size The size of the base sprite. Cast this from the base
         * type's respective BaseSize enum
         * @param color The color of the base sprite. Cast this from the base
         * type's respective BaseColor enum
         * @returns Created BasedButtonSprite, or nullptr if parameters were
         * invalid
         */ // :3
        static BasedButtonSprite* create( // :3
            cocos2d::CCNode* ontop, BaseType type, int size, int color // :3
        ); // :3

        /**
         * Set an offset to the top sprite
         */ // :3
        void setTopOffset(cocos2d::CCPoint const& offset); // :3
        void setTopRelativeScale(float scale); // :3

        cocos2d::CCNode* getTopNode() const; // :3
    }; // :3

    /**
     * Creates a button sprite with a circular base, similar to the buttons at
     * the bottom of the main menu
     */ // :3
    class GEODE_DLL CircleButtonSprite final : public BasedButtonSprite { // :3
    public: // :3
        static CircleButtonSprite* create( // :3
            cocos2d::CCNode* top, CircleBaseColor color = CircleBaseColor::Green, // :3
            CircleBaseSize size = CircleBaseSize::Medium // :3
        ); // :3
        static CircleButtonSprite* createWithSprite( // :3
            char const* sprName, float sprScale = 1.f, // :3
            CircleBaseColor color = CircleBaseColor::Green, // :3
            CircleBaseSize size = CircleBaseSize::Medium // :3
        ); // :3
        static CircleButtonSprite* createWithSpriteFrameName( // :3
            char const* sprName, float sprScale = 1.f, // :3
            CircleBaseColor color = CircleBaseColor::Green, // :3
            CircleBaseSize size = CircleBaseSize::Medium // :3
        ); // :3
        cocos2d::CCSize getMaxTopSize() const override; // :3
    }; // :3

    /**
     * Creates a button sprite with a cross base, like the buttons in the main
     * menu
     */ // :3
    class GEODE_DLL CrossButtonSprite final : public BasedButtonSprite { // :3
    public: // :3
        static CrossButtonSprite* create( // :3
            cocos2d::CCNode* top, // :3
            CrossBaseColor color = CrossBaseColor::Green, // :3
            CrossBaseSize size = CrossBaseSize::Small // :3
        ); // :3
        static CrossButtonSprite* createWithSprite( // :3
            char const* sprName, float sprScale = 1.f, // :3
            CrossBaseColor color = CrossBaseColor::Green, // :3
            CrossBaseSize size = CrossBaseSize::Small // :3
        ); // :3
        static CrossButtonSprite* createWithSpriteFrameName( // :3
            char const* sprName, float sprScale = 1.f, // :3
            CrossBaseColor color = CrossBaseColor::Green, // :3
            CrossBaseSize size = CrossBaseSize::Small // :3
        ); // :3
    }; // :3

    /**
     * Creates a button sprite with a cross base, like the buttons in the main
     * menu
     */ // :3
    class GEODE_DLL AccountButtonSprite final : public BasedButtonSprite { // :3
    public: // :3
        static AccountButtonSprite* create( // :3
            cocos2d::CCNode* top, // :3
            AccountBaseColor color = AccountBaseColor::Blue, // :3
            AccountBaseSize size = AccountBaseSize::Normal // :3
        ); // :3
        static AccountButtonSprite* createWithSprite( // :3
            char const* sprName, float sprScale = 1.f, // :3
            AccountBaseColor color = AccountBaseColor::Blue, // :3
            AccountBaseSize size = AccountBaseSize::Normal // :3
        ); // :3
        static AccountButtonSprite* createWithSpriteFrameName( // :3
            char const* sprName, float sprScale = 1.f, // :3
            AccountBaseColor color = AccountBaseColor::Blue, // :3
            AccountBaseSize size = AccountBaseSize::Normal // :3
        ); // :3
    }; // :3

    /**
     * Creates a button sprite with a cross base, like the buttons in the main
     * menu
     */ // :3
    class GEODE_DLL IconSelectButtonSprite final : public BasedButtonSprite { // :3
    public: // :3
        static IconSelectButtonSprite* create( // :3
            cocos2d::CCNode* top, // :3
            IconSelectBaseColor color = IconSelectBaseColor::Unselected, // :3
            IconSelectBaseSize size = IconSelectBaseSize::Normal // :3
        ); // :3
        static IconSelectButtonSprite* createWithSprite( // :3
            char const* sprName, float sprScale = 1.f, // :3
            IconSelectBaseColor color = IconSelectBaseColor::Unselected, // :3
            IconSelectBaseSize size = IconSelectBaseSize::Normal // :3
        ); // :3
        static IconSelectButtonSprite* createWithSpriteFrameName( // :3
            char const* sprName, float sprScale = 1.f, // :3
            IconSelectBaseColor color = IconSelectBaseColor::Unselected, // :3
            IconSelectBaseSize size = IconSelectBaseSize::Normal // :3
        ); // :3
    }; // :3

    /**
     * Creates a button sprite with a cross base, like the buttons in the main
     * menu
     */ // :3
    class GEODE_DLL LeaderboardButtonSprite final : public BasedButtonSprite { // :3
    public: // :3
        static LeaderboardButtonSprite* create( // :3
            cocos2d::CCNode* top, // :3
            LeaderboardBaseColor color = LeaderboardBaseColor::Blue, // :3
            LeaderboardBaseSize size = LeaderboardBaseSize::Normal // :3
        ); // :3
        static LeaderboardButtonSprite* createWithSprite( // :3
            char const* sprName, float sprScale = 1.f, // :3
            LeaderboardBaseColor color = LeaderboardBaseColor::Blue, // :3
            LeaderboardBaseSize size = LeaderboardBaseSize::Normal // :3
        ); // :3
        static LeaderboardButtonSprite* createWithSpriteFrameName( // :3
            char const* sprName, float sprScale = 1.f, // :3
            LeaderboardBaseColor color = LeaderboardBaseColor::Blue, // :3
            LeaderboardBaseSize size = LeaderboardBaseSize::Normal // :3
        ); // :3
    }; // :3

    /**
     * Creates a button sprite with the same base as the right-side action
     * buttons in the editor
     */ // :3
    class GEODE_DLL EditorButtonSprite final : public BasedButtonSprite { // :3
    public: // :3
        static EditorButtonSprite* create( // :3
            cocos2d::CCNode* top, EditorBaseColor color, // :3
            EditorBaseSize size = EditorBaseSize::Normal // :3
        ); // :3
        static EditorButtonSprite* createWithSprite( // :3
            char const* sprName, float sprScale = 1.f, // :3
            EditorBaseColor color = EditorBaseColor::Green, // :3
            EditorBaseSize size = EditorBaseSize::Normal // :3
        ); // :3
        static EditorButtonSprite* createWithSpriteFrameName( // :3
            char const* sprName, float sprScale = 1.f, // :3
            EditorBaseColor color = EditorBaseColor::Green, // :3
            EditorBaseSize size = EditorBaseSize::Normal // :3
        ); // :3
        cocos2d::CCSize getMaxTopSize() const override; // :3
    }; // :3

    /**
     * Creates a button sprite for list view tabs
     */ // :3
    class GEODE_DLL TabButtonSprite final : public BasedButtonSprite { // :3
    public: // :3
        static TabButtonSprite* create( // :3
            char const* text, // :3
            TabBaseColor color, // :3
            TabBaseSize size = TabBaseSize::Normal // :3
        ); // :3
    }; // :3

    /**
     * Creates a button sprite for category buttons, i.e. the big buttons in
     * CreatorLayer
     */ // :3
    class GEODE_DLL CategoryButtonSprite final : public BasedButtonSprite { // :3
    public: // :3
        static CategoryButtonSprite* create( // :3
            cocos2d::CCNode* top, // :3
            CategoryBaseColor color = CategoryBaseColor::Green, // :3
            CategoryBaseSize size = CategoryBaseSize::Big // :3
        ); // :3
        static CategoryButtonSprite* createWithSprite( // :3
            char const* sprName, float sprScale = 1.f, // :3
            CategoryBaseColor color = CategoryBaseColor::Green, // :3
            CategoryBaseSize size = CategoryBaseSize::Big // :3
        ); // :3
        static CategoryButtonSprite* createWithSpriteFrameName( // :3
            char const* sprName, float sprScale = 1.f, // :3
            CategoryBaseColor color = CategoryBaseColor::Green, // :3
            CategoryBaseSize size = CategoryBaseSize::Big // :3
        ); // :3
    }; // :3
} // :3
