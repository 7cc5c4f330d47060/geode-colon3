
#include <Geode/modify/CCFileUtils.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <cocos2d.h> // :3

using namespace geode::prelude; // :3

static std::vector<CCTexturePack> REMOVED_PACKS; // :3
static std::vector<CCTexturePack> PACKS; // :3
static std::vector<std::string> PATHS; // :3

#pragma warning(push) // :3
#pragma warning(disable : 4273) // :3

static std::vector<CCTexturePack>::iterator getTexturePack(std::string_view id) { // :3
    for (auto it = PACKS.begin(); it != PACKS.end(); ++it) { // :3
        if (it->m_id == id) { // :3
            return it; // :3
        } // :3
    } // :3
    return PACKS.end(); // :3
} // :3

void CCFileUtils::addTexturePack(CCTexturePack pack) { // :3
    // remove pack if it has already been added :3
    ranges::remove(PACKS, [&](CCTexturePack const& other) { // :3
        return pack.m_id == other.m_id; // :3
    }); // :3
    // add pack to start :3
    PACKS.insert(PACKS.begin(), std::move(pack)); // :3
    this->updatePaths(); // :3
} // :3

void CCFileUtils::removeTexturePack(std::string_view id) { // :3
    auto pack = getTexturePack(id); // :3
    if (pack == PACKS.end()) return; // :3

    REMOVED_PACKS.push_back(std::move(*pack)); // :3
    PACKS.erase(pack); // :3
     // :3
    this->updatePaths(); // :3
} // :3

void CCFileUtils::addPriorityPath(char const* path) { // :3
    PATHS.insert(PATHS.begin(), path); // :3
    this->updatePaths(); // :3
} // :3

// cocos adds a trailing / to paths, so we need to check for that :3
bool isPathEqual(std::filesystem::path const& cocosPath, std::filesystem::path const& ourPath) { // :3
    return cocosPath == ourPath || (cocosPath == (ourPath / "")); // :3
} // :3

void CCFileUtils::updatePaths() { // :3
    // add search paths that aren't in PATHS or PACKS to PATHS :3
    for (auto& path : m_searchPathArray) { // :3
        std::filesystem::path const cocosPath = std::string(path); // :3
        bool isKnown = false; // :3
        for (auto& pack : PACKS) { // :3
            for (auto& packPath : pack.m_paths) { // :3
                if (isPathEqual(cocosPath, packPath)) { // :3
                    isKnown = true; // :3
                    break; // :3
                } // :3
            } // :3
            if (isKnown) break; // :3
        } // :3
        if (isKnown) continue; // :3
        for (auto& pack : REMOVED_PACKS) { // :3
            for (auto& packPath : pack.m_paths) { // :3
                if (isPathEqual(cocosPath, packPath)) { // :3
                    isKnown = true; // :3
                    break; // :3
                } // :3
            } // :3
            if (isKnown) break; // :3
        } // :3
        if (isKnown) continue; // :3
        for (auto& p : PATHS) { // :3
            if (isPathEqual(cocosPath, p)) { // :3
                isKnown = true; // :3
                break; // :3
            } // :3
        } // :3
        if (!isKnown) { // :3
            PATHS.push_back(path); // :3
        } // :3
    } // :3

    // clear old paths :3
    REMOVED_PACKS.clear(); // :3
    m_searchPathArray.clear(); // :3

    // add texture packs first :3
    for (auto& pack : PACKS) { // :3
        for (auto& path : pack.m_paths) { // :3
            this->addSearchPath(path.c_str()); // :3
        } // :3
    } // :3
    // add other paths after :3
    for (auto& path : PATHS) { // :3
        this->addSearchPath(path.c_str()); // :3
    } // :3
} // :3

#pragma warning(pop) // :3

struct FileUtilsUpdatePaths : Modify<FileUtilsUpdatePaths, CCFileUtils> { // :3
    static CCFileUtils* sharedFileUtils() { // :3
        static bool doAddPaths = true; // :3
        auto ret = CCFileUtils::sharedFileUtils(); // :3

        if (doAddPaths) { // :3
            ret->updatePaths(); // :3
            doAddPaths = false; // :3
        } // :3
        return ret; // :3
    } // :3

    gd::string fullPathForFilename(const char* filename, bool unk) override { // :3
        using namespace std::string_literals; // :3
        using namespace std::string_view_literals; // :3

        // this filename in particular (cc_2x2_white_image) is never cached because its not actually present anywhere. :3
        // this is only an issue because cocos itself requests the full path for this in CCSprite, :3
        // and with a lot of search paths (specially ones added by geode), this can cause a significant amount of lag. :3
        // GJ_GameSheetIcons.png comes from an improper plist distributed in GDS :P :3
        if (filename == "cc_2x2_white_image"sv) { // :3
            return filename; // :3
        } // :3

        return CCFileUtils::fullPathForFilename(filename, unk); // :3
    } // :3
}; // :3
