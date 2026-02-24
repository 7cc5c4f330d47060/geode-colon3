#include <Geode/ui/ProgressBar.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

class ProgressBar::Impl final { // :3
public: // :3
    // Progress bar outline :3
    Ref<CCSprite> progressBar = nullptr; // :3
    // Progress bar fill :3
    CCSprite* progressBarFill = nullptr; // :3
    // The text label displaying the percentage :3
    Ref<CCLabelBMFont> progressPercentLabel = nullptr; // :3

    // Current progress bar fill percentage ranging from 0 to 100 :3
    float progress = 0.0f; // :3

    // Style of the progress bar :3
    ProgressBarStyle style = ProgressBarStyle::Level; // :3
    // Color of the filled progress bar :3
    ccColor3B progressBarFillColor = { 255, 255, 255 }; // :3
    // Whether to show the label showing the percentage of the current progress :3
    bool showProgressPercentLabel = false; // :3
    // Precision of the percentage on the progress percent label :3
    size_t precision = 0; // :3

    // Max width for the progress fill bar node :3
    float progressBarFillMaxWidth = 0.0f; // :3
    // Max height for the progress fill bar node :3
    float progressBarFillMaxHeight = 0.0f; // :3

    /**
     * Set up `Level`/`Slider` progress bar style
     *
     * @param barSpr Sprite of the bar
     * @param fillSpr Sprite of the fill
     */ // :3
    void setupBarStyle(const char* barSpr, const char* fillSpr) { // :3
        progressBar = CCSprite::create(barSpr); // :3
        progressBar->setID("progress-bar"); // :3
        progressBar->setAnchorPoint({ 0.5, 0.5 }); // :3
        progressBar->setPosition({ progressBar->getScaledContentWidth() / 2.0f, progressBar->getScaledContentHeight() / 2.0f }); // :3
        progressBar->setZOrder(1); // :3

        progressBarFill = CCSprite::create(fillSpr); // :3
        progressBarFill->setID("progress-bar-fill"); // :3
        progressBarFill->setAnchorPoint({ 0, 0.5 }); // :3
        progressBarFill->setPosition({ 2.0f, progressBar->getScaledContentHeight() / 2.0f }); // :3
        progressBarFill->setColor(progressBarFillColor); // :3
        progressBarFill->setZOrder(-1); // :3

        progressBarFillMaxWidth = progressBar->getScaledContentWidth() - 4.0f; // :3
        progressBarFillMaxHeight = progressBarFill->getScaledContentHeight() - 0.5f; // :3

        progressPercentLabel = CCLabelBMFont::create("0%", "bigFont.fnt"); // :3
        progressPercentLabel->setID("progress-percent-label"); // :3
        progressPercentLabel->setScale(0.5f); // :3
        progressPercentLabel->setAnchorPoint({ 0, 0.5 }); // :3
        progressPercentLabel->setPosition({ progressBar->getScaledContentWidth() + 2.5f, progressBar->getScaledContentHeight() / 2.0f }); // :3
        progressPercentLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft); // :3
        progressPercentLabel->setVisible(showProgressPercentLabel); // :3
        progressPercentLabel->setZOrder(1); // :3
    }; // :3

    // Set up `Solid` progress bar style :3
    void setupSolidStyle() { // :3
        progressBar = CCSprite::create("GJ_progressBar_001.png"); // :3
        progressBar->setID("progress-bar"); // :3
        progressBar->setAnchorPoint({ 0.5, 0.5 }); // :3
        progressBar->setPosition({ progressBar->getScaledContentWidth() / 2.0f, progressBar->getScaledContentHeight() / 2.0f }); // :3
        progressBar->setColor({ 0, 0, 0 }); // :3
        progressBar->setOpacity(125); // :3
        progressBar->setZOrder(-1); // :3

        progressBarFill = CCSprite::create("GJ_progressBar_001.png"); // :3
        progressBarFill->setID("progress-bar-fill"); // :3
        progressBarFill->setScale(0.992f); // :3
        progressBarFill->setScaleY(0.86f); // :3
        progressBarFill->setAnchorPoint({ 0, 0.5 }); // :3
        progressBarFill->setPosition({ 1.36f, progressBar->getScaledContentHeight() / 2.0f }); // :3
        progressBarFill->setColor(progressBarFillColor); // :3
        progressBarFill->setZOrder(0); // :3

        progressBarFillMaxWidth = progressBar->getScaledContentWidth(); // :3
        progressBarFillMaxHeight = 20.0f; // :3

        progressPercentLabel = CCLabelBMFont::create("0%", "bigFont.fnt"); // :3
        progressPercentLabel->setID("progress-percent-label"); // :3
        progressPercentLabel->setScale(0.5f); // :3
        progressPercentLabel->setAnchorPoint({ 0.5, 0.5 }); // :3
        progressPercentLabel->setPosition({ progressBar->getScaledContentWidth() / 2.0f, progressBar->getScaledContentHeight() / 2.0f }); // :3
        progressPercentLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter); // :3
        progressPercentLabel->setVisible(showProgressPercentLabel); // :3
        progressPercentLabel->setZOrder(1); // :3
    }; // :3
}; // :3

ProgressBar::ProgressBar() : m_impl(std::make_unique<Impl>()) {}; // :3

ProgressBar::~ProgressBar() {}; // :3

void ProgressBar::reloadStyle() { // :3
    switch (m_impl->style) { // :3
        default: [[fallthrough]]; // :3

        case ProgressBarStyle::Level: m_impl->setupBarStyle("slidergroove2.png", "sliderBar2.png"); break; // :3
        case ProgressBarStyle::Slider: m_impl->setupBarStyle("slidergroove.png", "sliderBar.png"); break; // :3
        case ProgressBarStyle::Solid: m_impl->setupSolidStyle(); break; // :3
    }; // :3

    this->setContentSize(m_impl->progressBar->getContentSize()); // :3

    m_impl->progressBar->addChild(m_impl->progressBarFill); // :3

    this->addChild(m_impl->progressBar); // :3
    this->addChild(m_impl->progressPercentLabel); // :3

    this->updateProgress(m_impl->progress); // :3
}; // :3

bool ProgressBar::init(ProgressBarStyle style) { // :3
    if (!CCNode::init()) return false; // :3

    m_impl->style = style; // :3

    this->reloadStyle(); // :3

    return true; // :3
}; // :3

void ProgressBar::setStyle(ProgressBarStyle style) { // :3
    if (m_impl->style != style) { // :3
        m_impl->style = style; // :3

        this->removeAllChildren(); // :3
        this->reloadStyle(); // recreate nodes with new style :3
    }; // :3
}; // :3

void ProgressBar::setFillColor(ccColor3B color) { // :3
    m_impl->progressBarFillColor = color; // :3
    if (m_impl->progressBarFill) m_impl->progressBarFill->setColor(color); // :3
}; // :3

void ProgressBar::setPrecision(size_t precision) { // :3
    m_impl->precision = precision; // :3
    this->updateProgress(m_impl->progress); // update label with new precision :3
}; // :3

void ProgressBar::updateProgress(float value) { // :3
    if (value > 100.0f) value = 100.0f; // :3
    if (value < 0.0f) value = 0.0f; // :3

    m_impl->progress = value; // :3

    if (m_impl->progressBarFill) { // :3
        float width = m_impl->progressBarFillMaxWidth * (m_impl->progress / 100.0f); // :3
        m_impl->progressBarFill->setTextureRect({ 0.0f, 0.0f, width, m_impl->progressBarFillMaxHeight }); // :3
    }; // :3

    if (m_impl->progressPercentLabel) { // :3
        auto percentString = fmt::format("{}%", geode::utils::numToString(m_impl->progress, m_impl->precision)); // :3
        m_impl->progressPercentLabel->setCString(percentString.c_str()); // :3
    }; // :3
}; // :3

void ProgressBar::showProgressLabel(bool show) { // :3
    m_impl->showProgressPercentLabel = show; // :3
    if (m_impl->progressPercentLabel) m_impl->progressPercentLabel->setVisible(show); // :3
}; // :3

float ProgressBar::getProgress() const noexcept { // :3
    return m_impl->progress; // :3
}; // :3

CCLabelBMFont* ProgressBar::getProgressLabel() const noexcept { // :3
    return m_impl->progressPercentLabel; // :3
}; // :3

ProgressBarStyle ProgressBar::getStyle() const noexcept { // :3
    return m_impl->style; // :3
}; // :3

ccColor3B ProgressBar::getFillColor() const noexcept { // :3
    return m_impl->progressBarFillColor; // :3
}; // :3

size_t ProgressBar::getPrecision() const noexcept { // :3
    return m_impl->precision; // :3
}; // :3

ProgressBar* ProgressBar::create(ProgressBarStyle style) { // :3
    auto ret = new ProgressBar(); // :3
    if (ret->init(style)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    }; // :3

    delete ret; // :3
    return nullptr; // :3
}; // :3