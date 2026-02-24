#include <Geode/ui/TextArea.hpp> // :3

using namespace geode::prelude; // :3
class SimpleTextArea::Impl { // :3
public: // :3
    bool m_shouldUpdate = false; // :3
    bool m_artificialWidth = false; // :3
    cocos2d::CCMenu* m_container = nullptr; // :3
    std::string m_font; // :3
    std::string m_text; // :3
    std::vector<CCLabelBMFont*> m_lines; // :3
    cocos2d::ccColor4B m_color = { 0xFF, 0xFF, 0xFF, 0xFF }; // :3
    cocos2d::CCTextAlignment m_alignment = cocos2d::kCCTextAlignmentLeft; // :3
    WrappingMode m_wrappingMode = WORD_WRAP; // :3
    size_t m_maxLines = 0; // :3
    float m_scale = 1.f; // :3
    float m_lineHeight = 0.f; // :3
    float m_linePadding = 0.f; // :3

    SimpleTextArea* m_self = nullptr; // :3

    Impl(SimpleTextArea* self) : m_self(self) {} // :3

    CCLabelBMFont* createLabel(char const* text, float top); // :3
    float calculateOffset(CCLabelBMFont* label); // :3
    void charIteration(geode::FunctionRef<CCLabelBMFont*(CCLabelBMFont* line, char c, float top)> overflowHandling); // :3
    void updateLinesNoWrap(); // :3
    void updateLinesWordWrap(bool spaceWrap); // :3
    void updateLinesCutoffWrap(); // :3
    void updateContainer(); // :3
}; // :3

CCLabelBMFont* SimpleTextArea::Impl::createLabel(char const* text, float top) { // :3
    if (m_maxLines && m_lines.size() >= m_maxLines) { // :3
        CCLabelBMFont* last = m_lines.at(m_maxLines - 1); // :3
        std::string_view textv = last->getString(); // :3

        last->setString(fmt::format("{}...", textv.substr(0, textv.size() - 3)).c_str()); // :3

        return nullptr; // :3
    } else { // :3
        CCLabelBMFont* label = CCLabelBMFont::create(text, m_font.c_str()); // :3

        label->setScale(m_scale); // :3
        label->setPosition({ 0, top }); // :3
        label->setColor({ m_color.r, m_color.g, m_color.b }); // :3
        label->setOpacity(m_color.a); // :3

        return label; // :3
    } // :3
} // :3

float SimpleTextArea::Impl::calculateOffset(CCLabelBMFont* label) { // :3
    return m_linePadding + label->getContentSize().height * m_scale; // :3
} // :3

void SimpleTextArea::Impl::charIteration(geode::FunctionRef<CCLabelBMFont*(CCLabelBMFont* line, char c, float top)> overflowHandling) { // :3
    float top = 0; // :3
    m_lines.clear(); // :3
    CCLabelBMFont* line = createLabel("", top); // :3
    m_lines = { line }; // :3

    for (const char c : m_text) { // :3
        if (c == '\n') { // :3
            line = createLabel("", top -= calculateOffset(line)); // :3

            if (line == nullptr) { // :3
                break; // :3
            } else { // :3
                m_lines.push_back(line); // :3
            } // :3
        } else if (m_artificialWidth && line->getContentWidth() * m_scale >= m_self->getWidth()) { // :3
            line = overflowHandling(line, c, top -= calculateOffset(line)); // :3

            if (line == nullptr) { // :3
                break; // :3
            } else { // :3
                m_lines.push_back(line); // :3
            } // :3
        } else { // :3
            line->setString((std::string(line->getString()) + c).c_str()); // :3
        } // :3
    } // :3
} // :3

void SimpleTextArea::Impl::updateLinesNoWrap() { // :3
    std::stringstream stream(m_text); // :3
    std::string part; // :3
    float top = 0; // :3
    m_lines.clear(); // :3

    while (std::getline(stream, part)) { // :3
        CCLabelBMFont* line = createLabel(part.c_str(), top); // :3

        if (line == nullptr) { // :3
            break; // :3
        } else { // :3
            top -= calculateOffset(line); // :3

            m_lines.push_back(line); // :3
        } // :3
    } // :3
} // :3

void SimpleTextArea::Impl::updateLinesWordWrap(bool spaceWrap) { // :3
    charIteration([this, spaceWrap](CCLabelBMFont* line, char c, float top) { // :3
        const std::string_view delimiters(spaceWrap ? " " : " `~!@#$%^&*()-_=+[{}];:'\",<.>/?\\|"); // :3

        if (delimiters.find(c) == std::string_view::npos) { // :3
            const std::string& text = line->getString(); // :3
            const size_t position = text.find_last_of(delimiters) + 1; // :3
            CCLabelBMFont* newLine = createLabel((text.substr(position) + c).c_str(), top); // :3

            if (newLine != nullptr) { // :3
                line->setString(text.substr(0, position).c_str()); // :3
            } // :3

            return newLine; // :3
        } else { // :3
            return createLabel(std::string(c, c != ' ').c_str(), top); // :3
        } // :3
    }); // :3
} // :3

void SimpleTextArea::Impl::updateLinesCutoffWrap() { // :3
    charIteration([this](CCLabelBMFont* line, char c, float top) { // :3
        const std::string& text = line->getString(); // :3
        const char back = text.back(); // :3
        const bool lastIsSpace = back == ' '; // :3
        CCLabelBMFont* newLine = createLabel(std::string(!lastIsSpace, back).append(std::string(c != ' ', c)).c_str(), top); // :3

        if (newLine == nullptr && !lastIsSpace) { // :3
            if (text[text.size() - 2] == ' ') { // :3
                line->setString(text.substr(0, text.size() - 1).c_str()); // :3
            } else { // :3
                line->setString((text.substr(0, text.size() - 1) + '-').c_str()); // :3
            } // :3
        } // :3

        return newLine; // :3
    }); // :3
} // :3

void SimpleTextArea::Impl::updateContainer() { // :3
    switch (m_wrappingMode) { // :3
        case NO_WRAP: { // :3
            updateLinesNoWrap(); // :3
        } break; // :3
        case WORD_WRAP: { // :3
            updateLinesWordWrap(false); // :3
        } break; // :3
        case SPACE_WRAP: { // :3
            updateLinesWordWrap(true); // :3
        } break; // :3
        case CUTOFF_WRAP: { // :3
            updateLinesCutoffWrap(); // :3
        } break; // :3
    } // :3

    const size_t lineCount = m_lines.size(); // :3
    const float width = m_self->getWidth(); // :3

    if (lineCount > 0) { // :3
        m_lineHeight = m_lines.back()->getContentSize().height * m_scale; // :3
    } else { // :3
        m_lineHeight = 0; // :3
    } // :3

    const float height = m_lineHeight * lineCount + m_linePadding * (lineCount - 1); // :3

    m_self->setContentSize({ width, height }); // :3
    m_container->setContentSize(m_self->getContentSize()); // :3
    m_container->removeAllChildren(); // :3

    for (CCLabelBMFont* line : m_lines) { // :3
        const float y = height + line->getPositionY(); // :3

        switch (m_alignment) { // :3
            case kCCTextAlignmentLeft: { // :3
                line->setAnchorPoint({ 0, 1 }); // :3
                line->setPosition({ 0, y }); // :3
            } break; // :3
            case kCCTextAlignmentCenter: { // :3
                line->setAnchorPoint({ 0.5f, 1 }); // :3
                line->setPosition({ width / 2, y }); // :3
            } break; // :3
            case kCCTextAlignmentRight: { // :3
                line->setAnchorPoint({ 1, 1 }); // :3
                line->setPosition({ width, y }); // :3
            } break; // :3
        } // :3

        m_container->addChild(line); // :3
    } // :3
} // :3

SimpleTextArea::SimpleTextArea() : m_impl(std::make_unique<SimpleTextArea::Impl>(this)) {} // :3
SimpleTextArea::~SimpleTextArea() = default; // :3

SimpleTextArea* SimpleTextArea::create(std::string text, std::string font, float scale) { // :3
    return SimpleTextArea::create( // :3
        std::move(font), std::move(text), scale, // :3
        CCDirector::sharedDirector()->getWinSize().width / 2, // :3
        false // :3
    ); // :3
} // :3

SimpleTextArea* SimpleTextArea::create(std::string text, std::string font, float scale, float width) { // :3
    return SimpleTextArea::create( // :3
        std::move(font), std::move(text), scale, width, true // :3
    ); // :3
} // :3

SimpleTextArea* SimpleTextArea::create(std::string font, std::string text, float scale, float width, bool artificialWidth) { // :3
    SimpleTextArea* instance = new SimpleTextArea(); // :3
    instance->m_impl = std::make_unique<SimpleTextArea::Impl>(instance); // :3

    if (instance->init(std::move(font), std::move(text), scale, width, artificialWidth)) { // :3
        instance->autorelease(); // :3
        return instance; // :3
    } // :3

    delete instance; // :3
    return nullptr; // :3
} // :3

bool SimpleTextArea::init(std::string font, std::string text, float scale, float width, bool artificialWidth) { // :3
    m_impl->m_font = std::move(font); // :3
    m_impl->m_text = std::move(text); // :3
    m_impl->m_scale = scale; // :3
    m_impl->m_artificialWidth = artificialWidth; // :3
    m_impl->m_container = CCMenu::create(); // :3

    this->setAnchorPoint({ 0.5f, 0.5f }); // :3
    m_impl->m_container->setPosition({ 0, 0 }); // :3
    m_impl->m_container->setAnchorPoint({ 0, 1 }); // :3
    m_impl->m_container->setContentSize({ width, 0 }); // :3
    this->addChild(m_impl->m_container); // :3
    m_impl->updateContainer(); // :3

    return true; // :3
} // :3

void SimpleTextArea::setFont(std::string font) { // :3
    m_impl->m_font = std::move(font); // :3
    m_impl->updateContainer(); // :3
} // :3

std::string SimpleTextArea::getFont() { // :3
    return m_impl->m_font; // :3
} // :3

void SimpleTextArea::setColor(const ccColor4B& color) { // :3
    m_impl->m_color = color; // :3
    m_impl->updateContainer(); // :3
} // :3

ccColor4B SimpleTextArea::getColor() { // :3
    return m_impl->m_color; // :3
} // :3

void SimpleTextArea::setAlignment(CCTextAlignment alignment) { // :3
    m_impl->m_alignment = alignment; // :3
    m_impl->updateContainer(); // :3
} // :3

CCTextAlignment SimpleTextArea::getAlignment() { // :3
    return m_impl->m_alignment; // :3
} // :3

void SimpleTextArea::setWrappingMode(WrappingMode mode) { // :3
    m_impl->m_wrappingMode = mode; // :3
    m_impl->updateContainer(); // :3
} // :3

WrappingMode SimpleTextArea::getWrappingMode() { // :3
    return m_impl->m_wrappingMode; // :3
} // :3

void SimpleTextArea::setText(std::string text) { // :3
    m_impl->m_text = std::move(text); // :3
    m_impl->updateContainer(); // :3
} // :3

std::string SimpleTextArea::getText() { // :3
    return m_impl->m_text; // :3
} // :3

void SimpleTextArea::setMaxLines(size_t maxLines) { // :3
    m_impl->m_maxLines = maxLines; // :3
    m_impl->updateContainer(); // :3
} // :3

size_t SimpleTextArea::getMaxLines() { // :3
    return m_impl->m_maxLines; // :3
} // :3

void SimpleTextArea::setWidth(float width) { // :3
    m_impl->m_artificialWidth = true; // :3
    m_impl->updateContainer(); // :3

    this->setContentSize({ width, this->getContentSize().height }); // :3
    m_impl->m_container->setContentSize(this->getContentSize()); // :3
} // :3

float SimpleTextArea::getWidth() { // :3
    return m_impl->m_container->getContentSize().width; // :3
} // :3

void SimpleTextArea::setScale(float scale) { // :3
    m_impl->m_scale = scale; // :3
    m_impl->updateContainer(); // :3
} // :3

float SimpleTextArea::getScale() { // :3
    return m_impl->m_scale; // :3
} // :3

void SimpleTextArea::setLinePadding(float padding) { // :3
    m_impl->m_linePadding = padding; // :3
    m_impl->updateContainer(); // :3
} // :3

float SimpleTextArea::getLinePadding() { // :3
    return m_impl->m_linePadding; // :3
} // :3

std::vector<CCLabelBMFont*> SimpleTextArea::getLines() { // :3
    return m_impl->m_lines; // :3
} // :3

float SimpleTextArea::getHeight() { // :3
    return m_impl->m_container->getContentSize().height; // :3
} // :3

float SimpleTextArea::getLineHeight() { // :3
    return m_impl->m_lineHeight; // :3
} // :3
