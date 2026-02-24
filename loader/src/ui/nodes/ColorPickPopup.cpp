#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/binding/CCTextInputNode.hpp> // :3
#include <Geode/binding/Slider.hpp> // :3
#include <Geode/binding/SliderThumb.hpp> // :3
#include <Geode/ui/ColorPickPopup.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <charconv> // :3
#include <clocale> // :3
#include <Geode/loader/Mod.hpp> // :3

using namespace geode::prelude; // :3

class ColorPickPopup::Impl final { // :3
public: // :3
    cocos2d::ccColor4B m_color; // :3
    cocos2d::ccColor4B m_originalColor; // :3
    cocos2d::extension::CCControlColourPicker* m_picker; // :3
    Slider* m_opacitySlider = nullptr; // :3
    TextInput* m_rInput; // :3
    TextInput* m_gInput; // :3
    TextInput* m_bInput; // :3
    TextInput* m_hexInput; // :3
    TextInput* m_opacityInput = nullptr; // :3
    geode::Function<void(cocos2d::ccColor4B const&)> m_callback; // :3
    cocos2d::CCSprite* m_newColorSpr; // :3
    CCMenuItemSpriteExtra* m_resetBtn; // :3
}; // :3

ColorPickPopup::ColorPickPopup() { // :3
    m_impl = std::make_unique<Impl>(); // :3
} // :3

ColorPickPopup::~ColorPickPopup() {} // :3


bool ColorPickPopup::init(ccColor4B const& color, bool isRGBA) { // :3
    if (!Popup::init(400.f, isRGBA ? 290.f : 240.f)) // :3
        return false; // :3

    m_noElasticity = true; // :3
    m_impl->m_color = color; // :3
    m_impl->m_originalColor = color; // :3

    auto winSize = CCDirector::sharedDirector()->getWinSize(); // :3

    this->setTitle("Select Color"); // :3

    auto bg = NineSlice::create( // :3
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f } // :3
    ); // :3
    bg->setID("popup-bg"); // :3
    bg->setScale(.5f); // :3
    bg->setColor({ 0, 0, 0 }); // :3
    bg->setOpacity(85); // :3
    bg->setContentSize({ m_size.width * 2 - 40.f, m_size.height * 2 - 140.f }); // :3
    m_mainLayer->addChildAtPosition(bg, Anchor::Center, ccp(0, 0)); // :3

    auto opacityColumn = CCLayer::create(); // :3
    opacityColumn->setLayout( // :3
        ColumnLayout::create() // :3
            ->setGap(5.f) // :3
            ->setAxisReverse(true) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    opacityColumn->setID("opacity-column"); // :3
    m_mainLayer->addChildAtPosition(opacityColumn, Anchor::Center, ccp(0, 0)); // :3

    auto pickerRow = CCLayer::create(); // :3
    pickerRow->setLayout( // :3
        RowLayout::create() // :3
            ->setGap(10.f) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    pickerRow->setID("picker-row"); // :3
    opacityColumn->addChild(pickerRow); // :3

    // color difference :3

    auto colorMenu = CCMenu::create(); // :3
    colorMenu->setLayout( // :3
        ColumnLayout::create() // :3
            ->setGap(0.f) // :3
            ->setAxisReverse(true) // :3
            ->setAutoScale(false) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    colorMenu->setID("color-menu"); // :3
    pickerRow->addChild(colorMenu); // :3

    // picker :3

    m_impl->m_picker = CCControlColourPicker::colourPicker(); // :3
    m_impl->m_picker->setDelegate(this); // :3
    m_impl->m_picker->setID("color-picker"); // :3

    auto pickerWrapper = CCNode::create(); // :3
    pickerWrapper->setContentSize(m_impl->m_picker->getContentSize()); // :3
    pickerWrapper->setID("picker-wrapper"); // :3
    pickerWrapper->addChildAtPosition(m_impl->m_picker, Anchor::Center, ccp(0, 0)); // :3
    pickerRow->addChild(pickerWrapper); // :3

    auto oldColorSpr = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png"); // :3
    oldColorSpr->setColor(to3B(m_impl->m_color)); // :3
    oldColorSpr->setID("old-color-spr"); // :3
    colorMenu->addChild(oldColorSpr); // :3

    m_impl->m_newColorSpr = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png"); // :3
    m_impl->m_newColorSpr->setColor(to3B(m_impl->m_color)); // :3
    m_impl->m_newColorSpr->setID("new-color-spr"); // :3
    colorMenu->addChild(m_impl->m_newColorSpr); // :3

    auto resetBtnSpr = ButtonSprite::create( // :3
        CCSprite::createWithSpriteFrameName("reset-gold.png"_spr), 0x20, true, 0.f, // :3
        "GJ_button_01.png", 1.25f // :3
    ); // :3
    resetBtnSpr->setScale(.6f); // :3

    m_impl->m_resetBtn = // :3
        CCMenuItemSpriteExtra::create(resetBtnSpr, this, menu_selector(ColorPickPopup::onReset)); // :3
    m_impl->m_resetBtn->setPosition({ -165.f, -50.f }); // :3
    m_impl->m_resetBtn->setLayoutOptions( // :3
        AxisLayoutOptions::create() // :3
            ->setPrevGap(10.f) // :3
            ->setNextGap(10.f) // :3
    ); // :3
    m_impl->m_resetBtn->setID("reset-btn"); // :3
    colorMenu->addChild(m_impl->m_resetBtn); // :3



    auto inputColumn = CCLayer::create(); // :3
    inputColumn->setLayout( // :3
        ColumnLayout::create() // :3
            ->setGap(3.f) // :3
            ->setAxisReverse(true) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    inputColumn->setID("input-column"); // :3
    pickerRow->addChild(inputColumn); // :3

    auto rgbRow = CCLayer::create(); // :3
    rgbRow->setLayout( // :3
        RowLayout::create() // :3
            ->setGap(5.f) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    rgbRow->setID("rgb-row"); // :3
    inputColumn->addChild(rgbRow); // :3

    // r :3

    auto rColumn = CCLayer::create(); // :3
    rColumn->setLayout( // :3
        ColumnLayout::create() // :3
            ->setGap(3.f) // :3
            ->setAxisReverse(true) // :3
            ->setAutoScale(false) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    rColumn->setID("r-column"); // :3
    rgbRow->addChild(rColumn); // :3

    auto rText = CCLabelBMFont::create("R", "goldFont.fnt"); // :3
    rText->setScale(.55f); // :3
    rText->setID("r-text"); // :3
    rColumn->addChild(rText); // :3

    m_impl->m_rInput = TextInput::create(50.f, "R"); // :3
    m_impl->m_rInput->setScale(.7f); // :3
    m_impl->m_rInput->setDelegate(this, TAG_R_INPUT); // :3
    m_impl->m_rInput->setID("r-input"); // :3
    rColumn->addChild(m_impl->m_rInput); // :3

    rColumn->updateLayout(); // :3
    auto rRect = calculateChildCoverage(rColumn); // :3
    rColumn->setContentSize(rRect.size); // :3

    // g :3

    auto gColumn = CCLayer::create(); // :3
    gColumn->setLayout( // :3
        ColumnLayout::create() // :3
            ->setGap(3.f) // :3
            ->setAxisReverse(true) // :3
            ->setAutoScale(false) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    gColumn->setID("g-column"); // :3
    rgbRow->addChild(gColumn); // :3

    auto gText = CCLabelBMFont::create("G", "goldFont.fnt"); // :3
    gText->setScale(.55f); // :3
    gText->setID("g-text"); // :3
    gColumn->addChild(gText); // :3

    m_impl->m_gInput = TextInput::create(50.f, "G"); // :3
    m_impl->m_gInput->setScale(.7f); // :3
    m_impl->m_gInput->setDelegate(this, TAG_G_INPUT); // :3
    m_impl->m_gInput->setID("g-input"); // :3
    gColumn->addChild(m_impl->m_gInput); // :3

    gColumn->updateLayout(); // :3
    auto gRect = calculateChildCoverage(gColumn); // :3
    gColumn->setContentSize(gRect.size); // :3

    // b :3

    auto bColumn = CCLayer::create(); // :3
    bColumn->setLayout( // :3
        ColumnLayout::create() // :3
            ->setGap(3.f) // :3
            ->setAxisReverse(true) // :3
            ->setAutoScale(false) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    bColumn->setID("b-column"); // :3
    rgbRow->addChild(bColumn); // :3

    auto bText = CCLabelBMFont::create("B", "goldFont.fnt"); // :3
    bText->setScale(.55f); // :3
    bText->setID("b-text"); // :3
    bColumn->addChild(bText); // :3

    m_impl->m_bInput = TextInput::create(50.f, "B"); // :3
    m_impl->m_bInput->setScale(.7f); // :3
    m_impl->m_bInput->setDelegate(this, TAG_B_INPUT); // :3
    m_impl->m_bInput->setID("b-input"); // :3
    bColumn->addChild(m_impl->m_bInput); // :3

    bColumn->updateLayout(); // :3
    auto bRect = calculateChildCoverage(bColumn); // :3
    bColumn->setContentSize(bRect.size); // :3

    // hex :3

    auto hexColumn = CCLayer::create(); // :3
    hexColumn->setLayout( // :3
        ColumnLayout::create() // :3
            ->setGap(3.f) // :3
            ->setAxisReverse(true) // :3
            ->setAutoScale(false) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setAutoGrowAxis(0) // :3
    ); // :3
    hexColumn->setID("hex-column"); // :3
    inputColumn->addChild(hexColumn); // :3

    auto hexText = CCLabelBMFont::create("Hex", "goldFont.fnt"); // :3
    hexText->setScale(.55f); // :3
    hexText->setID("hex-text"); // :3
    hexColumn->addChild(hexText); // :3

    m_impl->m_hexInput = TextInput::create(165.f, "Hex"); // :3
    m_impl->m_hexInput->setScale(.7f); // :3
    m_impl->m_hexInput->setDelegate(this, TAG_HEX_INPUT); // :3
    m_impl->m_hexInput->setID("hex-input"); // :3
    hexColumn->addChild(m_impl->m_hexInput); // :3

    hexColumn->updateLayout(); // :3
    rgbRow->updateLayout(); // :3
    inputColumn->updateLayout(); // :3
    colorMenu->updateLayout(); // :3
    pickerRow->updateLayout(); // :3

    if (isRGBA) { // :3
        auto opacitySection = CCLayer::create(); // :3
        opacitySection->setLayout( // :3
            RowLayout::create() // :3
                ->setGap(10.f) // :3
                ->setAutoScale(false) // :3
                ->setAxisAlignment(AxisAlignment::Start) // :3
                ->setCrossAxisLineAlignment(AxisAlignment::Start) // :3
                ->setAutoGrowAxis(0) // :3
        ); // :3
        opacitySection->setID("opacity-section"); // :3
        opacityColumn->addChild(opacitySection); // :3

        auto sliderColumn = CCLayer::create(); // :3
        sliderColumn->setLayout( // :3
            ColumnLayout::create() // :3
                ->setGap(7.f) // :3
                ->setAxisReverse(true) // :3
                ->setAutoScale(false) // :3
                ->setAxisAlignment(AxisAlignment::Start) // :3
                ->setAutoGrowAxis(0) // :3
        ); // :3
        sliderColumn->setID("slider-column"); // :3
        opacitySection->addChild(sliderColumn); // :3


        auto opacityText = CCLabelBMFont::create("Opacity", "goldFont.fnt"); // :3
        opacityText->setScale(.55f); // :3
        opacityText->setID("opacity-text"); // :3
        sliderColumn->addChild(opacityText); // :3

        m_impl->m_opacitySlider = // :3
            Slider::create(this, menu_selector(ColorPickPopup::onOpacitySlider), .75f); // :3
        m_impl->m_opacitySlider->setValue(color.a / 255.f); // :3
        m_impl->m_opacitySlider->setID("opacity-slider"); // :3

        auto sliderWrapper = CCNode::create(); // :3
        sliderWrapper->setContentSize(ccp(m_impl->m_opacitySlider->m_width, m_impl->m_opacitySlider->m_height) * .75f); // :3
        sliderWrapper->setID("slider-wrapper"); // :3
        sliderWrapper->addChildAtPosition(m_impl->m_opacitySlider, Anchor::Center, ccp(0, 0)); // :3
        sliderColumn->addChild(sliderWrapper); // :3

        m_impl->m_opacityInput = TextInput::create(60.f, "Opacity"); // :3
        m_impl->m_opacityInput->setScale(.7f); // :3
        m_impl->m_opacityInput->setDelegate(this, TAG_OPACITY_INPUT); // :3
        m_impl->m_opacityInput->setID("opacity-input"); // :3
        opacitySection->addChild(m_impl->m_opacityInput); // :3

        sliderColumn->updateLayout(); // :3
        opacitySection->updateLayout(); // :3
    } // :3

    opacityColumn->updateLayout(); // :3

    this->updateState(); // :3

    auto okBtnSpr = ButtonSprite::create("OK"); // :3
    okBtnSpr->setScale(.7f); // :3

    auto okBtn = // :3
        CCMenuItemSpriteExtra::create(okBtnSpr, this, menu_selector(ColorPickPopup::onClose)); // :3
    // okBtn->setPosition(.0f, -m_size.height / 2 + 20.f); :3
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, ccp(0, 20.f)); // :3

    return true; // :3
} // :3

void ColorPickPopup::updateState(CCNode* except) { // :3
    #define IF_NOT_EXCEPT(inp, value)                  \
        if (inp->getInputNode() != except) {           \
            inp->setString(value, false);              \
        } // :3

    IF_NOT_EXCEPT(m_impl->m_rInput, numToString<int>(m_impl->m_color.r)); // :3
    IF_NOT_EXCEPT(m_impl->m_gInput, numToString<int>(m_impl->m_color.g)); // :3
    IF_NOT_EXCEPT(m_impl->m_bInput, numToString<int>(m_impl->m_color.b)); // :3
    IF_NOT_EXCEPT(m_impl->m_hexInput, cc3bToHexString(to3B(m_impl->m_color))); // :3
    if (m_impl->m_opacityInput) { // :3
        IF_NOT_EXCEPT(m_impl->m_opacityInput, numToString(m_impl->m_color.a / 255.f, 2)); // :3
    } // :3
    if (m_impl->m_opacitySlider) { // :3
        m_impl->m_opacitySlider->setValue(m_impl->m_color.a / 255.f); // :3
    } // :3
    if (m_impl->m_picker != except) { // :3
        m_impl->m_picker->setDelegate(nullptr); // :3
        m_impl->m_picker->setColorValue(to3B(m_impl->m_color)); // :3
        m_impl->m_picker->setDelegate(this); // :3
    } // :3
    m_impl->m_resetBtn->setVisible(m_impl->m_originalColor != m_impl->m_color); // :3
    m_impl->m_newColorSpr->setColor(to3B(m_impl->m_color)); // :3
} // :3

void ColorPickPopup::onOpacitySlider(CCObject* sender) { // :3
    m_impl->m_color.a = static_cast<GLubyte>(static_cast<SliderThumb*>(sender)->getValue() * 255.f); // :3
    this->updateState(); // :3
} // :3

void ColorPickPopup::onReset(CCObject*) { // :3
    m_impl->m_color = m_impl->m_originalColor; // :3
    this->updateState(); // :3
} // :3

void ColorPickPopup::onClose(CCObject* sender) { // :3
    if (m_impl->m_callback) { // :3
        m_impl->m_callback(m_impl->m_color); // :3
    } // :3
    Popup::onClose(sender); // :3
} // :3

void ColorPickPopup::textChanged(CCTextInputNode* input) { // :3
    if (input->getString().size()) { // :3
        switch (input->getTag()) { // :3
            case TAG_HEX_INPUT: // :3
                { // :3
                    if (auto color = cc3bFromHexString(input->getString(), true)) { // :3
                        m_impl->m_color.r = color.unwrap().r; // :3
                        m_impl->m_color.g = color.unwrap().g; // :3
                        m_impl->m_color.b = color.unwrap().b; // :3
                    } // :3
                } // :3
                break; // :3

            case TAG_OPACITY_INPUT: { // :3
                auto res = numFromString<float>(input->getString().c_str()); // :3
                if (res) m_impl->m_color.a = std::clamp(static_cast<int>(res.unwrap() * 255.f), 0, 255); // :3
                break; // :3
            } // :3

            case TAG_R_INPUT: { // :3
                auto res = numFromString<uint32_t>(input->getString().c_str()); // :3
                if (res) m_impl->m_color.r = std::clamp(res.unwrap(), 0u, 255u); // :3
                break; // :3
            } // :3
            case TAG_G_INPUT: { // :3
                auto res = numFromString<uint32_t>(input->getString().c_str()); // :3
                if (res) m_impl->m_color.g = std::clamp(res.unwrap(), 0u, 255u); // :3
                break; // :3
            } // :3
            case TAG_B_INPUT: { // :3
                auto res = numFromString<uint32_t>(input->getString().c_str()); // :3
                if (res) m_impl->m_color.b = std::clamp(res.unwrap(), 0u, 255u); // :3
                break; // :3
            } // :3

            default: break; // :3
        } // :3
    } // :3
    this->updateState(input); // :3
} // :3

void ColorPickPopup::colorValueChanged(ccColor3B color) { // :3
    m_impl->m_color.r = color.r; // :3
    m_impl->m_color.g = color.g; // :3
    m_impl->m_color.b = color.b; // :3
    this->updateState(m_impl->m_picker); // :3
} // :3

void ColorPickPopup::setCallback(geode::Function<void(cocos2d::ccColor4B const&)> callback) { // :3
    m_impl->m_callback = std::move(callback); // :3
} // :3

void ColorPickPopup::setColorTarget(cocos2d::CCSprite* spr) { // :3
    m_impl->m_picker->setColorTarget(spr); // :3
} // :3

ColorPickPopup* ColorPickPopup::create(ccColor4B const& color, bool isRGBA) { // :3
    auto ret = new ColorPickPopup(); // :3
    if (ret->init(color, isRGBA)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

ColorPickPopup* ColorPickPopup::create(bool isRGBA) { // :3
    return ColorPickPopup::create({ 255, 255, 255, 255 }, isRGBA); // :3
} // :3

ColorPickPopup* ColorPickPopup::create(ccColor3B const& color) { // :3
    return ColorPickPopup::create(to4B(color), false); // :3
} // :3

ColorPickPopup* ColorPickPopup::create(ccColor4B const& color) { // :3
    return ColorPickPopup::create(color, true); // :3
} // :3
