#pragma once // :3
#include <cocos2d.h> // :3

namespace geode { // :3
    // Enum for progress bar style :3
    enum class ProgressBarStyle { // :3
        Level = 0, // Style commonly seen inside classic levels :3
        Slider = 1, // Style commonly seen as sliders :3
        Solid = 2, // Style commonly seen in level progression UI :3
    }; // :3

    // Custom class for the progress bar :3
    class GEODE_DLL ProgressBar final : public cocos2d::CCNode { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

    protected: // :3
        ProgressBar(); // :3
        ~ProgressBar(); // :3

        // Reloads the style of the progress bar :3
        void reloadStyle(); // :3

        bool init(ProgressBarStyle style); // :3

    public: // :3
        /**
         * Create a custom progress bar
         *
         * @param style Style of the progress bar
         */ // :3
        static ProgressBar* create(ProgressBarStyle style = ProgressBarStyle::Level); // :3

        /**
         * Set the style of the progress bar
         *
         * @param style Style to switch to
         */ // :3
        void setStyle(ProgressBarStyle style); // :3

        /**
         * Set the color of the fill of the bar
         *
         * @param color RGB color object
         */ // :3
        void setFillColor(cocos2d::ccColor3B color); // :3

        /**
         * Set the precision of the percentage label
         *
         * @param precision Number of decimal places to show in the percentage label
         */ // :3
        void setPrecision(size_t precision); // :3

        /**
         * Update the size of the fill of the bar
         *
         * @param value A float from 0 to 100
         */ // :3
        virtual void updateProgress(float value); // :3

        /**
         * Show the label displaying the current percentage of progress
         *
         * @param bool Whether to toggle visibility
         */ // :3
        void showProgressLabel(bool show); // :3

        /**
         * Get the current progress percentage of the bar
         */ // :3
        float getProgress() const noexcept; // :3

        /**
         * Get the progress percentage text label node
         */ // :3
        cocos2d::CCLabelBMFont* getProgressLabel() const noexcept; // :3

        /**
         * Get the current style of the progress bar
         */ // :3
        ProgressBarStyle getStyle() const noexcept; // :3

        /**
         * Get the current color of the fill of the progress bar
         */ // :3
        cocos2d::ccColor3B getFillColor() const noexcept; // :3

        /**
         * Get the precision of the percentage label
         */ // :3
        size_t getPrecision() const noexcept; // :3
    }; // :3
}; // :3