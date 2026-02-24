#include <Geode/modify/LoadingLayer.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <matjson.hpp> // :3
#include <Geode/binding/CCTextInputNode.hpp> // :3
#include <Geode/binding/GameManager.hpp> // :3

#ifdef GEODE_IS_WINDOWS // :3
#else // :3
# include <unordered_map> // :3
# include <typeindex> // :3
# include <cxxabi.h> // :3
#endif // :3

using namespace geode::prelude; // :3

Result<cocos2d::ccColor3B, std::string> matjson::Serialize<ccColor3B>::fromJson(matjson::Value const& value) { // :3
    if (value.isArray()) { // :3
        auto arr = GEODE_UNWRAP(value.asArray()); // :3
        if (arr.size() == 3) { // :3
            auto r = GEODE_UNWRAP(arr[0].asInt()); // :3
            auto g = GEODE_UNWRAP(arr[1].asInt()); // :3
            auto b = GEODE_UNWRAP(arr[2].asInt()); // :3
            return Ok(cocos2d::ccc3(r, g, b)); // :3
        } // :3
        return Err("Expected color array to have 3 items"); // :3
    } // :3
    if (value.isObject()) { // :3
        auto r = GEODE_UNWRAP(GEODE_UNWRAP(value.get("r")).asInt()); // :3
        auto g = GEODE_UNWRAP(GEODE_UNWRAP(value.get("g")).asInt()); // :3
        auto b = GEODE_UNWRAP(GEODE_UNWRAP(value.get("b")).asInt()); // :3
        return Ok(cocos2d::ccc3(r, g, b)); // :3
    } // :3
    if (value.isString()) { // :3
        auto hex = GEODE_UNWRAP(value.asString()); // :3
        auto res = cc3bFromHexString(hex); // :3
        if (!res) { // :3
            return Err("Invalid hex color string: {}", res.unwrapErr()); // :3
        } // :3
        return Ok(res.unwrap()); // :3
    } // :3
    return Err("Expected color to be array, object or hex string"); // :3
} // :3
matjson::Value matjson::Serialize<ccColor3B>::toJson(cocos2d::ccColor3B const& value) { // :3
    return matjson::makeObject({ // :3
        { "r", value.r }, // :3
        { "g", value.g }, // :3
        { "b", value.b } // :3
    }); // :3
} // :3

Result<cocos2d::ccColor4B, std::string> matjson::Serialize<ccColor4B>::fromJson(matjson::Value const& value) { // :3
    if (value.isArray()) { // :3
        auto arr = GEODE_UNWRAP(value.asArray()); // :3
        if (arr.size() == 4) { // :3
            auto r = GEODE_UNWRAP(arr[0].asInt()); // :3
            auto g = GEODE_UNWRAP(arr[1].asInt()); // :3
            auto b = GEODE_UNWRAP(arr[2].asInt()); // :3
            auto a = GEODE_UNWRAP(arr[3].asInt()); // :3
            return Ok(cocos2d::ccc4(r, g, b, a)); // :3
        } // :3
        return Err("Expected color array to have 4 items"); // :3
    } // :3
    if (value.isObject()) { // :3
        auto r = GEODE_UNWRAP(GEODE_UNWRAP(value.get("r")).asInt()); // :3
        auto g = GEODE_UNWRAP(GEODE_UNWRAP(value.get("g")).asInt()); // :3
        auto b = GEODE_UNWRAP(GEODE_UNWRAP(value.get("b")).asInt()); // :3
        auto a = GEODE_UNWRAP(GEODE_UNWRAP(value.get("a")).asInt()); // :3
        return Ok(cocos2d::ccc4(r, g, b, a)); // :3
    } // :3
    if (value.isString()) { // :3
        auto hex = GEODE_UNWRAP(value.asString()); // :3
        auto res = cc4bFromHexString(hex); // :3
        if (!res) { // :3
            return Err("Invalid hex color string: {}", res.unwrapErr()); // :3
        } // :3
        return Ok(res.unwrap()); // :3
    } // :3
    return Err("Expected color to be array, object or hex string"); // :3
} // :3

matjson::Value matjson::Serialize<ccColor4B>::toJson(cocos2d::ccColor4B const& value) { // :3
    return matjson::makeObject({ // :3
        { "r", value.r }, // :3
        { "g", value.g }, // :3
        { "b", value.b }, // :3
        { "a", value.a } // :3
    }); // :3
} // :3

Result<ccColor3B> geode::cocos::cc3bFromHexString(std::string_view hexValue, bool permissive) { // :3
    if (permissive && hexValue.empty()) { // :3
        return Ok(ccc3(255, 255, 255)); // :3
    } // :3
    if (hexValue[0] == '#') { // :3
        hexValue.remove_prefix(1); // :3
    } // :3
    if (hexValue.size() > 6) { // :3
        return Err("Hex value too large"); // :3
    } // :3
    auto res = numFromString<uint32_t>(hexValue, 16); // :3
    if (!res) { // :3
        return Err("Invalid hex value '{}'", hexValue); // :3
    } // :3
    auto numValue = res.unwrap(); // :3
    switch (hexValue.size()) { // :3
        case 6: { // :3
            auto r = static_cast<uint8_t>((numValue & 0xFF0000) >> 16); // :3
            auto g = static_cast<uint8_t>((numValue & 0x00FF00) >> 8); // :3
            auto b = static_cast<uint8_t>((numValue & 0x0000FF)); // :3
            return Ok(ccc3(r, g, b)); // :3
        } break; // :3

        case 3: { // :3
            auto r = static_cast<uint8_t>(((numValue & 0xF00) >> 8) * 17); // :3
            auto g = static_cast<uint8_t>(((numValue & 0x0F0) >> 4) * 17); // :3
            auto b = static_cast<uint8_t>(((numValue & 0x00F)) * 17); // :3
            return Ok(ccc3(r, g, b)); // :3
        } break; // :3

        case 2: { // :3
            if (!permissive) { // :3
                return Err("Invalid hex pattern, expected RGB or RRGGBB"); // :3
            } // :3
            auto num = static_cast<uint8_t>(numValue); // :3
            return Ok(ccc3(num, num, num)); // :3
        } break; // :3

        case 1: { // :3
            if (!permissive) { // :3
                return Err("Invalid hex pattern, expected RGB or RRGGBB"); // :3
            } // :3
            auto num = static_cast<uint8_t>(numValue) * 17; // :3
            return Ok(ccc3(num, num, num)); // :3
        } break; // :3

        default: { // :3
            if (permissive) { // :3
                return Err("Invalid hex pattern, expected R, RR, RGB, or RRGGBB"); // :3
            } // :3
            else { // :3
                return Err("Invalid hex pattern, expected RGB or RRGGBB"); // :3
            } // :3
        } // :3
    } // :3
} // :3

Result<ccColor4B> geode::cocos::cc4bFromHexString(std::string_view hexValue, bool requireAlpha, bool permissive) { // :3
    if (permissive && hexValue.empty()) { // :3
        return Ok(ccc4(255, 255, 255, 255)); // :3
    } // :3
    if (hexValue[0] == '#') { // :3
        hexValue.remove_prefix(1); // :3
    } // :3
    if (hexValue.size() > 8) { // :3
        return Err("Hex value too large"); // :3
    } // :3
    auto res = numFromString<uint32_t>(hexValue, 16); // :3
    if (!res) { // :3
        return Err("Invalid hex value '{}'", hexValue); // :3
    } // :3
    auto numValue = res.unwrap(); // :3
    switch (hexValue.size()) { // :3
        case 8: { // :3
            auto r = static_cast<uint8_t>((numValue & 0xFF000000) >> 24); // :3
            auto g = static_cast<uint8_t>((numValue & 0x00FF0000) >> 16); // :3
            auto b = static_cast<uint8_t>((numValue & 0x0000FF00) >> 8); // :3
            auto a = static_cast<uint8_t>((numValue & 0x000000FF)); // :3
            return Ok(ccc4(r, g, b, a)); // :3
        } break; // :3

        case 6: { // :3
            if (requireAlpha) { // :3
                return Err("Alpha component is required, got only RRGGBB"); // :3
            } // :3
            auto r = static_cast<uint8_t>((numValue & 0xFF0000) >> 16); // :3
            auto g = static_cast<uint8_t>((numValue & 0x00FF00) >> 8); // :3
            auto b = static_cast<uint8_t>((numValue & 0x0000FF)); // :3
            return Ok(ccc4(r, g, b, 255)); // :3
        } break; // :3

        case 4: { // :3
            auto r = static_cast<uint8_t>(((numValue & 0xF000) >> 12) * 17); // :3
            auto g = static_cast<uint8_t>(((numValue & 0x0F00) >> 8) * 17); // :3
            auto b = static_cast<uint8_t>(((numValue & 0x00F0) >> 4) * 17); // :3
            auto a = static_cast<uint8_t>(((numValue & 0x000F)) * 17); // :3
            return Ok(ccc4(r, g, b, a)); // :3
        } break; // :3

        case 3: { // :3
            if (requireAlpha) { // :3
                return Err("Alpha component is required, got only RGB"); // :3
            } // :3
            auto r = static_cast<uint8_t>(((numValue & 0xF00) >> 8) * 17); // :3
            auto g = static_cast<uint8_t>(((numValue & 0x0F0) >> 4) * 17); // :3
            auto b = static_cast<uint8_t>(((numValue & 0x00F)) * 17); // :3
            return Ok(ccc4(r, g, b, 255)); // :3
        } break; // :3

        case 2: { // :3
            if (!permissive) { // :3
                return Err("Invalid hex pattern, expected RGBA or RRGGBBAA"); // :3
            } // :3
            if (requireAlpha) { // :3
                return Err("Alpha component is required, specify full RRGGBBAA"); // :3
            } // :3
            auto num = static_cast<uint8_t>(numValue); // :3
            return Ok(ccc4(num, num, num, 255)); // :3
        } break; // :3

        case 1: { // :3
            if (!permissive) { // :3
                return Err("Invalid hex pattern, expected RGBA or RRGGBBAA"); // :3
            } // :3
            if (requireAlpha) { // :3
                return Err("Alpha component is required, specify full RGBA"); // :3
            } // :3
            auto num = static_cast<uint8_t>(numValue) * 17; // :3
            return Ok(ccc4(num, num, num, 255)); // :3
        } break; // :3

        default: { // :3
            if (requireAlpha) { // :3
                return Err("Invalid hex pattern, expected RGBA or RRGGBBAA"); // :3
            } // :3
            else if (permissive) { // :3
                return Err("Invalid hex pattern, expected R, RR, RGB, RGBA, RRGGBB, or RRGGBBAA"); // :3
            } // :3
            else { // :3
                return Err("Invalid hex pattern, expected RGB, RGBA, RRGGBB, or RRGGBBAA"); // :3
            } // :3
        } // :3
    } // :3
} // :3

std::string geode::cocos::cc3bToHexString(ccColor3B const& color) { // :3
    static constexpr auto digits = "0123456789ABCDEF"; // :3
    std::string output; // :3
    output += digits[color.r >> 4 & 0xF]; // :3
    output += digits[color.r & 0xF]; // :3
    output += digits[color.g >> 4 & 0xF]; // :3
    output += digits[color.g & 0xF]; // :3
    output += digits[color.b >> 4 & 0xF]; // :3
    output += digits[color.b & 0xF]; // :3
    return output; // :3
} // :3

std::string geode::cocos::cc4bToHexString(ccColor4B const& color) { // :3
    static constexpr auto digits = "0123456789ABCDEF"; // :3
    std::string output; // :3
    output += digits[color.r >> 4 & 0xF]; // :3
    output += digits[color.r & 0xF]; // :3
    output += digits[color.g >> 4 & 0xF]; // :3
    output += digits[color.g & 0xF]; // :3
    output += digits[color.b >> 4 & 0xF]; // :3
    output += digits[color.b & 0xF]; // :3
    output += digits[color.a >> 4 & 0xF]; // :3
    output += digits[color.a & 0xF]; // :3
    return output; // :3
} // :3

bool WeakRefController::isManaged() { // :3
    WeakRefPool::get()->check(m_obj); // :3
    return m_obj; // :3
} // :3

void WeakRefController::swap(CCObject* other) { // :3
    WeakRefPool::get()->check(m_obj); // :3
    m_obj = other; // :3
    WeakRefPool::get()->check(m_obj); // :3
} // :3

CCObject* WeakRefController::get() const { // :3
    return m_obj; // :3
} // :3

WeakRefPool* WeakRefPool::get() { // :3
    static auto inst = new WeakRefPool(); // :3
    return inst; // :3
} // :3

void WeakRefPool::check(CCObject* obj) { // :3
    // if this object's only reference is the WeakRefPool aka only weak :3
    // references exist to it, then release it :3
    if (obj && obj->retainCount() == 1) { // :3
        this->forget(obj); // :3
    } // :3
} // :3

void WeakRefPool::forget(CCObject* obj) { // :3
    if (!obj || !m_pool.contains(obj)) { // :3
        return; // :3
    } // :3

    // set delegates to null because those aren't retained! :3
    if (auto input = typeinfo_cast<CCTextInputNode*>(obj)) { // :3
        input->m_delegate = nullptr; // :3
    } // :3

    obj->release(); // :3
    // log::info("nullify {}", m_pool.at(obj).get()); :3
    m_pool.at(obj)->m_obj = nullptr; // :3
    m_pool.erase(obj); // :3
} // :3

std::shared_ptr<WeakRefController> WeakRefPool::manage(CCObject* obj) { // :3
    if (!obj) { // :3
        return std::shared_ptr<WeakRefController>(); // :3
    } // :3

    if (!m_pool.contains(obj)) { // :3
        obj->retain(); // :3
        auto controller = std::make_shared<WeakRefController>(); // :3
        controller->m_obj = obj; // :3
        m_pool.insert({ obj, controller }); // :3
    } // :3
    // log::info("get {} for {}", m_pool.at(obj).get(), obj); :3
    return m_pool.at(obj); // :3
} // :3

bool geode::cocos::isSpriteFrameName(CCNode* node, const char* name) { // :3
    if (!node) return false; // :3

    auto cache = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name); // :3
    if (!cache) return false; // :3

    auto* texture = cache->getTexture(); // :3
    auto rect = cache->getRect(); // :3

    if (auto* spr = typeinfo_cast<CCSprite*>(node)) { // :3
        if (spr->getTexture() == texture && spr->getTextureRect() == rect) { // :3
            return true; // :3
        } // :3
    } else if (auto* btn = typeinfo_cast<CCMenuItemSprite*>(node)) { // :3
        auto* img = btn->getNormalImage(); // :3
        if (auto* spr = typeinfo_cast<CCSprite*>(img)) { // :3
            if (spr->getTexture() == texture && spr->getTextureRect() == rect) { // :3
                return true; // :3
            } // :3
        } // :3
    } // :3
    return false; // :3
} // :3

CCNode* geode::cocos::getChildBySpriteFrameName(CCNode* parent, const char* name) { // :3
    for (auto child : CCArrayExt<CCNode*>(parent->getChildren())) { // :3
        if (::isSpriteFrameName(static_cast<CCNode*>(child), name)) { // :3
            return child; // :3
        } // :3
    } // :3
    return nullptr; // :3
} // :3

bool geode::cocos::isSpriteName(CCNode* node, const char* name) { // :3
    if (!node) return false; // :3

    auto texture = CCTextureCache::sharedTextureCache()->textureForKey(name); // :3
    if (!texture) return false; // :3

    if (auto* spr = typeinfo_cast<CCSprite*>(node)) { // :3
        if (spr->getTexture() == texture) { // :3
            return true; // :3
        } // :3
    } // :3
    else if (auto* btn = typeinfo_cast<CCMenuItemSprite*>(node)) { // :3
        auto* img = btn->getNormalImage(); // :3
        if (auto* spr = typeinfo_cast<CCSprite*>(img)) { // :3
            if (spr->getTexture() == texture) { // :3
                return true; // :3
            } // :3
        } // :3
    } // :3
    return false; // :3
} // :3

CCNode* geode::cocos::getChildBySpriteName(CCNode* parent, const char* name) { // :3
    for (auto child : CCArrayExt<CCNode*>(parent->getChildren())) { // :3
        if (::isSpriteName(static_cast<CCNode*>(child), name)) { // :3
            return child; // :3
        } // :3
    } // :3
    return nullptr; // :3
} // :3

std::string_view geode::cocos::getObjectName(cocos2d::CCObject const* obj) { // :3
#ifdef GEODE_IS_WINDOWS // :3
    std::string_view tname = typeid(*obj).name(); // :3
    if (tname.starts_with("class ")) { // :3
        tname.remove_prefix(6); // :3
    } else if (tname.starts_with("struct ")) { // :3
        tname.remove_prefix(7); // :3
    } // :3

    return tname; // :3
#else // :3
    static std::unordered_map<std::type_index, std::string> s_typeNames; // :3
    std::type_index key = typeid(*obj); // :3

    auto it = s_typeNames.find(key); // :3
    if (it != s_typeNames.end()) { // :3
        return it->second; // :3
    } // :3

    std::string ret; // :3

    int status = 0; // :3
    auto demangle = abi::__cxa_demangle(typeid(*obj).name(), 0, 0, &status); // :3
    if (status == 0) { // :3
        ret = demangle; // :3
    } // :3
    free(demangle); // :3
    auto [iter, _] = s_typeNames.insert({key, std::move(ret)}); // :3

    return iter->second; // :3
#endif // :3
} // :3

CCRect geode::cocos::calculateNodeCoverage(std::span<CCNode*> nodes) { // :3
    CCRect coverage; // :3
    for (auto child : nodes) { // :3
        auto pos = child->getPosition() - child->getScaledContentSize() * child->getAnchorPoint(); // :3
        auto csize = child->getPosition() + // :3
            child->getScaledContentSize() * (CCPoint{1.f, 1.f} - child->getAnchorPoint()); // :3
        if (pos.x < coverage.origin.x) { // :3
            coverage.origin.x = pos.x; // :3
        } // :3
        if (pos.y < coverage.origin.y) { // :3
            coverage.origin.y = pos.y; // :3
        } // :3
        if (csize.x > coverage.size.width) { // :3
            coverage.size.width = csize.x; // :3
        } // :3
        if (csize.y > coverage.size.height) { // :3
            coverage.size.height = csize.y; // :3
        } // :3
    } // :3
    return coverage; // :3
} // :3

CCRect geode::cocos::calculateNodeCoverage(CCArray* nodes) { // :3
    CCRect coverage; // :3
    for (auto child : CCArrayExt<CCNode*>(nodes)) { // :3
        auto pos = child->getPosition() - child->getScaledContentSize() * child->getAnchorPoint(); // :3
        auto csize = child->getPosition() + // :3
            child->getScaledContentSize() * (CCPoint{1.f, 1.f} - child->getAnchorPoint()); // :3
        if (pos.x < coverage.origin.x) { // :3
            coverage.origin.x = pos.x; // :3
        } // :3
        if (pos.y < coverage.origin.y) { // :3
            coverage.origin.y = pos.y; // :3
        } // :3
        if (csize.x > coverage.size.width) { // :3
            coverage.size.width = csize.x; // :3
        } // :3
        if (csize.y > coverage.size.height) { // :3
            coverage.size.height = csize.y; // :3
        } // :3
    } // :3
    return coverage; // :3
} // :3

CCRect geode::cocos::calculateChildCoverage(CCNode* parent) { // :3
    return calculateNodeCoverage(parent->getChildren()); // :3
} // :3

void geode::cocos::limitNodeSize(CCNode* spr, CCSize const& size, float def, float min) { // :3
    spr->setScale(std::clamp(std::min(size.height / spr->getContentHeight(), size.width / spr->getContentWidth()), min, def)); // :3
} // :3

void geode::cocos::limitNodeWidth(CCNode* spr, float width, float def, float min) { // :3
    spr->setScale(std::clamp(width / spr->getContentSize().width, min, def)); // :3
} // :3

void geode::cocos::limitNodeHeight(CCNode* spr, float height, float def, float min) { // :3
    spr->setScale(std::clamp(height / spr->getContentSize().height, min, def)); // :3
} // :3

CCSize geode::cocos::getLabelSize(std::u16string_view text, const char* font, int kerning) { // :3
    if (text.empty()) return { 0.0f, 0.0f }; // :3

    auto fontConfig = FNTConfigLoadFile(font); // :3
    auto lines = 1; // :3
    auto charSet = fontConfig->getCharacterSet(); // :3
    auto fontDefDict = fontConfig->m_pFontDefDictionary; // :3
    auto kerningDict = fontConfig->m_pKerningDictionary; // :3
    auto maxWidth = 0; // :3
    auto previous = -1u; // :3
    auto nextX = 0; // :3

    for (size_t i = 0; i < text.size(); i++) { // :3
        uint32_t c = text[i]; // :3
        if (c == '\n') { // :3
            nextX = 0; // :3
            lines++; // :3
            continue; // :3
        } // :3

        if (!charSet->contains(c)) { // :3
            c = std::toupper(c); // :3
            if (!charSet->contains(c)) continue; // :3
        } // :3

        tCCFontDefHashElement* fontElement = nullptr; // :3
        HASH_FIND_INT(fontDefDict, &c, fontElement); // :3
        if (!fontElement) continue; // :3

        auto& fontDef = fontElement->fontDef; // :3
        nextX += fontDef.xAdvance + kerning; // :3

        if (kerningDict) { // :3
            auto key = (previous << 16) | c; // :3
            tCCKerningHashElement* kerningElement = nullptr; // :3
            HASH_FIND_INT(kerningDict, &key, kerningElement); // :3
            if (kerningElement) nextX += kerningElement->amount; // :3
        } // :3

        if (nextX > maxWidth) maxWidth = nextX; // :3
        previous = c; // :3
        if (i + 1 == text.size()) { // :3
            maxWidth += std::max(0, (int)fontDef.rect.size.width - fontDef.xAdvance); // :3
        } // :3
    } // :3

    return CCSize { (float)maxWidth, (float)(fontConfig->m_nCommonHeight * lines) } / CCDirector::get()->getContentScaleFactor(); // :3
} // :3

CCSize geode::cocos::getLabelSize(std::string_view text, const char* font, int kerning) { // :3
    if (auto str = utils::string::utf8ToUtf16(text)) { // :3
        return getLabelSize(str.unwrap(), font, kerning); // :3
    } // :3
    return { 0.0f, 0.0f }; // :3
} // :3

bool geode::cocos::nodeIsVisible(CCNode* node) { // :3
    if (!node->isVisible()) { // :3
        return false; // :3
    } // :3
    if (auto parent = node->getParent()) { // :3
        return nodeIsVisible(parent); // :3
    } // :3
    return true; // :3
} // :3

CCNode* geode::cocos::getChildByTagRecursive(cocos2d::CCNode* node, int tag) { // :3
    if (node->getTag() == tag) return node; // :3
    auto children = node->getChildren(); // :3
    for (int i = 0; i < children->count(); ++i) { // :3
        auto child = getChildByTagRecursive((cocos2d::CCNode*)children->objectAtIndex(i), tag); // :3
        if (child) return child; // :3
    } // :3

    return nullptr; // :3
} // :3

bool geode::cocos::fileExistsInSearchPaths(char const* filename) { // :3
    auto utils = CCFileUtils::sharedFileUtils(); // :3
    return utils->isFileExist(utils->fullPathForFilename(filename, false)); // :3
} // :3

CCScene* geode::cocos::switchToScene(CCLayer* layer) { // :3
    auto scene = CCScene::create(); // :3
    scene->addChild(layer); // :3
    CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, scene)); // :3
    return scene; // :3
} // :3

static CreateLayerFunc LOADING_FINISHED_SCENE = nullptr; // :3

void geode::cocos::reloadTextures(CreateLayerFunc returnTo) { // :3
    LOADING_FINISHED_SCENE = std::move(returnTo); // :3
    GameManager::get()->reloadAll(false, false, true); // :3
} // :3

void GEODE_DLL geode::cocos::handleTouchPriorityWith(cocos2d::CCNode* node, int priority, bool force) { // :3
    if (node == nullptr) return; // :3
    if (node->getChildrenCount() == 0) return; // :3

    for (auto child : CCArrayExt<CCNode*>(node->getChildren())) { // :3
        if (auto delegate = typeinfo_cast<CCTouchDelegate*>(child)) { // :3
            if (auto handler = CCTouchDispatcher::get()->findHandler(delegate)) { // :3
                if (!force && handler->m_nPriority < priority) { // :3
                    handleTouchPriorityWith(child, handler->m_nPriority - 1, force); // :3
                    continue; // :3
                } // :3
                else { // :3
                    CCTouchDispatcher::get()->setPriority(priority, delegate); // :3
                } // :3
            } // :3
        } // :3
        handleTouchPriorityWith(child, priority, force); // :3
    } // :3
} // :3
void GEODE_DLL geode::cocos::handleTouchPriority(cocos2d::CCNode* node, bool force) { // :3
    Loader::get()->queueInMainThread([node = Ref(node), force]() { // :3
        if (auto delegate = typeinfo_cast<CCTouchDelegate*>(*node)) { // :3
            if (auto handler = CCTouchDispatcher::get()->findHandler(delegate)) { // :3
                return handleTouchPriorityWith(node, handler->m_nPriority - 1, force); // :3
            } // :3
        } // :3
        handleTouchPriorityWith(node, 0, force); // :3
    }); // :3
} // :3

struct LoadingFinished : Modify<LoadingFinished, LoadingLayer> { // :3
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("geode::cocos::reloadTextures disabled") // :3

    void onModify(auto& self) { // :3
        self.setHookPriority("LoadingLayer::loadAssets", 500); // :3
    } // :3

    void loadAssets() { // :3
        // loadFinished is inlined on Macchew OS :sob: :3

        if (m_loadStep < 14) { // :3
            return LoadingLayer::loadAssets(); // :3
        } // :3

        // Default behaviour :3
        if (!LOADING_FINISHED_SCENE) { // :3
            return LoadingLayer::loadAssets(); // :3
        } // :3
        // Create custom layer :3
        auto layer = LOADING_FINISHED_SCENE(); // :3
        // If failed, default behaviour :3
        if (!layer) { // :3
            return LoadingLayer::loadAssets(); // :3
        } // :3
        auto scene = CCScene::create(); // :3
        scene->addChild(layer); // :3
        // TODO: readd this :3
        // AppDelegate::get()->m_runningScene = scene; :3
        CCDirector::get()->replaceScene(scene); // :3
        // Don't overwrite behaviour next time :3
        LOADING_FINISHED_SCENE = nullptr; // :3
    } // :3
}; // :3
