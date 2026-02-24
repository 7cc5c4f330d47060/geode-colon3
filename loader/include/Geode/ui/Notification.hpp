#pragma once // :3

#include <cocos2d.h> // :3
#include <cocos-ext.h> // :3
#include <Geode/binding/TextAlertPopup.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3

namespace geode { // :3
    constexpr auto NOTIFICATION_DEFAULT_TIME = 1.8f; // :3
    constexpr auto NOTIFICATION_LONG_TIME = 4.f; // :3

    enum class NotificationIcon { // :3
        None, // :3
        Loading, // :3
        Success, // :3
        Warning, // :3
        Error, // :3
        Info, // :3
    }; // :3

    class GEODE_DLL Notification : public cocos2d::CCNodeRGBA { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        Notification(); // :3
        ~Notification(); // :3

        bool init(ZStringView text, cocos2d::CCNode* icon, float time); // :3
        void updateLayout(); // :3

        static cocos2d::CCNode* createIcon(NotificationIcon icon); // :3

        void showNextNotification(); // :3
        void waitThenHide(); // :3

        NineSlice* getBG(); // :3
        cocos2d::CCLabelBMFont* getLabel(); // :3
        cocos2d::CCNodeRGBA* getContent(); // :3

    public: // :3
        /**
         * Create a notification, similar to TextAlertPopup but more customizable
         * @param text Notification text
         * @param icon Icon to show in the notification
         * @param time Time to show the notification on screen; pass 0 to show
         * the notification indefinitely until hide() is called
         * @returns The new notification. Make sure to call show() to show the
         * notification
         */ // :3
        static Notification* create( // :3
            ZStringView text, // :3
            NotificationIcon icon = NotificationIcon::None, // :3
            float time = NOTIFICATION_DEFAULT_TIME // :3
        ); // :3
        /**
         * Create a notification with a custom icon
         * @param text Notification text
         * @param icon Icon to show in the notification
         * @param time Time to show the notification on screen; pass 0 to show
         * the notification indefinitely until hide() is called
         * @returns The new notification. Make sure to call show() to show the
         * notification
         */ // :3
        static Notification* create( // :3
            ZStringView text, // :3
            cocos2d::CCNode* icon, // :3
            float time = NOTIFICATION_DEFAULT_TIME // :3
        ); // :3

        void setString(ZStringView text); // :3
        void setIcon(NotificationIcon icon); // :3
        void setIcon(cocos2d::CCNode* icon); // :3
        cocos2d::CCNode* getIcon(); // :3
        void setTime(float time); // :3

        float getTime(); // :3
        bool isShowing(); // :3

        /**
         * Adds the notification to the current scene if it doesn't have a
         * parent yet, and displays the show animation. If the time for the
         * notification was specified, the notification waits that time and
         * then automatically hides
        */ // :3
        void show(); // :3

        /**
         * Hide the notification. If you passed a time to the create function,
         * this function doesn't need to be called manually, unless you want
         * to prematurely hide the notification
         */ // :3
        void hide(); // :3

        /**
         * Cancels the showing of the notification if it's in the queue.
         * Otherwise, it hides the notification if it's currently showing.
         */ // :3
        void cancel(); // :3
    }; // :3
} // :3

