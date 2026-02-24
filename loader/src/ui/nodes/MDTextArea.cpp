#include <Geode/binding/ProfilePage.hpp> // :3
#include <Geode/binding/LevelTools.hpp> // :3
#include <Geode/binding/LevelInfoLayer.hpp> // :3
#include <Geode/binding/CCContentLayer.hpp> // :3
#include <Geode/binding/GJSearchObject.hpp> // :3
#include <Geode/binding/LevelBrowserLayer.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/ui/MDTextArea.hpp> // :3
#include <Geode/ui/BreakLine.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <Geode/utils/casts.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <ui/mods/list/ModItem.hpp> // :3
#include <md4c.h> // :3
#include <charconv> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/ui/GeodeUI.hpp> // :3
#include <memory> // :3
#include <server/Server.hpp> // :3
#include <regex> // :3

using namespace geode::prelude; // :3

static constexpr float g_fontScale = .5f; // :3
static constexpr float g_paragraphPadding = 7.f; // :3
static constexpr float g_indent = 7.f; // :3
static constexpr float g_codeBlockIndent = 8.f; // :3
static constexpr ccColor3B g_linkColor = {0x7f, 0xf4, 0xf4}; // :3

class MDTextArea::Impl { // :3
public: // :3
    std::string m_text; // :3
    cocos2d::CCSize m_size; // :3
    NineSlice* m_bgSprite = nullptr; // :3
    cocos2d::CCMenu* m_content = nullptr; // :3
    CCScrollLayerExt* m_scrollLayer = nullptr; // :3
    TextRenderer* m_renderer = nullptr; // :3
    bool m_compatibilityMode = false; // :3
}; // :3

MDTextArea::MDTextArea() : m_impl(std::make_unique<Impl>()) {} // :3

auto makeMdFont() -> TextRenderer::Font { // :3
    return [](int style) -> TextRenderer::Label { // :3
        if ((style & TextStyleBold) && (style & TextStyleItalic)) { // :3
            return CCLabelBMFont::create("", "mdFontBI.fnt"_spr); // :3
        } // :3
        if ((style & TextStyleBold)) { // :3
            return CCLabelBMFont::create("", "mdFontB.fnt"_spr); // :3
        } // :3
        if ((style & TextStyleItalic)) { // :3
            return CCLabelBMFont::create("", "mdFontI.fnt"_spr); // :3
        } // :3
        return CCLabelBMFont::create("", "mdFont.fnt"_spr); // :3
    }; // :3
} // :3

auto makeMdMonoFont() -> TextRenderer::Font { // :3
    return [](int style) -> TextRenderer::Label { // :3
        return CCLabelBMFont::create("", "mdFontMono.fnt"_spr); // :3
    }; // :3
} // :3

class MDContentLayer : public CCContentLayer { // :3
protected: // :3
    CCMenu* m_content; // :3

public: // :3
    static MDContentLayer* create(CCMenu* content, float width, float height) { // :3
        auto ret = new MDContentLayer(); // :3
        if (ret->initWithColor({ 0, 255, 0, 0 }, width, height)) { // :3
            ret->m_content = content; // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3

    void setPosition(CCPoint const& pos) override { // :3
        // cringe CCContentLayer expect its children to :3
        // all be TableViewCells :3
        CCLayerColor::setPosition(pos); // :3

        // so that's why based MDContentLayer expects itself :3
        // to have a CCMenu :-) :3
        if (m_content) { // :3
            for (auto child : CCArrayExt<CCNode*>(m_content->getChildren())) { // :3
                auto y = this->getPositionY() + child->getPositionY(); // :3
                child->setVisible( // :3
                    !((m_content->getContentSize().height < y) || // :3
                      (y < -child->getContentSize().height)) // :3
                ); // :3
            } // :3
        } // :3
    } // :3
}; // :3

Result<ccColor3B> colorForIdentifier(std::string tag) { // :3
    auto sv = std::string_view(tag); // :3
    if (tag.length() > 2 && tag[1] == '-') { // :3
        return cc3bFromHexString(sv.substr(2)); // :3
    } // :3
    // Support the old form of <carbitaryletters hex> :3
    else if (tag.find(' ') != std::string::npos) { // :3
        return cc3bFromHexString(string::trim(tag.substr(tag.find(' ') + 1))); // :3
    } // :3
    else { // :3
        auto colorText = sv.substr(1); // :3
        if (!colorText.size()) { // :3
            return Err("No color specified"); // :3
        } // :3
        else if (colorText.size() > 1) { // :3
            return Err("Color tag " + tag + " unexpectedly long, either do <cx> or <c hex>"); // :3
        } // :3
        else { // :3
            switch (colorText.front()) { // :3
                case 'a': return Ok(ccc3(150, 50, 255)); break; // :3
                case 'b': return Ok(ccc3(74, 82, 225)); break; // :3
                case 'c': return Ok(ccc3(255, 255, 150)); break; // :3
                case 'd': return Ok(ccc3(255, 150, 255)); break; // :3
                case 'f': return Ok(ccc3(150, 255, 255)); break; // :3
                case 'g': return Ok(ccc3(64, 227, 72)); break; // :3
                case 'j': return Ok(ccc3(50, 200, 255)); break; // :3
                case 'l': return Ok(ccc3(96, 171, 239)); break; // :3
                case 'o': return Ok(ccc3(255, 165, 75)); break; // :3
                case 'p': return Ok(ccc3(255, 0, 255)); break; // :3
                case 'r': return Ok(ccc3(255, 90, 90)); break; // :3
                case 's': return Ok(ccc3(255, 220, 65)); break; // :3
                case 'y': return Ok(ccc3(255, 255, 0)); break; // :3
                default: return Err("Unknown color {}", colorText); // :3
            } // :3
        } // :3
    } // :3
    return Err("Unknown error"); // :3
} // :3

bool MDTextArea::init(std::string str, CCSize const& size) { // :3
    if (!CCLayer::init()) return false; // :3

    this->ignoreAnchorPointForPosition(false); // :3
    this->setAnchorPoint({ .5f, .5f }); // :3

    m_impl->m_text = std::move(str); // :3
    m_impl->m_size = size - CCSize { 15.f, 0.f }; // :3
    this->setContentSize(m_impl->m_size); // :3
    m_impl->m_renderer = TextRenderer::create(); // :3
    CC_SAFE_RETAIN(m_impl->m_renderer); // :3

    m_impl->m_bgSprite = NineSlice::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }); // :3
    m_impl->m_bgSprite->setScale(.5f); // :3
    m_impl->m_bgSprite->setColor({ 0, 0, 0 }); // :3
    m_impl->m_bgSprite->setOpacity(75); // :3
    m_impl->m_bgSprite->setContentSize(size * 2); // :3
    m_impl->m_bgSprite->setPosition(m_impl->m_size / 2); // :3
    this->addChild(m_impl->m_bgSprite); // :3

    m_impl->m_scrollLayer = ScrollLayer::create({ 0, 0, m_impl->m_size.width, m_impl->m_size.height }, true); // :3

    m_impl->m_content = CCMenu::create(); // :3
    m_impl->m_content->setZOrder(2); // :3
    m_impl->m_scrollLayer->m_contentLayer->addChild(m_impl->m_content); // :3

    m_impl->m_scrollLayer->setTouchEnabled(true); // :3

    this->addChild(m_impl->m_scrollLayer); // :3

    this->updateLabel(); // :3

    return true; // :3
} // :3

MDTextArea::~MDTextArea() { // :3
    CC_SAFE_RELEASE(m_impl->m_renderer); // :3
} // :3

void MDTextArea::onLink(CCObject* pSender) { // :3
    auto href = static_cast<CCString*>(static_cast<CCNode*>(pSender)->getUserObject()); // :3
    auto layer = FLAlertLayer::create( // :3
        this, "Hold Up!", // :3
        fmt::format("Links are spooky! Are you sure you want to go to <cy>{}</c>?", href->getCString()), // :3
        "Cancel", "Yes", 360.f // :3
    ); // :3
    layer->setUserObject(href); // :3
    layer->show(); // :3
} // :3

void MDTextArea::onGDProfile(CCObject* pSender) { // :3
    auto href = static_cast<CCString*>(static_cast<CCNode*>(pSender)->getUserObject()); // :3
    auto profile = std::string(href->getCString()); // :3
    profile = profile.substr(profile.find(":") + 1); // :3
    auto res = numFromString<int>(profile); // :3
    if (res.isErr()) { // :3
        FLAlertLayer::create( // :3
            "Error", // :3
            "Invalid profile ID: <cr>" + profile + // :3
                "</c>. This is " // :3
                "probably the mod developer's fault, report the bug to them.", // :3
            "OK" // :3
        )->show(); // :3
        return; // :3
    } // :3
    ProfilePage::create(res.unwrap(), false)->show(); // :3
} // :3

void MDTextArea::onGDLevel(CCObject* pSender) { // :3
    auto href = static_cast<CCString*>(static_cast<CCNode*>(pSender)->getUserObject()); // :3
    auto level = std::string(href->getCString()); // :3
    level = level.substr(level.find(":") + 1); // :3
    auto res = numFromString<int>(level); // :3
    if (res.isErr()) { // :3
        FLAlertLayer::create( // :3
            "Error", // :3
            "Invalid level ID: <cr>" + level + // :3
                "</c>. This is " // :3
                "probably the mod developers's fault, report the bug to them.", // :3
            "OK" // :3
        )->show(); // :3
        return; // :3
    } // :3
    auto searchObject = GJSearchObject::create(SearchType::Type19, fmt::format("{}&gameVersion=22", res.unwrap())); // :3
    auto scene = LevelBrowserLayer::scene(searchObject); // :3
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene)); // :3
} // :3

void MDTextArea::onGeodeMod(CCObject* sender) { // :3
    auto href = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject()); // :3
    std::string_view modID = href->getCString(); // :3
    modID.remove_prefix(modID.find(':') + 1); // :3
    (void)openInfoPopup(std::string{modID}); // :3
} // :3

void MDTextArea::FLAlert_Clicked(FLAlertLayer* layer, bool btn) { // :3
    if (btn) { // :3
        web::openLinkInBrowser(static_cast<CCString*>(layer->getUserObject())->getCString()); // :3
    } // :3
} // :3

struct MDParser { // :3
    static std::string s_lastLink; // :3
    static bool s_lastLinkWasAuto; // :3
    static std::string s_lastImage; // :3
    static bool s_isOrderedList; // :3
    static bool s_isCodeBlock; // :3
    static float s_codeStart; // :3
    static size_t s_orderedListNum; // :3
    static std::vector<TextRenderer::Label> s_codeSpans; // :3
    static bool s_breakListLine; // :3

    static int parseText(MD_TEXTTYPE type, MD_CHAR const* rawText, MD_SIZE size, void* mdtextarea) { // :3
        auto textarea = static_cast<MDTextArea*>(mdtextarea); // :3
        auto renderer = textarea->m_impl->m_renderer; // :3
        auto compatibilityMode = textarea->m_impl->m_compatibilityMode; // :3

        auto text = std::string(rawText, size); // :3
        switch (type) { // :3
            case MD_TEXTTYPE::MD_TEXT_CODE: // :3
                { // :3
                    auto rendered = renderer->renderString(text); // :3
                    if (!s_isCodeBlock) { // :3
                        // code span BGs need to be rendered after all :3
                        // rendering is done since the position of the :3
                        // rendered labels may change after alignments :3
                        // are adjusted :3
                        ranges::push(s_codeSpans, rendered); // :3
                    } // :3
                } // :3
                break; // :3

            case MD_TEXTTYPE::MD_TEXT_BR: // :3
                { // :3
                    renderer->breakLine(); // :3
                } // :3
                break; // :3

            case MD_TEXTTYPE::MD_TEXT_SOFTBR: // :3
                { // :3
                    renderer->renderString(" "); // :3
                } // :3
                break; // :3

            case MD_TEXTTYPE::MD_TEXT_NORMAL: // :3
                { // :3
                    if (!s_lastLink.empty()) { // :3
                        // Render `<mod:mod.id>` as a `ModItem` :3
                        if (s_lastLinkWasAuto && s_lastLink.starts_with("mod:")) { // :3
                            auto item = AnyModItem::create(s_lastLink.substr(s_lastLink.find(':') + 1)); // :3
                            item->updateDisplay(textarea->m_impl->m_size.width, ModListDisplay::SmallList); // :3
                            renderer->renderNode(item); // :3
                        } // :3
                        else { // :3
                            renderer->pushDecoFlags(TextDecorationUnderline); // force underline for links :3
                            auto rendered = renderer->renderStringInteractive( // :3
                                text, textarea, // :3
                                utils::string::startsWith(s_lastLink, "user:") // :3
                                    ? menu_selector(MDTextArea::onGDProfile) // :3
                                    : utils::string::startsWith(s_lastLink, "level:") // :3
                                        ? menu_selector(MDTextArea::onGDLevel) // :3
                                        : utils::string::startsWith(s_lastLink, "mod:") // :3
                                            ? menu_selector(MDTextArea::onGeodeMod) // :3
                                            : menu_selector(MDTextArea::onLink) // :3
                            ); // :3
                            for (auto const& label : rendered) { // :3
                                label.m_node->setUserObject(CCString::create(s_lastLink)); // :3
                            } // :3
                            renderer->popDecoFlags(); // :3
                        } // :3
                    } // :3
                    else if (!s_lastImage.empty()) { // :3
                        bool isFrame = false; // :3

                        const auto splitOnce = [](const std::string& str, char delim) -> std::pair<std::string, std::string> { // :3
                            const auto pos = str.find(delim); // :3
                            if (pos == std::string::npos) { // :3
                                return { str, {} }; // :3
                            } // :3
                            return { str.substr(0, pos), str.substr(pos + 1) }; // :3
                        }; // :3

                        // key value pair of arguments :3
                        std::vector<std::pair<std::string, std::string>> imgArguments; // :3
                        auto split = splitOnce(s_lastImage, '?'); // :3
                        s_lastImage = split.first; // :3

                        imgArguments = ranges::map<decltype(imgArguments)>(utils::string::split(split.second, "&"), [&](auto str) { // :3
                            return splitOnce(str, '='); // :3
                        }); // :3

                        float spriteScale = 1.0f; // :3
                        float spriteWidth = 0.0f; // :3
                        float spriteHeight = 0.0f; // :3

                        for (auto [key, value] : imgArguments) { // :3
                            if (key == "scale") { // :3
                                auto scaleRes = utils::numFromString<float>(value); // :3
                                if (scaleRes) { // :3
                                    spriteScale = scaleRes.unwrap(); // :3
                                } // :3
                            } // :3
                            else if (key == "width") { // :3
                                auto widthRes = utils::numFromString<float>(value); // :3
                                if (widthRes) { // :3
                                    spriteWidth = widthRes.unwrap(); // :3
                                } // :3
                            } // :3
                            else if (key == "height") { // :3
                                auto heightRes = utils::numFromString<float>(value); // :3
                                if (heightRes) { // :3
                                    spriteHeight = heightRes.unwrap(); // :3
                                } // :3
                            } // :3
                        } // :3

                        if (utils::string::startsWith(s_lastImage, "frame:")) { // :3
                            s_lastImage = s_lastImage.substr(s_lastImage.find(":") + 1); // :3
                            isFrame = true; // :3
                        } // :3
                        CCSprite* spr = nullptr; // :3
                        if (isFrame) { // :3
                            spr = CCSprite::createWithSpriteFrameName(s_lastImage.c_str()); // :3
                        } // :3
                        else { // :3
                            spr = CCSprite::create(s_lastImage.c_str()); // :3
                        } // :3
                        if (spr && spr->getUserObject("geode.texture-loader/fallback") == nullptr) { // :3
                            spr->setScale(spriteScale); // :3
                            if (spriteWidth > 0.0f && spriteHeight <= 0.0f) { // :3
                                limitNodeWidth(spr, spriteWidth, 999.f, .1f); // :3
                            } // :3
                            else if (spriteHeight > 0.0f && spriteWidth <= 0.0f) { // :3
                                limitNodeHeight(spr, spriteHeight, 999.f, .1f); // :3
                            } // :3
                            else if (spriteWidth > 0.0f && spriteHeight > 0.0f) { // :3
                                limitNodeSize(spr, { spriteWidth, spriteHeight }, 999.f, .1f); // :3
                            } // :3
                            renderer->renderNode(spr); // :3
                        } // :3
                        else { // :3
                            renderer->renderString(text); // :3
                        } // :3
                        s_lastImage = ""; // :3
                    } // :3
                    else { // :3
                        renderer->renderString(text); // :3
                    } // :3
                } // :3
                break; // :3

            case MD_TEXTTYPE::MD_TEXT_HTML: // :3
                { // :3
                    if (text.size() > 2) { // :3
                        auto tag = utils::string::trim(text.substr(1, text.size() - 2)); // :3
                        auto isClosing = tag.front() == '/'; // :3
                        if (isClosing) tag.erase(tag.begin()); // :3

                        if (tag.front() != 'c') { // :3
                            log::warn("Unknown tag {}", text); // :3
                            renderer->renderString(text); // :3
                        } // :3
                        else { // :3
                            if (isClosing) { // :3
                                renderer->popColor(); // :3
                            } // :3
                            else { // :3
                                auto color = colorForIdentifier(std::move(tag)); // :3
                                if (color) { // :3
                                    renderer->pushColor(color.unwrap()); // :3
                                } // :3
                                else if (compatibilityMode) { // :3
                                    renderer->pushColor(ccc3(255, 0, 0)); // :3
                                } // :3
                                else { // :3
                                    log::warn("Error parsing color: {}", color.unwrapErr()); // :3
                                } // :3
                            } // :3
                        } // :3
                    } // :3
                    else { // :3
                        log::warn("Too short tag {}", text); // :3
                        renderer->renderString(text); // :3
                    } // :3
                } // :3
                break; // :3

            default: // :3
                { // :3
                    log::warn("Unhandled text type {}", static_cast<int>(type)); // :3
                } // :3
                break; // :3
        } // :3
        return 0; // :3
    } // :3

    static int enterBlock(MD_BLOCKTYPE type, void* detail, void* mdtextarea) { // :3
        auto textarea = static_cast<MDTextArea*>(mdtextarea); // :3
        auto renderer = textarea->m_impl->m_renderer; // :3
        switch (type) { // :3
            case MD_BLOCKTYPE::MD_BLOCK_DOC: // :3
                { // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_H: // :3
                { // :3
                    auto hdetail = static_cast<MD_BLOCK_H_DETAIL*>(detail); // :3
                    renderer->pushStyleFlags(TextStyleBold); // :3
                    switch (hdetail->level) { // :3
                        case 1: renderer->pushScale(g_fontScale * 2.f); break; // :3
                        case 2: renderer->pushScale(g_fontScale * 1.5f); break; // :3
                        case 3: renderer->pushScale(g_fontScale * 1.17f); break; // :3
                        case 4: renderer->pushScale(g_fontScale); break; // :3
                        case 5: renderer->pushScale(g_fontScale * .83f); break; // :3
                        default: // :3
                        case 6: renderer->pushScale(g_fontScale * .67f); break; // :3
                    } // :3
                    // switch (hdetail->level) { :3
                    //     case 3: renderer->pushCaps(TextCapitalization::AllUpper); break; :3
                    // } :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_P: // :3
                { // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_UL: // :3
            case MD_BLOCKTYPE::MD_BLOCK_OL: // :3
                { // :3
                    renderer->pushIndent(g_indent); // :3
                    s_isOrderedList = type == MD_BLOCKTYPE::MD_BLOCK_OL; // :3
                    s_orderedListNum = 0; // :3
                    if (s_breakListLine) { // :3
                        renderer->breakLine(); // :3
                        s_breakListLine = false; // :3
                    } // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_HR: // :3
                { // :3
                    renderer->breakLine(g_paragraphPadding / 2); // :3
                    renderer->renderNode(BreakLine::create(textarea->m_impl->m_size.width)); // :3
                    renderer->breakLine(g_paragraphPadding); // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_LI: // :3
                { // :3
                    if (s_breakListLine) { // :3
                        renderer->breakLine(); // :3
                        s_breakListLine = false; // :3
                    } // :3
                    renderer->pushOpacity(renderer->getCurrentOpacity() / 2); // :3
                    auto lidetail = static_cast<MD_BLOCK_LI_DETAIL*>(detail); // :3
                    if (s_isOrderedList) { // :3
                        s_orderedListNum++; // :3
                        renderer->renderString(std::to_string(s_orderedListNum) + ". "); // :3
                    } // :3
                    else { // :3
                        renderer->renderString("• "); // :3
                    } // :3
                    renderer->popOpacity(); // :3
                    s_breakListLine = true; // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_CODE: // :3
                { // :3
                    s_isCodeBlock = true; // :3
                    s_codeStart = renderer->getCursorPos().y; // :3
                    renderer->pushFont(makeMdMonoFont()); // :3
                    renderer->pushIndent(g_codeBlockIndent); // :3
                    renderer->pushWrapOffset(g_codeBlockIndent); // :3
                } // :3
                break; // :3

            default: // :3
                { // :3
                    log::warn("Unhandled block enter type {}", static_cast<int>(type)); // :3
                } // :3
                break; // :3
        } // :3
        return 0; // :3
    } // :3

    static int leaveBlock(MD_BLOCKTYPE type, void* detail, void* mdtextarea) { // :3
        auto textarea = static_cast<MDTextArea*>(mdtextarea); // :3
        auto renderer = textarea->m_impl->m_renderer; // :3
        switch (type) { // :3
            case MD_BLOCKTYPE::MD_BLOCK_DOC: // :3
                { // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_H: // :3
                { // :3
                    auto hdetail = static_cast<MD_BLOCK_H_DETAIL*>(detail); // :3
                    renderer->breakLine(); // :3
                    if (hdetail->level == 1) { // :3
                        renderer->breakLine(g_paragraphPadding / 2); // :3
                        renderer->renderNode(BreakLine::create(textarea->m_impl->m_size.width)); // :3
                    } // :3
                    renderer->breakLine(g_paragraphPadding); // :3
                    renderer->popScale(); // :3
                    renderer->popStyleFlags(); // :3
                    // switch (hdetail->level) { :3
                    //     case 3: renderer->popCaps(); break; :3
                    // } :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_P: // :3
                { // :3
                    renderer->breakLine(); // :3
                    renderer->breakLine(g_paragraphPadding); // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_OL: // :3
            case MD_BLOCKTYPE::MD_BLOCK_UL: // :3
                { // :3
                    renderer->popIndent(); // :3
                    if (s_breakListLine) { // :3
                        renderer->breakLine(); // :3
                        s_breakListLine = false; // :3
                    } // :3
                    if (renderer->getCurrentIndent() == 0) { // :3
                        renderer->breakLine(); // :3
                    } // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_CODE: // :3
                { // :3
                    auto codeEnd = renderer->getCursorPos().y; // :3

                    auto pad = g_codeBlockIndent / 1.5f; // :3

                    CCSize size { textarea->m_impl->m_size.width - renderer->getCurrentIndent() - // :3
                                      renderer->getCurrentWrapOffset() + pad * 2, // :3
                                  s_codeStart - codeEnd + pad * 2 }; // :3

                    auto bg = // :3
                        NineSlice::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }); // :3
                    bg->setScale(.25f); // :3
                    bg->setColor({ 0, 0, 0 }); // :3
                    bg->setOpacity(75); // :3
                    bg->setContentSize(size * 4); // :3
                    bg->setPosition( // :3
                        size.width / 2 + renderer->getCurrentIndent() - pad, // :3
                        // mmm i love magic numbers :3
                        // the -2.f is to offset the the box :3
                        // to fit the Ubuntu font very neatly. :3
                        // idk if it works the same for other :3
                        // fonts :3
                        s_codeStart - 2.f + pad - size.height / 2 // :3
                    ); // :3
                    bg->setAnchorPoint({ .5f, .5f }); // :3
                    bg->setZOrder(-1); // :3
                    textarea->m_impl->m_content->addChild(bg); // :3

                    renderer->popWrapOffset(); // :3
                    renderer->popIndent(); // :3
                    renderer->popFont(); // :3

                    renderer->breakLine(); // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_LI: // :3
                { // :3
                } // :3
                break; // :3

            case MD_BLOCKTYPE::MD_BLOCK_HR: // :3
                { // :3
                } // :3
                break; // :3

            default: // :3
                { // :3
                    log::warn("Unhandled block leave type {}", static_cast<int>(type)); // :3
                } // :3
                break; // :3
        } // :3
        return 0; // :3
    } // :3

    static int enterSpan(MD_SPANTYPE type, void* detail, void* mdtextarea) { // :3
        auto renderer = static_cast<MDTextArea*>(mdtextarea)->m_impl->m_renderer; // :3
        switch (type) { // :3
            case MD_SPANTYPE::MD_SPAN_STRONG: // :3
                { // :3
                    renderer->pushStyleFlags(TextStyleBold); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_EM: // :3
                { // :3
                    renderer->pushStyleFlags(TextStyleItalic); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_DEL: // :3
                { // :3
                    renderer->pushDecoFlags(TextDecorationStrikethrough); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_U: // :3
                { // :3
                    renderer->pushDecoFlags(TextDecorationUnderline); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_IMG: // :3
                { // :3
                    auto adetail = static_cast<MD_SPAN_IMG_DETAIL*>(detail); // :3
                    s_lastImage = std::string(adetail->src.text, adetail->src.size); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_A: // :3
                { // :3
                    auto adetail = static_cast<MD_SPAN_A_DETAIL*>(detail); // :3
                    s_lastLink = std::string(adetail->href.text, adetail->href.size); // :3
                    s_lastLinkWasAuto = adetail->is_autolink != 0; // :3

                    renderer->pushColor(g_linkColor); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_CODE: // :3
                { // :3
                    s_isCodeBlock = false; // :3
                    renderer->pushFont(makeMdMonoFont()); // :3
                } // :3
                break; // :3

            default: // :3
                { // :3
                    log::warn("Unhandled span enter type {}", static_cast<int>(type)); // :3
                } // :3
                break; // :3
        } // :3
        return 0; // :3
    } // :3

    static int leaveSpan(MD_SPANTYPE type, void* detail, void* mdtextarea) { // :3
        auto renderer = static_cast<MDTextArea*>(mdtextarea)->m_impl->m_renderer; // :3
        switch (type) { // :3
            case MD_SPANTYPE::MD_SPAN_STRONG: // :3
                { // :3
                    renderer->popStyleFlags(); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_EM: // :3
                { // :3
                    renderer->popStyleFlags(); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_DEL: // :3
                { // :3
                    renderer->popDecoFlags(); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_U: // :3
                { // :3
                    renderer->popDecoFlags(); // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_A: // :3
                { // :3
                    renderer->popColor(); // :3
                    s_lastLink = ""; // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_IMG: // :3
                { // :3
                    s_lastImage = ""; // :3
                } // :3
                break; // :3

            case MD_SPANTYPE::MD_SPAN_CODE: // :3
                { // :3
                    renderer->popFont(); // :3
                } // :3
                break; // :3

            default: // :3
                { // :3
                    log::warn("Unhandled span leave type {}", static_cast<int>(type)); // :3
                } // :3
                break; // :3
        } // :3
        return 0; // :3
    } // :3
}; // :3

std::string MDParser::s_lastLink = ""; // :3
bool MDParser::s_lastLinkWasAuto = false; // :3
std::string MDParser::s_lastImage = ""; // :3
bool MDParser::s_isOrderedList = false; // :3
size_t MDParser::s_orderedListNum = 0; // :3
bool MDParser::s_isCodeBlock = false; // :3
float MDParser::s_codeStart = 0; // :3
decltype(MDParser::s_codeSpans) MDParser::s_codeSpans = {}; // :3
bool MDParser::s_breakListLine = false; // :3

void MDTextArea::updateLabel() { // :3
    m_impl->m_renderer->begin(m_impl->m_content, CCPointZero, m_impl->m_size); // :3

    m_impl->m_renderer->pushFont(makeMdFont()); // :3
    m_impl->m_renderer->pushScale(.5f); // :3
    m_impl->m_renderer->pushVerticalAlign(TextAlignment::End); // :3
    m_impl->m_renderer->pushHorizontalAlign(TextAlignment::Begin); // :3

    MD_PARSER parser; // :3

    parser.abi_version = 0; // :3
    parser.flags = MD_FLAG_UNDERLINE | MD_FLAG_STRIKETHROUGH | MD_FLAG_PERMISSIVEURLAUTOLINKS | // :3
        MD_FLAG_PERMISSIVEWWWAUTOLINKS; // :3

    parser.text = &MDParser::parseText; // :3
    parser.enter_block = &MDParser::enterBlock; // :3
    parser.leave_block = &MDParser::leaveBlock; // :3
    parser.enter_span = &MDParser::enterSpan; // :3
    parser.leave_span = &MDParser::leaveSpan; // :3
    parser.debug_log = nullptr; // :3
    parser.syntax = nullptr; // :3

    MDParser::s_codeSpans = {}; // :3

    auto textContent = m_impl->m_text; // :3
    if (m_impl->m_compatibilityMode) { // :3
        textContent = MDTextArea::translateNewlines(m_impl->m_text); // :3

        // ery proofing... :3
        utils::string::replaceIP(textContent, "<c_>", "<c->"); // :3
    } // :3

    if (md_parse(textContent.c_str(), textContent.size(), &parser, this)) { // :3
        m_impl->m_renderer->renderString("Error parsing Markdown"); // :3
    } // :3

    for (auto& render : MDParser::s_codeSpans) { // :3
        auto bg = NineSlice::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }); // :3
        bg->setScale(.125f); // :3
        bg->setColor({ 0, 0, 0 }); // :3
        bg->setOpacity(75); // :3
        bg->setContentSize(render.m_node->getScaledContentSize() * 8 + CCSize { 20.f, .0f }); // :3
        bg->setPosition( // :3
            render.m_node->getPositionX() - 2.5f * (.5f - render.m_node->getAnchorPoint().x), // :3
            render.m_node->getPositionY() - .5f // :3
        ); // :3
        bg->setAnchorPoint(render.m_node->getAnchorPoint()); // :3
        bg->setZOrder(-1); // :3
        m_impl->m_content->addChild(bg); // :3
        // i know what you're thinking. :3
        // my brother in christ, what the hell is this? :3
        // where did this magical + 1.5f come from? :3
        // the reason is that if you remove them, code :3
        // spans are slightly offset and it triggers my :3
        // OCD. :3
        render.m_node->setPositionY(render.m_node->getPositionY() + 1.5f); // :3
    } // :3

    m_impl->m_renderer->end(); // :3

    if (m_impl->m_content->getContentSize().height > m_impl->m_size.height) { // :3
        // Generate bottom padding :3
        m_impl->m_scrollLayer->m_contentLayer->setContentSize(m_impl->m_content->getContentSize() + CCSize { 0.f, 12.5 }); // :3
        m_impl->m_content->setPositionY(10.f); // :3
    } else { // :3
        m_impl->m_scrollLayer->m_contentLayer->setContentSize(m_impl->m_content->getContentSize()); // :3
        m_impl->m_content->setPositionY(-2.5f); // :3
    } // :3

    m_impl->m_scrollLayer->moveToTop(); // :3
} // :3

CCScrollLayerExt* MDTextArea::getScrollLayer() const { // :3
    return m_impl->m_scrollLayer; // :3
} // :3

void MDTextArea::setString(char const* text) { // :3
    m_impl->m_text = text; // :3
    this->updateLabel(); // :3
} // :3

char const* MDTextArea::getString() { // :3
    return m_impl->m_text.c_str(); // :3
} // :3

MDTextArea* MDTextArea::create(std::string str, CCSize const& size) { // :3
    auto ret = new MDTextArea; // :3
    if (ret->init(std::move(str), size)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

MDTextArea* MDTextArea::create(std::string str, CCSize const& size, bool compatibilityMode) { // :3
    auto ret = new MDTextArea; // :3

    if (ret->init(std::move(str), size)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

std::string MDTextArea::translateNewlines(std::string const& str) { // :3
    std::regex newlineRe("(.*\\S)\n(?!\n)"); // :3
    return std::regex_replace(str, newlineRe, "$1  \n"); // :3
} // :3
