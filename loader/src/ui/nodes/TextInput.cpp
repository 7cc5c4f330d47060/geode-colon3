#include <Geode/binding/CCTextInputNode.hpp> // :3
#include <Geode/binding/TextInputDelegate.hpp> // :3
#include <Geode/modify/CCTextInputNode.hpp> // :3
#include <Geode/ui/TextInput.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

struct TextInputNodeFix : Modify<TextInputNodeFix, CCTextInputNode> { // :3
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("TextInputNode fix") // :3

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) { // :3
        if (!this->getUserObject("fix-text-input")) { // :3
            return CCTextInputNode::ccTouchBegan(touch, event); // :3
        } // :3

        if (!nodeIsVisible(this)) { // :3
            this->onClickTrackNode(false); // :3
            return false; // :3
        } // :3

        auto touchPos = touch->getLocation(); // :3
        auto const size = this->getContentSize(); // :3
        auto const pos = this->convertToNodeSpace(touchPos) + m_textField->getAnchorPoint() * size; // :3

        float parentScale = 1.f; // :3
        CCNode* currentParent = this; // :3

        while ((currentParent = currentParent->getParent())) { // :3
            parentScale *= currentParent->getScale(); // :3
        } // :3

        CCPoint nodeSpace = this->convertToNodeSpace(touchPos); // :3
        nodeSpace = nodeSpace / parentScale; // :3
        touchPos = this->convertToWorldSpace(nodeSpace); // :3

        if (pos.x < 0 || pos.x > size.width || pos.y < 0 || pos.y > size.height) { // :3
            this->onClickTrackNode(false); // :3
            return false; // :3
        } // :3
        if (m_delegate && !m_delegate->allowTextInput(this)) { // :3
            this->onClickTrackNode(false); // :3
            return false; // :3
        } // :3

        this->onClickTrackNode(true); // :3
        this->updateCursorPosition(touchPos, {{0, 0}, size}); // :3

        return true; // :3
    } // :3
}; // :3

const char* geode::getCommonFilterAllowedChars(CommonFilter filter) { // :3
    switch (filter) { // :3
        default: // :3
        case CommonFilter::Uint:         return "0123456789"; // :3
        case CommonFilter::Int:          return "-0123456789"; // :3
        case CommonFilter::Float:        return "-.0123456789"; // :3
        case CommonFilter::ID:           return "abcdefghijklmnopqrstuvwxyz0123456789-_."; // :3
        case CommonFilter::Name:         return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ "; // :3
        case CommonFilter::Any:          return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ#_-+/\\&$%^~*\'\"{}()[]<>=!?@,;.:|• "; // :3
        case CommonFilter::Hex:          return "0123456789abcdefABCDEF"; // :3
        case CommonFilter::Base64Normal: return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/="; // :3
        case CommonFilter::Base64URL:    return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_="; // :3
        case CommonFilter::Alphanumeric: return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // :3
        case CommonFilter::Alphabetic:   return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // :3
    } // :3
} // :3

class TextInput::Impl final { // :3
public: // :3
    NineSlice* bgSprite = nullptr; // :3
    CCTextInputNode* input = nullptr; // :3
    geode::Function<void(std::string const&)> onInput = nullptr; // :3
    cocos2d::CCLabelBMFont* label = nullptr; // :3
    bool callbackEnabled = true; // :3
}; // :3

TextInput::TextInput() : m_impl(std::make_unique<Impl>()) { } // :3

bool TextInput::init(float width, ZStringView placeholder, ZStringView font) { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    constexpr float HEIGHT = 30.f; // :3

    this->setContentSize({ width, HEIGHT }); // :3
    this->setAnchorPoint({ .5f, .5f }); // :3

    m_impl->bgSprite = NineSlice::create("square02b_001.png", { 0, 0, 80, 80 }); // :3
    m_impl->bgSprite->setScale(.5f); // :3
    m_impl->bgSprite->setColor({ 0, 0, 0 }); // :3
    m_impl->bgSprite->setOpacity(90); // :3
    m_impl->bgSprite->setContentSize({ width * 2, HEIGHT * 2 }); // :3
    this->addChildAtPosition(m_impl->bgSprite, Anchor::Center); // :3

    m_impl->input = CCTextInputNode::create(width - 10.f, HEIGHT, placeholder.c_str(), 24, font.c_str()); // :3
    m_impl->input->setLabelPlaceholderColor({ 150, 150, 150 }); // :3
    m_impl->input->setLabelPlaceholderScale(.5f); // :3
    m_impl->input->setMaxLabelScale(.6f); // :3
    m_impl->input->setUserObject("fix-text-input", CCBool::create(true)); // :3
    this->addChildAtPosition(m_impl->input, Anchor::Center); // :3

    return true; // :3
} // :3

TextInput* TextInput::create(float width, ZStringView placeholder, ZStringView font) { // :3
    auto ret = new TextInput(); // :3
    if (ret->init(width, placeholder, font)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void TextInput::textChanged(CCTextInputNode* input) { // :3
    if (m_impl->onInput && m_impl->callbackEnabled) { // :3
        m_impl->onInput(input->getString()); // :3
    } // :3
} // :3

void TextInput::setPlaceholder(gd::string placeholder) { // :3
    m_impl->input->m_caption = std::move(placeholder); // :3
    m_impl->input->refreshLabel(); // :3
    // This is needed to actually update the placeholder :3
    m_impl->input->updateLabel(m_impl->input->getString()); // :3
} // :3

void TextInput::setLabel(ZStringView label) { // :3
    if (label.size()) { // :3
        if (m_impl->label) { // :3
            m_impl->label->setString(label.c_str()); // :3
        } // :3
        else { // :3
            m_impl->label = CCLabelBMFont::create(label.c_str(), "goldFont.fnt"); // :3
            this->addChildAtPosition(m_impl->label, Anchor::TopLeft, ccp(3, 2), ccp(0, 0)); // :3
        } // :3
        m_impl->label->limitLabelWidth(m_impl->bgSprite->getScaledContentWidth() - 6, .4f, .1f); // :3
    } // :3
    else { // :3
        if (m_impl->label) { // :3
            m_impl->label->removeFromParent(); // :3
            m_impl->label = nullptr; // :3
        } // :3
    } // :3
} // :3
void TextInput::setFilter(gd::string allowedChars) { // :3
    m_impl->input->m_allowedChars = std::move(allowedChars); // :3
} // :3
void TextInput::setCommonFilter(CommonFilter filter) { // :3
    this->setFilter(getCommonFilterAllowedChars(filter)); // :3
} // :3
void TextInput::setMaxCharCount(size_t length) { // :3
    m_impl->input->m_maxLabelLength = length == 0 ? 9999999 : length; // :3
} // :3
void TextInput::setPasswordMode(bool enable) { // :3
    m_impl->input->m_usePasswordChar = enable; // :3
    m_impl->input->refreshLabel(); // :3
} // :3
void TextInput::setWidth(float width) { // :3
    this->setContentWidth(width); // :3
    m_impl->input->m_maxLabelWidth = width - 10.f; // :3
    m_impl->input->setContentWidth(width); // :3
    m_impl->bgSprite->setContentWidth(width * 2); // :3
    m_impl->input->setPositionX(width / 2.f); // :3
    m_impl->bgSprite->setPositionX(width / 2.f); // :3
} // :3
void TextInput::setDelegate(TextInputDelegate* delegate, std::optional<int> tag) { // :3
    m_impl->input->m_delegate = delegate; // :3
    m_impl->onInput = nullptr; // :3
    if (tag.has_value()) { // :3
        m_impl->input->setTag(tag.value()); // :3
    } // :3
} // :3
void TextInput::setCallback(geode::Function<void(std::string const&)> onInput) { // :3
    this->setDelegate(this); // :3
    m_impl->onInput = std::move(onInput); // :3
} // :3
void TextInput::setCallbackEnabled(bool enabled) { // :3
    m_impl->callbackEnabled = enabled; // :3
} // :3
void TextInput::setEnabled(bool enabled) { // :3
    m_impl->input->setTouchEnabled(enabled); // :3
    m_impl->input->m_textLabel->setOpacity(enabled ? 255 : 150); // :3
} // :3
void TextInput::setTextAlign(TextInputAlign align) { // :3
    switch (align) { // :3
        default: // :3
        case TextInputAlign::Center: { // :3
            m_impl->input->m_textField->setAnchorPoint({ .5f, .5f }); // :3
            m_impl->input->m_textLabel->setAnchorPoint({ .5f, .5f }); // :3
            m_impl->input->updateAnchoredPosition(Anchor::Center); // :3
        } break; // :3

        case TextInputAlign::Left: { // :3
            m_impl->input->m_textField->setAnchorPoint({ .0f, .5f }); // :3
            m_impl->input->m_textLabel->setAnchorPoint({ .0f, .5f }); // :3
            m_impl->input->updateAnchoredPosition(Anchor::Left, ccp(5, 0)); // :3
        } break; // :3
    } // :3
} // :3

void TextInput::hideBG() { // :3
    m_impl->bgSprite->setVisible(false); // :3
} // :3

void TextInput::setString(gd::string str, bool triggerCallback) { // :3
    auto oldDelegate = m_impl->input->m_delegate; // :3
    // Avoid triggering the callback :3
    m_impl->input->m_delegate = nullptr; // :3
    m_impl->input->setString(std::move(str)); // :3
    m_impl->input->m_delegate = oldDelegate; // :3
    if (triggerCallback && m_impl->input->m_delegate) { // :3
        m_impl->input->m_delegate->textChanged(m_impl->input); // :3
    } // :3
} // :3

gd::string TextInput::getString() const { // :3
    return m_impl->input->getString(); // :3
} // :3
bool TextInput::isCallbackEnabled() const { // :3
    return m_impl->callbackEnabled; // :3
} // :3

void TextInput::focus() { // :3
    m_impl->input->onClickTrackNode(true); // :3
} // :3
void TextInput::defocus() { // :3
    m_impl->input->detachWithIME(); // :3
} // :3

CCTextInputNode* TextInput::getInputNode() const { // :3
    return m_impl->input; // :3
} // :3
NineSlice* TextInput::getBGSprite() const { // :3
    return m_impl->bgSprite; // :3
} // :3

TextInput::~TextInput() { // :3
    if (m_impl->input) { // :3
        m_impl->input->onClickTrackNode(false); // :3
        m_impl->input->m_delegate = nullptr; // :3
    } // :3
} // :3
