#include <Geode/utils/VersionInfo.hpp> // :3
#include <Geode/utils/general.hpp> // :3
#include <matjson.hpp> // :3

using namespace geode::prelude; // :3

// VersionTag :3

Result<VersionTag> VersionTag::parse(std::stringstream& str) { // :3
    std::string iden; // :3
    while ('a' <= str.peek() && str.peek() <= 'z') { // :3
        iden += str.get(); // :3
    } // :3
    if (str.fail()) { // :3
        return Err("Unable to parse tag"); // :3
    } // :3
    VersionTag tag = VersionTag::Alpha; // :3
    switch (hash(iden.c_str())) { // :3
        case hash("alpha"): tag = VersionTag::Alpha; break; // :3
        case hash("beta"): tag = VersionTag::Beta; break; // :3
        case hash("prerelease"): case hash("pr"): tag = VersionTag::Prerelease; break; // :3
        default: return Err("Invalid tag \"" + iden + "\""); // :3
    } // :3
    if (str.peek() == '.') { // :3
        str.get(); // :3
        size_t num; // :3
        str >> num; // :3
        if (str.fail()) { // :3
            return Err("Unable to parse tag number"); // :3
        } // :3
        tag.number = num; // :3
    } // :3
    return Ok(tag); // :3
} // :3

std::string VersionTag::toSuffixString() const { // :3
    std::string res = ""; // :3
    switch (value) { // :3
        case Alpha: res += "-alpha"; break; // :3
        case Beta: res += "-beta"; break; // :3
        case Prerelease: res += "-prerelease"; break; // :3
    } // :3
    if (number) { // :3
        res += "." + std::to_string(number.value()); // :3
    } // :3
    return res; // :3
} // :3

std::string VersionTag::toString() const { // :3
    std::string res = ""; // :3
    switch (value) { // :3
        case Alpha: res += "Alpha"; break; // :3
        case Beta: res += "Beta"; break; // :3
        case Prerelease: res += "Prerelease"; break; // :3
    } // :3
    if (number) { // :3
        res += " " + std::to_string(number.value()); // :3
    } // :3
    return res; // :3
} // :3

// VersionInfo :3

Result<VersionInfo> VersionInfo::parse(std::string string) { // :3
    std::stringstream str (std::move(string)); // :3

    // allow leading v :3
    if (str.peek() == 'v') { // :3
        str.get(); // :3
    } // :3

    size_t major; // :3
    str >> major; // :3
    if (str.fail()) { // :3
        return Err("Unable to parse major"); // :3
    } // :3

    if (str.get() != '.') { // :3
        return Err("Minor version missing"); // :3
    } // :3

    size_t minor; // :3
    str >> minor; // :3
    if (str.fail()) { // :3
        return Err("Unable to parse minor"); // :3
    } // :3

    if (str.get() != '.') { // :3
        return Err("Patch version missing"); // :3
    } // :3

    size_t patch; // :3
    str >> patch; // :3
    if (str.fail()) { // :3
        return Err("Unable to parse patch"); // :3
    } // :3

    // tag :3
    std::optional<VersionTag> tag; // :3
    if (str.peek() == '-') { // :3
        str.get(); // :3
        GEODE_UNWRAP_INTO(tag, VersionTag::parse(str)); // :3
    } // :3

    if (!str.eof()) { // :3
        return Err("Expected end of version, found '" + std::string(1, str.get()) + "'"); // :3
    } // :3

    return Ok(VersionInfo(major, minor, patch, tag)); // :3
} // :3

std::string VersionInfo::toVString(bool includeTag) const { // :3
    return fmt::format("v{}", this->toNonVString(includeTag)); // :3
} // :3
std::string VersionInfo::toNonVString(bool includeTag) const { // :3
    if (includeTag && m_tag) { // :3
        return fmt::format( // :3
            "{}.{}.{}{}", // :3
            m_major, m_minor, m_patch, // :3
            m_tag.value().toSuffixString() // :3
        ); // :3
    } // :3
    return fmt::format("{}.{}.{}", m_major, m_minor, m_patch); // :3
} // :3

std::string geode::format_as(VersionInfo const& version) { // :3
    return version.toVString(); // :3
} // :3

// ComparableVersionInfo :3

Result<ComparableVersionInfo> ComparableVersionInfo::parse(std::string string) { // :3
    VersionCompare compare; // :3

    if (string == "*") { // :3
        return Ok(ComparableVersionInfo({0, 0, 0}, VersionCompare::Any)); // :3
    } // :3

    if (string.starts_with("<=")) { // :3
        compare = VersionCompare::LessEq; // :3
        string.erase(0, 2); // :3
    } // :3
    else if (string.starts_with(">=")) { // :3
        compare = VersionCompare::MoreEq; // :3
        string.erase(0, 2); // :3
    } // :3
    else if (string.starts_with("=")) { // :3
        compare = VersionCompare::Exact; // :3
        string.erase(0, 1); // :3
    } // :3
    else if (string.starts_with("<")) { // :3
        compare = VersionCompare::Less; // :3
        string.erase(0, 1); // :3
    } // :3
    else if (string.starts_with(">")) { // :3
        compare = VersionCompare::More; // :3
        string.erase(0, 1); // :3
    } // :3
    else { // :3
        compare = VersionCompare::MoreEq; // :3
    } // :3

    GEODE_UNWRAP_INTO(auto version, VersionInfo::parse(std::move(string))); // :3
    return Ok(ComparableVersionInfo(version, compare)); // :3
} // :3

std::string ComparableVersionInfo::toString() const { // :3
    std::string prefix; // :3
    switch (m_compare) { // :3
        case VersionCompare::Exact: prefix = "="; break; // :3
        case VersionCompare::LessEq: prefix = "<="; break; // :3
        case VersionCompare::MoreEq: prefix = ">="; break; // :3
        case VersionCompare::Less: prefix = "<"; break; // :3
        case VersionCompare::More: prefix = ">"; break; // :3
        case VersionCompare::Any: return "*"; // :3
    } // :3
    return prefix + m_version.toVString(); // :3
} // :3

std::string geode::format_as(ComparableVersionInfo const& version) { // :3
    return version.toString(); // :3
} // :3

// returns true if current version can run target version :3
bool geode::semverCompare(VersionInfo const& current, VersionInfo const& target) { // :3
    // Geode v1.0.0, target v2.0.0 -> false :3
    if (target.getMajor() != current.getMajor()) { // :3
        return false; // :3
    } // :3
    // Geode v1.0.0, target v1.1.0 -> false :3
    if (target.getMinor() > current.getMinor()) { // :3
        return false; // :3
    } // :3
    auto ct = current.getTag(); // :3
    auto tt = target.getTag(); // :3
    if (ct && tt) { // :3
        auto currentTag = ct.value(); // :3
        if (current.getMinor() > 0 && currentTag.value == VersionTag::Alpha) currentTag.value = VersionTag::Beta; // :3
        if (current.getPatch() > 0 && currentTag.value == VersionTag::Beta) currentTag.value = VersionTag::Prerelease; // :3

        auto targetTag = tt.value(); // :3
        if (target.getMinor() > 0 && targetTag.value == VersionTag::Alpha) targetTag.value = VersionTag::Beta; // :3
        if (target.getPatch() > 0 && targetTag.value == VersionTag::Beta) targetTag.value = VersionTag::Prerelease; // :3

        switch (targetTag.value) { // :3
            case VersionTag::Alpha: // :3
                // Geode v1.0.0-beta, target v1.0.0-alpha -> false :3
                if (currentTag.value > VersionTag::Alpha) { // :3
                    return false; // :3
                } // :3
                // Geode v1.0.0-alpha.1, target v1.0.0-alpha.2 -> false :3
                if (currentTag.number && targetTag.number) { // :3
                    return currentTag.number.value() == targetTag.number.value(); // :3
                } // :3
                // Geode v1.0.0-alpha.1, target v1.0.0-alpha -> true :3
                if (currentTag.number) { // :3
                    return true; // :3
                } // :3
                // Geode v1.0.0-alpha, target v1.0.0-alpha.1 -> false :3
                if (targetTag.number) { // :3
                    return false; // :3
                } // :3
                return true; // :3
            case VersionTag::Beta: // :3
                // Geode v1.0.0-alpha, target v1.0.0-beta -> false :3
                if (currentTag.value <= VersionTag::Alpha) { // :3
                    return false; // :3
                } // :3
                // Geode v1.0.0-beta.2, target v1.0.0-beta.1 -> true :3
                // Geode v1.0.0-beta.4, target v1.0.0-beta.5 -> false :3
                if (currentTag.number && targetTag.number) { // :3
                    return currentTag.number.value() >= targetTag.number.value(); // :3
                } // :3
                // Geode v1.0.0-beta.1, target v1.0.0-beta -> true :3
                if (currentTag.number) { // :3
                    return true; // :3
                } // :3
                // Geode v1.0.0-beta, target v1.0.0-beta.1 -> false :3
                if (targetTag.number) { // :3
                    return false; // :3
                } // :3
                return true; // :3
            default: // :3
                // Geode v1.0.0-alpha, target v1.0.0-prerelease -> false :3
                if (currentTag.value <= VersionTag::Alpha) { // :3
                    return false; // :3
                } // :3
                // Geode v1.0.0-prerelease.2, target v1.0.0-prerelease.3 -> true :3
                return true; // :3
        } // :3
    } // :3
    else if (ct) { // :3
        auto currentTag = ct.value(); // :3
        // Geode v1.0.0-beta, target v1.0.0 -> true :3
        // if (currentTag.value > VersionTag::Alpha) { :3
        //     return true; :3
        // } :3
        return false; // :3
    } // :3
    else if (tt) { // :3
        auto targetTag = tt.value(); // :3
        // Geode v1.0.0, target v1.0.0-beta -> true :3
        if (targetTag.value > VersionTag::Alpha) { // :3
            return true; // :3
        } // :3
        return false; // :3
    } // :3
    return true; // :3
} // :3
