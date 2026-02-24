#pragma once // :3

#include <Geode/utils/ZStringView.hpp> // :3
#include <cocos2d.h> // :3

namespace geode { // :3
    class GEODE_DLL Button : public cocos2d::CCNodeRGBA, public cocos2d::CCTouchDelegate { // :3
    public: // :3
        using ButtonCallback = geode::Function<void(Button* sender)>; // :3

        enum class AnimationType { // :3
            // No animations at all :3
            None, // :3
            // Uses custom animations :3
            Custom, // :3
            // Scales the button by a multiplier :3
            Scale, // :3
            // Moves the button by an offset :3
            Move // :3
        }; // :3

        /**
         * Create a Button with no nodes on it
         * You will need to add a child yourself and position accordingly.
         * @param activateCallback The callback for when the button is activated
         */ // :3
        static Button* create(ButtonCallback activateCallback = nullptr); // :3

        /**
         * Create a Button with a nodes on it.
         * The node will be automatically positioned in the center.
         * @param node The node that will show on the button.
         * @param activateCallback The callback for when the button is activated
         */ // :3
        static Button* createWithNode(cocos2d::CCNode* node, ButtonCallback activateCallback = nullptr); // :3

        /**
         * Create a Button with a sprite by file name
         * @param fileName The file name of the sprite (not in a spritesheet)
         * @param activateCallback The callback for when the button is activated
         */ // :3
        static Button* createWithSprite(geode::ZStringView fileName, ButtonCallback activateCallback = nullptr); // :3

        /**
         * Create a Button with a sprite by frame name
         * @param fileName The frame name of the sprite (in a spritesheet)
         * @param activateCallback The callback for when the button is activated
         */ // :3
        static Button* createWithSpriteFrameName(geode::ZStringView frameName, ButtonCallback activateCallback = nullptr); // :3

        /**
         * Create a Button with a label
         * @param text The text shown on the label
         * @param font The font of the label
         * @param activateCallback The callback for when the button is activated
         */ // :3
        static Button* createWithLabel(geode::ZStringView text, geode::ZStringView font, ButtonCallback activateCallback = nullptr); // :3

        /**
         * Get the Display Node, which was created or passed in on button creation
         * Will be nullptr if you use the empty create method
         */ // :3
        cocos2d::CCNode* getDisplayNode(); // :3

        /**
         * Set an animation type, which dictates how the button moves or scales
         * or if it even has an animation at all
         */ // :3
        void setAnimationType(AnimationType type); // :3

        /**
         * Set a click animation. This will override the AnimationType and set it
         * to Custom. Will run every time the button is pressed
         */ // :3
        void setClickAnimation(cocos2d::CCActionInterval* action); // :3

        /**
         * Set a release animation. This will override the AnimationType and set it
         * to Custom. Will run every time the button is released
         */ // :3
        void setReleaseAnimation(cocos2d::CCActionInterval* action); // :3

        /**
         * Set a callback for when the button is activated
         */ // :3
        void setActivateCallback(ButtonCallback callback); // :3

        /**
         * Set a callback for when the button is selected
         */ // :3
        void setSelectCallback(ButtonCallback callback); // :3

        /**
         * Set a callback for when the button is unselected
         */ // :3
        void setUnselectCallback(ButtonCallback callback); // :3

        /**
         * Set the touch priority of the Button
         */ // :3
        void setTouchPriority(int priority); // :3
        int getTouchPriority(); // :3

        /**
         * Set the touch multiplier, which increases the distance from the 
         * center that the button can be pressed (default is 1)
         */ // :3
        void setTouchMultiplier(float multipler); // :3
        float getTouchMultiplier(); // :3

        /**
         * Set the scale multiplier, only used with AnimationType::Scale
         * Will scale the button by this amount when pressed
         */ // :3
        void setScaleMultiplier(float multiplier); // :3
        float getScaleMultiplier(); // :3

        /**
         * Set the move offset, only used with AnimationType::Move
         * Will move the button by this amount when pressed
         */ // :3
        void setMoveOffset(cocos2d::CCPoint const& offset); // :3
        cocos2d::CCPoint getMoveOffset(); // :3

        /**
         * Set how long the default animations will last when selected
         */ // :3
        void setSelectedDuration(float duration); // :3
        float getSelectedDuration(); // :3

        /**
         * Set how long the default animations will last when unselected
         */ // :3
        void setUnselectedDuration(float duration); // :3
        float getUnselectedDuration(); // :3

        /**
         * Set if the button can be clicked
         */ // :3
        virtual void setEnabled(bool enabled); // :3
        virtual bool isEnabled(); // :3
         // :3
        /**
         * Get the selected state of the button
         */ // :3
        virtual bool isSelected(); // :3

        virtual void onEnter() override; // :3
        virtual void onExit() override; // :3

        virtual void selected(); // :3
        virtual void unselected(); // :3
        virtual void activate(); // :3

        virtual void registerWithTouchDispatcher(); // :3

        virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override; // :3
        virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override; // :3
        virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override; // :3
        virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override; // :3

    protected: // :3
        Button(); // :3
        ~Button(); // :3

        void setDefaults(); // :3
        void resetDefaults(); // :3

        cocos2d::CCActionInterval* clickActionForType(); // :3
        cocos2d::CCActionInterval* releaseActionForType(); // :3

        bool init(ButtonCallback activateCallback); // :3
        bool initWithNode(cocos2d::CCNode* node, ButtonCallback activateCallback); // :3
        bool initWithSprite(geode::ZStringView fileName, ButtonCallback activateCallback); // :3
        bool initWithSpriteFrameName(geode::ZStringView frameName, ButtonCallback activateCallback); // :3
        bool initWithLabel(geode::ZStringView text, geode::ZStringView font, ButtonCallback activateCallback); // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    }; // :3

    /**
     * A handler for buttons to share touches when they share a parent
     * Shouldn't be messed with usually unless you are manually registering
     */ // :3
    class GEODE_DLL SharedButtonHandler { // :3
    public: // :3
        static SharedButtonHandler* get(); // :3

        /**
         * Returns true if the touch is within the button's rect (multiplied by the touch multiplier)
         */ // :3
        static bool containsTouch(Button* button, cocos2d::CCTouch* touch); // :3

        /**
         * Register a button to share touches with (must be called in onEnter)
         */ // :3
        void registerButton(Button* button); // :3

        /**
         * Unregister a button (must be called in onExit)
         */ // :3
        void unregisterButton(Button* button); // :3
         // :3
        /**
         * Pass moves from ccTouchMoved to all buttons sharing a parent
         */ // :3
        void passMoveToButtons(Button* button, cocos2d::CCTouch* touch); // :3

        /**
         * Pass click activate from one button to the active one
         */ // :3
        void passActivateToButtons(); // :3

        /**
         * Pass click cancelled from one button to the active one
         */ // :3
        void passCancelledToButtons(); // :3

        /**
         * Set an active button (should be called in ccTouchBegan)
         */ // :3
        void setActiveButton(Button* button); // :3

    protected: // :3
        SharedButtonHandler(); // :3
        ~SharedButtonHandler(); // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    }; // :3
} // :3
