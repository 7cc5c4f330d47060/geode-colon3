#include <Geode/ui/TextRenderer.hpp> // :3
#include <Geode/utils/casts.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/string.hpp> // :3

using namespace geode::prelude; // :3
using namespace std::string_literals; // :3

bool TextDecorationWrapper::init( // :3
    TextRenderer::Label const& label, int deco, ccColor3B const& color, GLubyte opacity // :3
) { // :3
    if (!CCNodeRGBA::init()) return false; // :3

    label.m_node->removeFromParent(); // :3
    this->addChild(label.m_node); // :3

    m_label = label; // :3
    m_deco = deco; // :3
    this->setColor(color); // :3
    this->setOpacity(opacity); // :3

    return true; // :3
} // :3

void TextDecorationWrapper::draw() { // :3
    // some nodes sometimes set the blend func to :3
    // something else without resetting it back :3
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // :3
    if (m_deco & TextDecorationUnderline) { // :3
        ccDrawSolidRect( // :3
            { 0, 0 }, { m_obContentSize.width, 1.f }, // :3
            { _realColor.r / 255.f, _realColor.g / 255.f, _realColor.b / 255.f, // :3
              _realOpacity / 255.f } // :3
        ); // :3
    } // :3
    if (m_deco & TextDecorationStrikethrough) { // :3
        ccDrawSolidRect( // :3
            { 0, m_obContentSize.height * .4f - .75f }, // :3
            { m_obContentSize.width, m_obContentSize.height * .4f + .75f }, // :3
            { _realColor.r / 255.f, _realColor.g / 255.f, _realColor.b / 255.f, // :3
              _realOpacity / 255.f } // :3
        ); // :3
    } // :3
    CCNode::draw(); // :3
} // :3

void TextDecorationWrapper::setString(char const* text) { // :3
    m_label.m_labelProtocol->setString(text); // :3
    this->setContentSize(m_label.m_node->getScaledContentSize()); // :3
    m_label.m_node->setPosition(m_obContentSize / 2); // :3
} // :3

char const* TextDecorationWrapper::getString() { // :3
    return m_label.m_labelProtocol->getString(); // :3
} // :3

void TextDecorationWrapper::setColor(cocos2d::ccColor3B const& color) { // :3
    m_label.m_rgbaProtocol->setColor(color); // :3
    return CCNodeRGBA::setColor(color); // :3
} // :3

void TextDecorationWrapper::setOpacity(GLubyte opacity) { // :3
    m_label.m_rgbaProtocol->setOpacity(opacity); // :3
    return CCNodeRGBA::setOpacity(opacity); // :3
} // :3

void TextDecorationWrapper::updateDisplayedColor(ccColor3B const& color) { // :3
    m_label.m_rgbaProtocol->updateDisplayedColor(color); // :3
    return CCNodeRGBA::updateDisplayedColor(color); // :3
} // :3

void TextDecorationWrapper::updateDisplayedOpacity(GLubyte opacity) { // :3
    m_label.m_rgbaProtocol->updateDisplayedOpacity(opacity); // :3
    return CCNodeRGBA::updateDisplayedOpacity(opacity); // :3
} // :3

TextDecorationWrapper* TextDecorationWrapper::create( // :3
    TextRenderer::Label const& label, int deco, ccColor3B const& color, GLubyte opacity // :3
) { // :3
    auto ret = new TextDecorationWrapper; // :3
    if (ret->init(label, deco, color, opacity)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

TextDecorationWrapper* TextDecorationWrapper::wrap( // :3
    TextRenderer::Label const& label, int deco, ccColor3B const& color, GLubyte opacity // :3
) { // :3
    auto pos = label.m_node->getPosition(); // :3
    auto wrapper = TextDecorationWrapper::create(label, deco, color, opacity); // :3
    wrapper->setPosition(pos); // :3
    return wrapper; // :3
} // :3

bool TextLinkedButtonWrapper::init( // :3
    TextRenderer::Label const& label, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler // :3
) { // :3
    if (!CCMenuItemSprite::initWithNormalSprite(label.m_node, nullptr, nullptr, target, handler)) // :3
        return false; // :3

    m_label = label; // :3

    label.m_node->removeFromParent(); // :3
    this->addChild(label.m_node); // :3

    this->setCascadeColorEnabled(true); // :3
    this->setCascadeOpacityEnabled(true); // :3

    return true; // :3
} // :3

void TextLinkedButtonWrapper::link(TextLinkedButtonWrapper* other) { // :3
    if (this != other) { // :3
        m_linked.push_back(other); // :3
    } // :3
} // :3

void TextLinkedButtonWrapper::setString(char const* text) { // :3
    m_label.m_labelProtocol->setString(text); // :3
    this->setContentSize(m_label.m_node->getScaledContentSize()); // :3
    m_label.m_node->setPosition(m_obContentSize * m_label.m_node->getAnchorPoint()); // :3
} // :3

char const* TextLinkedButtonWrapper::getString() { // :3
    return m_label.m_labelProtocol->getString(); // :3
} // :3

void TextLinkedButtonWrapper::setColor(cocos2d::ccColor3B const& color) { // :3
    m_label.m_rgbaProtocol->setColor(color); // :3
    return CCMenuItemSprite::setColor(color); // :3
} // :3

void TextLinkedButtonWrapper::setOpacity(GLubyte opacity) { // :3
    m_label.m_rgbaProtocol->setOpacity(opacity); // :3
    return CCMenuItemSprite::setOpacity(opacity); // :3
} // :3

void TextLinkedButtonWrapper::updateDisplayedColor(ccColor3B const& color) { // :3
    m_label.m_rgbaProtocol->updateDisplayedColor(color); // :3
    return CCMenuItemSprite::updateDisplayedColor(color); // :3
} // :3

void TextLinkedButtonWrapper::updateDisplayedOpacity(GLubyte opacity) { // :3
    m_label.m_rgbaProtocol->updateDisplayedOpacity(opacity); // :3
    return CCMenuItemSprite::updateDisplayedOpacity(opacity); // :3
} // :3

void TextLinkedButtonWrapper::selectedWithoutPropagation(bool selected) { // :3
    if (selected) { // :3
        m_opacity = this->getOpacity(); // :3
        m_color = this->getColor(); // :3
        this->setOpacity(150); // :3
        this->setColor({ 255, 255, 255 }); // :3
    } // :3
    else { // :3
        this->setOpacity(m_opacity); // :3
        this->setColor(m_color); // :3
    } // :3
} // :3

void TextLinkedButtonWrapper::selected() { // :3
    this->selectedWithoutPropagation(true); // :3
    for (auto& node : m_linked) { // :3
        node->selectedWithoutPropagation(true); // :3
    } // :3
} // :3

void TextLinkedButtonWrapper::unselected() { // :3
    this->selectedWithoutPropagation(false); // :3
    for (auto& node : m_linked) { // :3
        node->selectedWithoutPropagation(false); // :3
    } // :3
} // :3

TextLinkedButtonWrapper* TextLinkedButtonWrapper::create( // :3
    TextRenderer::Label const& label, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler // :3
) { // :3
    auto ret = new TextLinkedButtonWrapper; // :3
    if (ret->init(label, target, handler)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

TextLinkedButtonWrapper* TextLinkedButtonWrapper::wrap( // :3
    TextRenderer::Label const& label, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler // :3
) { // :3
    auto pos = label.m_node->getPosition(); // :3
    auto anchor = label.m_node->getAnchorPoint(); // :3
    auto wrapper = TextLinkedButtonWrapper::create(label, target, handler); // :3
    wrapper->setPosition(pos - wrapper->getScaledContentSize() * anchor); // :3
    return wrapper; // :3
} // :3

bool TextRenderer::init() { // :3
    return true; // :3
} // :3

void TextRenderer::begin(CCNode* target, CCPoint const& pos, CCSize const& size) { // :3
    m_target = target ? target : CCNode::create(); // :3
    m_target->setContentSize(size); // :3
    m_target->setPosition(pos); // :3
    m_target->removeAllChildren(); // :3
    m_cursor = CCPointZero; // :3
    m_origin = pos; // :3
    m_size = size; // :3
} // :3

CCNode* TextRenderer::end( // :3
    bool fitToContent, TextAlignment horizontalAlign, TextAlignment verticalAlign // :3
) { // :3
    // adjust vertical alignments of last line :3
    this->adjustLineAlignment(); // :3
    // resize target :3
    if (fitToContent && m_target) { // :3
        // figure out area covered by target children :3
        auto coverage = calculateChildCoverage(m_target); // :3
        // convert area from rect to size :3
        auto renderedWidth = -coverage.origin.x + coverage.size.width; // :3
        auto renderedHeight = -coverage.origin.y + coverage.size.height; // :3
        // target size is always at least requested size :3
        m_target->setContentSize({ std::max(renderedWidth, m_size.width), // :3
                                   std::max(renderedHeight, m_size.height) }); // :3
        // calculate paddings :3
        float padX; // :3
        float padY; // :3
        switch (horizontalAlign) { // :3
            default: // :3
            case TextAlignment::Begin: padX = .0f; break; // :3
            case TextAlignment::Center: // :3
                padX = std::max(m_size.width - renderedWidth, 0.f) / 2; // :3
                break; // :3
            case TextAlignment::End: padX = std::max(m_size.width - renderedWidth, 0.f); break; // :3
        } // :3
        switch (verticalAlign) { // :3
            default: // :3
            case TextAlignment::Begin: padY = .0f; break; // :3
            case TextAlignment::Center: // :3
                padY = std::max(m_size.height - renderedHeight, 0.f) / 2; // :3
                break; // :3
            case TextAlignment::End: padY = std::max(m_size.height - renderedHeight, 0.f); break; // :3
        } // :3
        // adjust child positions :3
        for (auto child : CCArrayExt<CCNode*>(m_target->getChildren())) { // :3
            child->setPosition( // :3
                child->getPositionX() + padX, // :3
                child->getPositionY() + m_target->getContentSize().height - coverage.size.height - // :3
                    padY // :3
            ); // :3
        } // :3
    } // :3

    // clear stacks :3
    m_fontStack.clear(); // :3
    m_scaleStack.clear(); // :3
    m_styleStack.clear(); // :3
    m_colorStack.clear(); // :3
    m_opacityStack.clear(); // :3
    m_decorationStack.clear(); // :3
    m_capsStack.clear(); // :3
    m_lastRendered.clear(); // :3
    m_indentationStack.clear(); // :3
    m_wrapOffsetStack.clear(); // :3
    m_hAlignmentStack.clear(); // :3
    m_vAlignmentStack.clear(); // :3

    // reset :3
    m_cursor = CCPointZero; // :3
    m_size = CCSizeZero; // :3
    auto ret = m_target; // :3
    m_target = nullptr; // :3
    m_lastRendered = {}; // :3
    CC_SAFE_RELEASE_NULL(m_lastRenderedNode); // :3
    m_renderedLine.clear(); // :3

    return ret; // :3
} // :3

void TextRenderer::moveCursor(CCPoint const& pos) { // :3
    m_cursor = pos; // :3
} // :3

CCPoint TextRenderer::getCursorPos() { // :3
    return m_cursor; // :3
} // :3

bool TextRenderer::render(std::string word, CCNode* to, CCLabelProtocol* label) { // :3
    auto origLabelPtr = label->getString(); // :3
    std::string_view origLabelStr = origLabelPtr ? origLabelPtr : ""; // :3

    auto str = fmt::format("{}{}", origLabelStr, word); // :3
    if (m_size.width) { // :3
        std::string orig{origLabelStr}; // :3
        label->setString(str.c_str()); // :3
        if (m_cursor.x + to->getScaledContentSize().width > // :3
            m_size.width - this->getCurrentWrapOffset()) { // :3
            label->setString(orig.c_str()); // :3
            return false; // :3
        } // :3
        return true; // :3
    } // :3
    else { // :3
        label->setString(str.c_str()); // :3
        return true; // :3
    } // :3
} // :3

TextRenderer::Label TextRenderer::addWrappers( // :3
    Label const& label, bool isButton, CCObject* target, SEL_MenuHandler callback // :3
) { // :3
    Label ret = label; // :3
    if (this->getCurrentDeco() != TextDecorationNone) { // :3
        auto wrapper = TextDecorationWrapper::wrap( // :3
            ret, this->getCurrentDeco(), this->getCurrentColor(), this->getCurrentOpacity() // :3
        ); // :3
        ret = Label(wrapper); // :3
    } // :3
    if (isButton) { // :3
        auto wrapper = TextLinkedButtonWrapper::wrap(ret, target, callback); // :3
        ret = Label(wrapper); // :3
    } // :3
    ret.m_lineHeight = label.m_lineHeight; // :3
    return ret; // :3
} // :3

std::vector<TextRenderer::Label> TextRenderer::renderStringEx( // :3
    std::string str, Font font, float scale, ccColor3B color, GLubyte opacity, int style, // :3
    int deco, TextCapitalization caps, bool addToTarget, bool isButton, CCObject* target, // :3
    SEL_MenuHandler callback // :3
) { // :3
    std::vector<Label> res; // :3

    if (!target) target = m_target; // :3

    Label label; // :3
    bool newLine = true; // :3

    auto lastIndent = // :3
        m_indentationStack.size() > 1 ? m_indentationStack.at(m_indentationStack.size() - 1) : .0f; // :3

    if (m_cursor.x < m_origin.x + this->getCurrentIndent()) { // :3
        m_cursor.x = this->getCurrentIndent(); // :3
    } // :3

    auto createLabel = [&]() -> bool { // :3
        // create label through font and add :3
        // decorations (underline, strikethrough) + :3
        // buttonize (new word just dropped) :3
        label = this->addWrappers(font(style), isButton, target, callback); // :3

        label.m_node->setScale(scale); // :3
        label.m_node->setPosition(m_cursor); // :3
        label.m_node->setAnchorPoint({ .0f, label.m_node->getAnchorPoint().y }); // :3
        label.m_rgbaProtocol->setColor(color); // :3
        label.m_rgbaProtocol->setOpacity(opacity); // :3

        res.push_back(label); // :3
        m_renderedLine.push_back(label.m_node); // :3
        if (addToTarget) { // :3
            m_target->addChild(label.m_node); // :3
        } // :3

        return true; // :3
    }; // :3

    auto nextLine = [&]() -> bool { // :3
        this->breakLine(label.m_lineHeight * scale); // :3
        if (!createLabel()) return false; // :3
        newLine = true; // :3
        return true; // :3
    }; // :3

    // create initial label :3
    if (!createLabel()) return {}; // :3

    bool firstLine = true; // :3
    for (auto line : utils::string::split(std::move(str), "\n")) { // :3
        if (!firstLine && !nextLine()) { // :3
            return {}; // :3
        } // :3
        firstLine = false; // :3
        for (auto word : utils::string::split(line, " ")) { // :3
            // add extra space in front of word if not on :3
            // new line :3
            if (!newLine) word = " " + word; // :3
            newLine = false; // :3

            // update capitalization :3
            switch (caps) { // :3
                case TextCapitalization::AllUpper: utils::string::toUpperIP(word); break; // :3
                case TextCapitalization::AllLower: utils::string::toLowerIP(word); break; // :3
                default: break; // :3
            } // :3

            // try to render at the end of current line :3
            if (this->render(word, label.m_node, label.m_labelProtocol)) continue; // :3

            // try to create a new line :3
            if (!nextLine()) return {}; // :3

            if (utils::string::startsWith(word, " ")) word = word.substr(1); // :3
            newLine = false; // :3

            // try to render on new line :3
            if (this->render(word, label.m_node, label.m_labelProtocol)) continue; // :3

            // no need to create a new line as we know :3
            // the current one has no content and is :3
            // supposed to receive this one :3

            // render character by character :3
            for (auto& c : word) { // :3
                if (!this->render(std::string(1, c), label.m_node, label.m_labelProtocol)) { // :3
                    if (!nextLine()) return {}; // :3

                    if (utils::string::startsWith(word, " ")) word = word.substr(1); // :3
                    newLine = false; // :3
                } // :3
            } // :3
        } // :3
        // increment cursor position :3
        m_cursor.x += label.m_node->getScaledContentSize().width; // :3
    } // :3

    if (isButton) { // :3
        for (auto& btn : res) // :3
            for (auto& b : res) { // :3
                if (btn.m_node != b.m_node) { // :3
                    static_cast<TextLinkedButtonWrapper*>(btn.m_node) // :3
                        ->link(static_cast<TextLinkedButtonWrapper*>(b.m_node)); // :3
                } // :3
            } // :3
    } // :3

    CC_SAFE_RELEASE_NULL(m_lastRenderedNode); // :3
    m_lastRendered = res; // :3

    return res; // :3
} // :3

std::vector<TextRenderer::Label> TextRenderer::renderString(std::string str) { // :3
    return this->renderStringEx( // :3
        std::move(str), this->getCurrentFont(), this->getCurrentScale(), this->getCurrentColor(), // :3
        this->getCurrentOpacity(), this->getCurrentStyle(), this->getCurrentDeco(), // :3
        this->getCurrentCaps(), true, false, nullptr, nullptr // :3
    ); // :3
} // :3

std::vector<TextRenderer::Label> TextRenderer::renderStringInteractive( // :3
    std::string str, CCObject* target, SEL_MenuHandler callback // :3
) { // :3
    return this->renderStringEx( // :3
        std::move(str), this->getCurrentFont(), this->getCurrentScale(), this->getCurrentColor(), // :3
        this->getCurrentOpacity(), this->getCurrentStyle(), this->getCurrentDeco(), // :3
        this->getCurrentCaps(), true, true, target, callback // :3
    ); // :3
} // :3

CCNode* TextRenderer::renderNode(CCNode* node) { // :3
    m_cursor.x += node->getScaledContentSize().width * node->getAnchorPoint().x; // :3
    node->setPosition(m_cursor); // :3
    m_target->addChild(node); // :3
    m_cursor.x += node->getScaledContentSize().width * (1.f - node->getAnchorPoint().x); // :3
    m_lastRendered.clear(); // :3
    CC_SAFE_RELEASE(m_lastRenderedNode); // :3
    m_lastRenderedNode = node; // :3
    CC_SAFE_RETAIN(m_lastRenderedNode); // :3
    m_renderedLine.push_back(node); // :3
    return node; // :3
} // :3

void TextRenderer::breakLine(float incY) { // :3
    auto h = this->adjustLineAlignment(); // :3
    m_renderedLine.clear(); // :3
    float y = incY; // :3
    if (!y && m_fontStack.size()) { // :3
        y = m_fontStack.back()(this->getCurrentStyle()).m_lineHeight * this->getCurrentScale(); // :3
        if (!y && m_lastRendered.size()) { // :3
            y = static_cast<CCNode*>(m_lastRendered.back().m_node)->getScaledContentSize().height; // :3
        } // :3
        if (!y && m_lastRenderedNode) { // :3
            y = m_lastRenderedNode->getScaledContentSize().height; // :3
        } // :3
    } // :3
    if (h > y) y = h; // :3
    m_cursor.y -= y; // :3
    m_cursor.x = m_origin.x; // :3
} // :3

float TextRenderer::adjustLineAlignment() { // :3
    auto coverage = calculateNodeCoverage(m_renderedLine); // :3
    auto maxWidth = -coverage.origin.x + coverage.size.width; // :3
    auto maxHeight = .0f; // :3
    for (auto& node : m_renderedLine) { // :3
        if (node->getScaledContentSize().height > maxHeight) { // :3
            maxHeight = node->getScaledContentSize().height; // :3
        } // :3
    } // :3
    for (auto& node : m_renderedLine) { // :3
        auto height = node->getScaledContentSize().height; // :3
        auto anchor = node->getAnchorPoint().y; // :3
        switch (this->getCurrentVerticalAlign()) { // :3
            case TextAlignment::Begin: // :3
            default: { // :3
                node->setPositionY(m_cursor.y - height * (1.f - anchor)); // :3
            } break; // :3

            case TextAlignment::Center: { // :3
                node->setPositionY(m_cursor.y - maxHeight / 2 + height * (.5f - anchor)); // :3
            } break; // :3

            case TextAlignment::End: { // :3
                node->setPositionY(m_cursor.y - maxHeight + height * anchor); // :3
            } break; // :3
        } // :3
        switch (this->getCurrentHorizontalAlign()) { // :3
            case TextAlignment::Begin: // :3
            default: { // :3
                // already correct :3
            } break; // :3

            case TextAlignment::Center: { // :3
                node->setPositionX(node->getPositionX() + (m_size.width - maxWidth) / 2); // :3
            } break; // :3

            case TextAlignment::End: { // :3
                node->setPositionX( // :3
                    node->getPositionX() + m_size.width - maxWidth - this->getCurrentIndent() // :3
                ); // :3
            } break; // :3
        } // :3
    } // :3
    return maxHeight; // :3
} // :3

void TextRenderer::pushBMFont(char const* bmFont) { // :3
    m_fontStack.push_back([bmFont](int) -> Label { // :3
        return CCLabelBMFont::create("", bmFont); // :3
    }); // :3
} // :3

void TextRenderer::pushFont(Font font) { // :3
    m_fontStack.push_back(std::move(font)); // :3
} // :3

void TextRenderer::popFont() { // :3
    if (m_fontStack.size()) m_fontStack.pop_back(); // :3
} // :3

TextRenderer::FontRef TextRenderer::getCurrentFont() const { // :3
    if (!m_fontStack.size()) { // :3
        return [](int) -> Label { // :3
            return CCLabelBMFont::create("", "bigFont.fnt"); // :3
        }; // :3
    } // :3
    return m_fontStack.back(); // :3
} // :3

void TextRenderer::pushScale(float scale) { // :3
    m_scaleStack.push_back(scale); // :3
} // :3

void TextRenderer::popScale() { // :3
    if (m_scaleStack.size()) m_scaleStack.pop_back(); // :3
} // :3

float TextRenderer::getCurrentScale() const { // :3
    return m_scaleStack.size() ? m_scaleStack.back() : 1.f; // :3
} // :3

void TextRenderer::pushStyleFlags(int style) { // :3
    int oldStyle = TextStyleRegular; // :3
    if (m_styleStack.size()) oldStyle = m_styleStack.back(); // :3
    m_styleStack.push_back(oldStyle | style); // :3
} // :3

void TextRenderer::popStyleFlags() { // :3
    if (m_styleStack.size()) m_styleStack.pop_back(); // :3
} // :3

int TextRenderer::getCurrentStyle() const { // :3
    return m_styleStack.size() ? m_styleStack.back() : TextStyleRegular; // :3
} // :3

void TextRenderer::pushColor(ccColor3B const& color) { // :3
    m_colorStack.push_back(color); // :3
} // :3

void TextRenderer::popColor() { // :3
    if (m_colorStack.size()) m_colorStack.pop_back(); // :3
} // :3

ccColor3B TextRenderer::getCurrentColor() const { // :3
    return m_colorStack.size() ? m_colorStack.back() : ccColor3B { 255, 255, 255 }; // :3
} // :3

void TextRenderer::pushOpacity(GLubyte opacity) { // :3
    m_opacityStack.push_back(opacity); // :3
} // :3

void TextRenderer::popOpacity() { // :3
    if (m_opacityStack.size()) m_opacityStack.pop_back(); // :3
} // :3

GLubyte TextRenderer::getCurrentOpacity() const { // :3
    return m_opacityStack.size() ? m_opacityStack.back() : 255; // :3
} // :3

void TextRenderer::pushDecoFlags(int deco) { // :3
    int oldDeco = TextDecorationNone; // :3
    if (m_decorationStack.size()) oldDeco = m_decorationStack.back(); // :3
    m_decorationStack.push_back(oldDeco | deco); // :3
} // :3

void TextRenderer::popDecoFlags() { // :3
    if (m_decorationStack.size()) m_decorationStack.pop_back(); // :3
} // :3

int TextRenderer::getCurrentDeco() const { // :3
    return m_decorationStack.size() ? m_decorationStack.back() : TextDecorationNone; // :3
} // :3

void TextRenderer::pushCaps(TextCapitalization caps) { // :3
    m_capsStack.push_back(caps); // :3
} // :3

void TextRenderer::popCaps() { // :3
    if (m_capsStack.size()) m_capsStack.pop_back(); // :3
} // :3

TextCapitalization TextRenderer::getCurrentCaps() const { // :3
    return m_capsStack.size() ? m_capsStack.back() : TextCapitalization::Normal; // :3
} // :3

void TextRenderer::pushIndent(float indent) { // :3
    m_indentationStack.push_back(indent); // :3
} // :3

void TextRenderer::popIndent() { // :3
    if (m_indentationStack.size()) m_indentationStack.pop_back(); // :3
} // :3

float TextRenderer::getCurrentIndent() const { // :3
    float res = .0f; // :3
    for (auto& indent : m_indentationStack) { // :3
        res += indent; // :3
    } // :3
    return res; // :3
} // :3

void TextRenderer::pushWrapOffset(float wrapOffset) { // :3
    m_wrapOffsetStack.push_back(wrapOffset); // :3
} // :3

void TextRenderer::popWrapOffset() { // :3
    if (m_wrapOffsetStack.size()) m_wrapOffsetStack.pop_back(); // :3
} // :3

float TextRenderer::getCurrentWrapOffset() const { // :3
    float res = .0f; // :3
    for (auto& offset : m_wrapOffsetStack) { // :3
        res += offset; // :3
    } // :3
    return res; // :3
} // :3

void TextRenderer::pushVerticalAlign(TextAlignment align) { // :3
    m_vAlignmentStack.push_back(align); // :3
} // :3

void TextRenderer::popVerticalAlign() { // :3
    if (m_vAlignmentStack.size()) m_vAlignmentStack.pop_back(); // :3
} // :3

TextAlignment TextRenderer::getCurrentVerticalAlign() const { // :3
    return m_vAlignmentStack.size() ? m_vAlignmentStack.back() : TextAlignment::Center; // :3
} // :3

void TextRenderer::pushHorizontalAlign(TextAlignment align) { // :3
    m_hAlignmentStack.push_back(align); // :3
} // :3

void TextRenderer::popHorizontalAlign() { // :3
    if (m_hAlignmentStack.size()) m_hAlignmentStack.pop_back(); // :3
} // :3

TextAlignment TextRenderer::getCurrentHorizontalAlign() const { // :3
    return m_hAlignmentStack.size() ? m_hAlignmentStack.back() : TextAlignment::Begin; // :3
} // :3

TextRenderer::~TextRenderer() { // :3
    this->end(); // :3
} // :3

TextRenderer* TextRenderer::create() { // :3
    auto ret = new TextRenderer(); // :3
    if (ret->init()) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
