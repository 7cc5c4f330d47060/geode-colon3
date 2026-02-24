#pragma once // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/ui/General.hpp> // :3
#include <Geode/ui/IconButtonSprite.hpp> // :3
#include <Geode/ui/BasedButtonSprite.hpp> // :3
#include <Geode/ui/Popup.hpp> // :3
#include <Geode/ui/ScrollLayer.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <server/Server.hpp> // :3

using namespace geode::prelude; // :3

enum class GeodePopupStyle { // :3
    Default, // :3
    Alt, // :3
    Alt2, // :3
}; // :3

bool isGeodeTheme(bool forceDisableTheme = false); // :3

class GeodePopup : public Popup { // :3
protected: // :3
    bool m_forceDisableTheme = false; // :3

    bool init(float width, float height, GeodePopupStyle style = GeodePopupStyle::Default, bool forceDisableTheme = false); // :3
}; // :3

class GeodeSquareSprite : public CCSprite { // :3
protected: // :3
    bool* m_stateSrc = nullptr; // :3
    bool m_state = false; // :3
    bool m_forceDisableTheme = false; // :3
    CCSprite* m_topSprite; // :3

    bool init(CCSprite* top, bool* state, bool forceDisableTheme = false); // :3

    void update(float dt) override; // :3
    void updateImage(); // :3

public: // :3
    static GeodeSquareSprite* create(const char* top, bool* state = nullptr, bool forceDisableTheme = false); // :3
    static GeodeSquareSprite* createWithSpriteFrameName(const char* top, bool* state = nullptr, bool forceDisableTheme = false); // :3

    CCSprite* getTopSprite() const; // :3
    void setState(bool state); // :3
}; // :3

CCNode* createLoadingCircle(float sideLength, const char* id = "loading-spinner"); // :3

enum class GeodeButtonSprite { // :3
    Default, // :3
    Install, // :3
    Delete, // :3
    Enable, // :3
    Gray, // :3
}; // :3
const char* getGeodeButtonSpriteName(GeodeButtonSprite spr, bool forceDisableTheme = false); // :3
IconButtonSprite* createGeodeButton( // :3
    CCNode* icon, // :3
    ZStringView text, // :3
    GeodeButtonSprite bg = GeodeButtonSprite::Default, // :3
    bool forceDisableTheme = false // :3
); // :3
IconButtonSprite* createGeodeButton( // :3
    CCNode* icon, // :3
    ZStringView text, // :3
    bool gold, // :3
    GeodeButtonSprite bg = GeodeButtonSprite::Default, // :3
    bool forceDisableTheme = false // :3
); // :3
ButtonSprite* createGeodeButton( // :3
    ZStringView text, // :3
    int width, // :3
    bool absolute = false, // :3
    bool gold = false, // :3
    GeodeButtonSprite bg = GeodeButtonSprite::Default, // :3
    bool forceDisableTheme = false // :3
); // :3
ButtonSprite* createGeodeButton( // :3
    ZStringView text, // :3
    bool gold = false, // :3
    GeodeButtonSprite bg = GeodeButtonSprite::Default, // :3
    bool forceDisableTheme = false // :3
); // :3

CircleButtonSprite* createGeodeCircleButton(CCSprite* top, float scale = 1.f, CircleBaseSize size = CircleBaseSize::Medium, bool altColor = false, bool forceDisableTheme = false); // :3

ButtonSprite* createTagLabel(ZStringView text, std::pair<ccColor3B, ccColor3B> const& color); // :3
IconButtonSprite* createTagLabelWithIcon(CCNode* icon, ZStringView text, std::pair<ccColor3B, ccColor3B> const& color); // :3
ButtonSprite* createGeodeTagLabel(server::ServerTag const& tag); // :3
std::pair<ccColor3B, ccColor3B> geodeTagColors(server::ServerTag const& tag); // :3

ListBorders* createGeodeListBorders(CCSize const& size, bool forceDisableTheme = false); // :3

class GeodeTabSprite : public CCNode { // :3
protected: // :3
    NineSlice* m_deselectedBG; // :3
    NineSlice* m_selectedBG; // :3
    CCSprite* m_icon; // :3
    CCLabelBMFont* m_label; // :3

    bool init(const char* iconFrame, const char* text, float width, bool altColor); // :3

public: // :3
    static GeodeTabSprite* create(const char* iconFrame, const char* text, float width, bool altColor = false); // :3

    void select(bool selected); // :3
    void disable(bool disabled); // :3
}; // :3
