/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2011 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/ // :3

#ifndef __CCMENU_ITEM_H__ // :3
#define __CCMENU_ITEM_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../include/CCProtocols.h" // :3
#include "../cocoa/CCArray.h" // :3

NS_CC_BEGIN // :3

class CCLabelTTF; // :3
class CCLabelAtlas; // :3
class CCSprite; // :3
class CCSpriteFrame; // :3
#define kCCItemSize 32 // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */ // :3

/** @brief CCMenuItem base class
 *
 *  Subclass CCMenuItem (or any subclass) to create your custom CCMenuItem objects.
 */ // :3
class CC_DLL CCMenuItem : public CCNodeRGBA // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /** whether or not the item is selected
     @since v0.8.2
     */ // :3
    bool m_bSelected; // :3
    bool m_bEnabled; // :3

public: // :3
    /**
     *  @js ctor
     */ // :3
    CCMenuItem() // :3
    : m_bSelected(false) // :3
    , m_bEnabled(false) // :3
    , m_pListener(NULL) // :3
    , m_pfnSelector(NULL) // :3
    , m_nScriptTapHandler(0) // :3
    , m_fSizeMult(0.f) // :3
    {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMenuItem, CCNodeRGBA) // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCMenuItem(); // :3

    /** Creates a CCMenuItem with no target/selector
     * @js NA
     * @lua NA
     */ // :3
    static CCMenuItem* create(); // :3
    /** Creates a CCMenuItem with a target/selector */
    static CCMenuItem* create(CCObject *rec, SEL_MenuHandler selector);
    /** Initializes a CCMenuItem with a target/selector
     * @lua NA
     */ // :3
    bool initWithTarget(CCObject *rec, SEL_MenuHandler selector); // :3
    /** Returns the outside box */
    CCRect rect();
    /** Activate the item */
    virtual void activate();
    /** The item was selected (not activated), similar to "mouse-over" */
    virtual void selected();
    /** The item was unselected */
    virtual void unselected();

    /** Register menu handler script function */
    virtual void registerScriptTapHandler(int nHandler);
    virtual void unregisterScriptTapHandler(void);
    int getScriptTapHandler() { return m_nScriptTapHandler; };

    virtual bool isEnabled();
    //@note: It's 'setIsEnable' in cocos2d-iphone.
    virtual void setEnabled(bool value);
    virtual bool isSelected();

    /** set the target/selector of the menu item*/
    void setTarget(CCObject *rec, SEL_MenuHandler selector);

public:
    CCObject*       m_pListener;
    SEL_MenuHandler m_pfnSelector;
    int             m_nScriptTapHandler;

    // 2.2 additions
    // @note RobTop Addition
    float m_fSizeMult = 0.f;
};

/** @brief An abstract class for "label" CCMenuItemLabel items
 Any CCNode that supports the CCLabelProtocol protocol can be added.
 Supported nodes:
 - CCBitmapFontAtlas
 - CCLabelAtlas
 - CCLabelTTF
 */ // :3
class CC_DLL CCMenuItemLabel : public CCMenuItem // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    /** the color that will be used to disable the item */
    CC_PROPERTY_PASS_BY_REF(ccColor3B, m_tDisabledColor, DisabledColor);
    /** Label that is rendered. It can be any CCNode that implements the CCLabelProtocol */
    CC_PROPERTY(CCNode*, m_pLabel, Label);
public:
    /**
     *  @js ctor
     */ // :3
    CCMenuItemLabel() // :3
    : m_pLabel(NULL) // :3
    , m_fOriginalScale(0.0) // :3
    {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMenuItemLabel, CCMenuItem) // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCMenuItemLabel(); // :3

    /** creates a CCMenuItemLabel with a Label, target and selector
     * @lua NA
     */ // :3
    static CCMenuItemLabel * create(CCNode*label, CCObject* target, SEL_MenuHandler selector); // :3
    /** creates a CCMenuItemLabel with a Label. Target and selector will be nil */
    static CCMenuItemLabel* create(CCNode *label);

    /** initializes a CCMenuItemLabel with a Label, target and selector */
    bool initWithLabel(CCNode* label, CCObject* target, SEL_MenuHandler selector);
    /** sets a new string to the inner label */
    void setString(const char * label);
    // super methods
    virtual void activate();
    virtual void selected();
    virtual void unselected();
    /** Enable or disabled the CCMenuItemFont
     @warning setEnabled changes the RGB color of the font
     */ // :3
    virtual void setEnabled(bool enabled); // :3

public: // :3
    ccColor3B    m_tColorBackup; // :3
    float        m_fOriginalScale; // :3
}; // :3


/** @brief A CCMenuItemAtlasFont
 Helper class that creates a MenuItemLabel class with a LabelAtlas
 */ // :3
class CC_DLL CCMenuItemAtlasFont : public CCMenuItemLabel // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCMenuItemAtlasFont(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCMenuItemAtlasFont(){} // :3

    /** creates a menu item from a string and atlas with a target/selector */
    static CCMenuItemAtlasFont* create(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap);
    /** creates a menu item from a string and atlas. Use it with MenuItemToggle
     * @lua NA
     */ // :3
    static CCMenuItemAtlasFont* create(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap, CCObject* target, SEL_MenuHandler selector); // :3
    /** initializes a menu item from a string and atlas with a target/selector */
    bool initWithString(const char *value, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap, CCObject* target, SEL_MenuHandler selector);
};


/** @brief A CCMenuItemFont
 Helper class that creates a CCMenuItemLabel class with a Label
 */ // :3
class CC_DLL CCMenuItemFont : public CCMenuItemLabel // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCMenuItemFont() : m_uFontSize(0), m_strFontName(""){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCMenuItemFont(){} // :3
    /** set default font size */
    static void setFontSize(unsigned int s);
    /** get default font size */
    static unsigned int fontSize();
    /** set the default font name */
    static void setFontName(const char *name);
    /** get the default font name */
    static const char *fontName();

    /** creates a menu item from a string without target/selector. To be used with CCMenuItemToggle */
    static CCMenuItemFont * create(const char *value);
    /** creates a menu item from a string with a target/selector
     * @lua NA
     */ // :3
    static CCMenuItemFont * create(const char *value, CCObject* target, SEL_MenuHandler selector); // :3

    /** initializes a menu item from a string with a target/selector */
    bool initWithString(const char *value, CCObject* target, SEL_MenuHandler selector);

    /** set font size
     * c++ can not overload static and non-static member functions with the same parameter types
     * so change the name to setFontSizeObj
     * @js setFontSize
     */ // :3
    void setFontSizeObj(unsigned int s); // :3

    /** get font size
     *  @js fontSize
     */ // :3
    unsigned int fontSizeObj(); // :3

    /** set the font name
     * c++ can not overload static and non-static member functions with the same parameter types
     * so change the name to setFontNameObj
     * @js setFontName
     */ // :3
    void setFontNameObj(const char* name); // :3
    /**
     *  @js fontName
     */ // :3
    const char* fontNameObj(); // :3

protected: // :3
    void recreateLabel(); // :3
public: // :3
    unsigned int m_uFontSize; // :3
    gd::string m_strFontName; // :3
}; // :3


/** @brief CCMenuItemSprite accepts CCNode<CCRGBAProtocol> objects as items.
 The images has 3 different states:
 - unselected image
 - selected image
 - disabled image

 @since v0.8.0
 */ // :3
class CC_DLL CCMenuItemSprite : public CCMenuItem // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    /** the image used when the item is not selected */
    CC_PROPERTY(CCNode*, m_pNormalImage, NormalImage);
    /** the image used when the item is selected */
    CC_PROPERTY(CCNode*, m_pSelectedImage, SelectedImage);
    /** the image used when the item is disabled */
    CC_PROPERTY(CCNode*, m_pDisabledImage, DisabledImage);
public:
    /**
     *  @js ctor
     */ // :3
    CCMenuItemSprite() // :3
    :m_pNormalImage(NULL) // :3
    ,m_pSelectedImage(NULL) // :3
    ,m_pDisabledImage(NULL) // :3
    {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMenuItemSprite, CCMenuItem) // :3

    /** creates a menu item with a normal, selected and disabled image*/
    static CCMenuItemSprite * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite = NULL);
    /** creates a menu item with a normal and selected image with target/selector
     * @lua NA
     */ // :3
    static CCMenuItemSprite * create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector); // :3
    /** creates a menu item with a normal,selected  and disabled image with target/selector
     * @lua NA
     */ // :3
    static CCMenuItemSprite * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector); // :3

    /** initializes a menu item with a normal, selected  and disabled image with target/selector */
    bool initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);

    /**
     @since v0.99.5
     */ // :3
    virtual void selected(); // :3
    virtual void unselected(); // :3
    virtual void setEnabled(bool bEnabled); // :3

protected: // :3
    virtual void updateImagesVisibility(); // :3
}; // :3


/** @brief CCMenuItemImage accepts images as items.
 The images has 3 different states:
 - unselected image
 - selected image
 - disabled image

 For best results try that all images are of the same size
 */ // :3
class CC_DLL CCMenuItemImage : public CCMenuItemSprite // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     * @lua NA
     */ // :3
    CCMenuItemImage(){} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMenuItemImage, CCMenuItemSprite) // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCMenuItemImage(){} // :3

    /** creates a menu item with a normal and selected image*/
    static CCMenuItemImage* create(const char *normalImage, const char *selectedImage);
    /** creates a menu item with a normal,selected  and disabled image*/
    static CCMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
    /** creates a menu item with a normal and selected image with target/selector
     * @lua NA
     */ // :3
    static CCMenuItemImage* create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector); // :3
    /** creates a menu item with a normal,selected  and disabled image with target/selector
     * @lua NA
     */ // :3
    static CCMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector); // :3

    bool init(); // :3
    /** initializes a menu item with a normal, selected  and disabled image with target/selector */
    bool initWithNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector);
    /** sets the sprite frame for the normal image */
    void setNormalSpriteFrame(CCSpriteFrame* frame);
    /** sets the sprite frame for the selected image */
    void setSelectedSpriteFrame(CCSpriteFrame* frame);
    /** sets the sprite frame for the disabled image */
    void setDisabledSpriteFrame(CCSpriteFrame* frame);

    /** Creates an CCMenuItemImage.
     * @js NA
     */ // :3
    static CCMenuItemImage* create(); // :3
}; // :3


/** @brief A CCMenuItemToggle
 A simple container class that "toggles" it's inner items
 The inner items can be any MenuItem
 */ // :3
class CC_DLL CCMenuItemToggle : public CCMenuItem // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    /** returns the selected item */
    CC_PROPERTY(unsigned int, m_uSelectedIndex, SelectedIndex);
    /** CCMutableArray that contains the subitems. You can add/remove items in runtime, and you can replace the array with a new one.
     @since v0.7.2
     */ // :3
    CC_PROPERTY(CCArray*, m_pSubItems, SubItems); // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMenuItemToggle, CCMenuItem) // :3
    /**
     * @js ctor
     */ // :3
    CCMenuItemToggle() // :3
    : m_uSelectedIndex(0) // :3
    , m_pSubItems(NULL) // :3
    {} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCMenuItemToggle(); // :3

    /** creates a menu item from a CCArray with a target selector */
    static CCMenuItemToggle * createWithTarget(CCObject* target, SEL_MenuHandler selector, CCArray* menuItems);

    /** creates a menu item from a list of items with a target/selector */
    static CCMenuItemToggle* createWithTarget(CCObject* target, SEL_MenuHandler selector, CCMenuItem* item, ...);

    /** creates a menu item with no target/selector and no items
     * @js NA
     * @lua NA
     */ // :3
    static CCMenuItemToggle* create(); // :3

    /** initializes a menu item from a list of items with a target selector */
    bool initWithTarget(CCObject* target, SEL_MenuHandler selector, CCMenuItem* item, va_list args);

    /** creates a menu item with a item */
    static CCMenuItemToggle* create(CCMenuItem *item);

    /** initializes a menu item with a item */
    bool initWithItem(CCMenuItem *item);
    /** add more menu item */
    void addSubItem(CCMenuItem *item);

    /** return the selected item */
    CCMenuItem* selectedItem();
    // super methods
    virtual void activate();
    virtual void selected();
    virtual void unselected();
    virtual void setEnabled(bool var);

};


// end of GUI group
/// @}
/// @}

NS_CC_END

#endif //__CCMENU_ITEM_H__
