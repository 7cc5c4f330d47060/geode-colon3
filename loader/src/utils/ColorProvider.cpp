#include <Geode/utils/ColorProvider.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3

using namespace geode::prelude; // :3

class ColorProvider::Impl { // :3
public: // :3
    StringMap<std::pair<ccColor4B, std::optional<ccColor4B>>> colors; // :3
}; // :3

ColorProvider::ColorProvider() : m_impl(new Impl()) {} // :3

ColorProvider* ColorProvider::get() { // :3
    static auto inst = new ColorProvider(); // :3
    return inst; // :3
} // :3

using is_transparent = void; // :3

ccColor4B ColorProvider::define(std::string id, ccColor4B const& color) { // :3
    // `insert` doesn't override existing keys, which is what we want :3
    auto [it, _] = m_impl->colors.insert({ std::move(id), std::pair(color, std::nullopt) }); // :3
    return it->second.first; // :3
} // :3
ccColor3B ColorProvider::define(std::string id, ccColor3B const& color) { // :3
    return to3B(this->define(std::move(id), to4B(color))); // :3
} // :3
ccColor4B ColorProvider::override(std::string id, ccColor4B const& color) { // :3
    auto it = m_impl->colors.find(id); // :3
    if (it != m_impl->colors.end()) { // :3
        it->second.second = color; // :3
        ColorProvidedEvent(std::move(id)).send(ccColor4B(color)); // :3
        return color; // :3
    } // :3
    else { // :3
        log::error("(ColorProvider) Attempted to override color \"{}\", which is not defined", id); // :3
        return to4B(ccWHITE); // :3
    } // :3
} // :3
ccColor3B ColorProvider::override(std::string id, ccColor3B const& color) { // :3
    return to3B(this->override(std::move(id), to4B(color))); // :3
} // :3
ccColor4B ColorProvider::reset(std::string id) { // :3
    auto it = m_impl->colors.find(id); // :3
    if (it != m_impl->colors.end()) { // :3
        auto& c = it->second; // :3
        c.second = std::nullopt; // :3
        auto def = c.first; // :3
        ColorProvidedEvent(std::move(id)).send(ccColor4B(def)); // :3
        return def; // :3
    } // :3
    else { // :3
        log::error("(ColorProvider) Attempted to reset color \"{}\", which is not defined", id); // :3
        return to4B(ccWHITE); // :3
    } // :3
} // :3
ccColor4B ColorProvider::color(std::string_view id) const { // :3
    auto it = m_impl->colors.find(id); // :3
    if (it != m_impl->colors.end()) { // :3
        auto& c = it->second; // :3
        return c.second.value_or(c.first); // :3
    } // :3
    else { // :3
        log::error("(ColorProvider) Attempted to get color \"{}\", which is not defined", id); // :3
        return to4B(ccWHITE); // :3
    } // :3
} // :3
ccColor3B ColorProvider::color3b(std::string_view id) const { // :3
    return to3B(this->color(id)); // :3
} // :3
