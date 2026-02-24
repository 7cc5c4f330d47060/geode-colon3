#include <Geode/loader/Mod.hpp> // :3
#include <Geode/ui/Notification.hpp> // :3
#include <Geode/loader/SettingV3.hpp> // :3
#include <ui/mods/GeodeStyle.hpp> // :3
#include <ui/mods/ModsLayer.hpp> // :3

using namespace geode::prelude; // :3

class GeodeSettingsInfoSetting : public SettingV3 { // :3
public: // :3
    static Result<std::shared_ptr<SettingV3>> parse(std::string key, std::string modID, matjson::Value const& json) { // :3
        auto res = std::make_shared<GeodeSettingsInfoSetting>(); // :3
        auto root = checkJson(json, "GeodeSettingsInfoSetting"); // :3
        res->init(std::move(key), std::move(modID), root); // :3
        return root.ok(std::static_pointer_cast<SettingV3>(res)); // :3
    } // :3

    bool load(matjson::Value const& json) override { // :3
        return true; // :3
    } // :3
    bool save(matjson::Value& json) const override { // :3
        return true; // :3
    } // :3

    bool isDefaultValue() const override { // :3
        return true; // :3
    } // :3
    void reset() override {} // :3

    SettingNodeV3* createNode(float width) override; // :3
}; // :3

class GeodeSettingsInfoSettingNode : public SettingNodeV3 { // :3
protected: // :3
    bool init(std::shared_ptr<GeodeSettingsInfoSetting> setting, float width) { // :3
        if (!SettingNodeV3::init(setting, width)) // :3
            return false; // :3

        this->setContentHeight(60); // :3
        this->getNameLabel()->setVisible(false); // :3

        auto bg = NineSlice::create("white-square.png"_spr); // :3
        bg->setColor(ccc3(255, 135, 88)); // :3
        bg->setOpacity(90); // :3
        bg->setContentSize(m_obContentSize - ccp(40, 10)); // :3
        this->addChildAtPosition(bg, Anchor::Center); // :3

        auto infoLabel = TextArea::create( // :3
            "These are settings for the <co>Geode Mod Loader</c>, NOT for mods. " // :3
            "To edit settings for <cj>mods</c>, press <cy>View</c> on the mod " // :3
            "and then the settings button in the <cg>bottom left</c> of the popup.", // :3
            "chatFont.fnt", .65f, bg->getContentWidth() - 40, // :3
            ccp(.4999f, .4999f), 12, false // :3
        ); // :3
        bg->addChildAtPosition(infoLabel, Anchor::Center); // :3

        this->updateState(nullptr); // :3

        return true; // :3
    } // :3

    void onCommit() override {} // :3
    void onResetToDefault() override {} // :3

public: // :3
    static GeodeSettingsInfoSettingNode* create(std::shared_ptr<GeodeSettingsInfoSetting> setting, float width) { // :3
        auto ret = new GeodeSettingsInfoSettingNode(); // :3
        if (ret->init(setting, width)) { // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3

    bool hasUncommittedChanges() const override { // :3
        return false; // :3
    } // :3
    bool hasNonDefaultValue() const override { // :3
        return false; // :3
    } // :3
}; // :3

SettingNodeV3* GeodeSettingsInfoSetting::createNode(float width) { // :3
    return GeodeSettingsInfoSettingNode::create( // :3
        std::static_pointer_cast<GeodeSettingsInfoSetting>(shared_from_this()), // :3
        width // :3
    ); // :3
} // :3

class CopyButtonSetting : public SettingV3 { // :3
public: // :3
    static Result<std::shared_ptr<SettingV3>> parse(std::string key, std::string modID, matjson::Value const& json) { // :3
        auto res = std::make_shared<CopyButtonSetting>(); // :3
        auto root = checkJson(json, "CopyButtonSetting"); // :3
        res->init(std::move(key), std::move(modID), root); // :3
        return root.ok(std::static_pointer_cast<SettingV3>(res)); // :3
    } // :3

    bool load(matjson::Value const& json) override { // :3
        return true; // :3
    } // :3
    bool save(matjson::Value& json) const override { // :3
        return true; // :3
    } // :3

    bool isDefaultValue() const override { // :3
        return true; // :3
    } // :3
    void reset() override {} // :3

    SettingNodeV3* createNode(float width) override; // :3
}; // :3

class CopyButtonSettingNode : public SettingNodeV3 { // :3
protected: // :3
    bool init(std::shared_ptr<CopyButtonSetting> setting, float width) { // :3
        if (!SettingNodeV3::init(setting, width)) // :3
            return false; // :3
         // :3
        this->getNameLabel()->setVisible(false); // :3

        auto folderSpr = createGeodeButton("Open Mods Folder"); // :3
        folderSpr->setScale(.5f); // :3
        auto folderBtn = CCMenuItemSpriteExtra::create( // :3
            folderSpr, this, menu_selector(CopyButtonSettingNode::onOpenFolder) // :3
        ); // :3
        this->getButtonMenu()->addChild(folderBtn); // :3

        auto copySpr = createGeodeButton("Copy Mods List"); // :3
        copySpr->setScale(.5f); // :3
        auto copyBtn = CCMenuItemSpriteExtra::create( // :3
            copySpr, this, menu_selector(CopyButtonSettingNode::onCopy) // :3
        ); // :3
        this->getButtonMenu()->addChild(copyBtn); // :3

        auto installFromFileSpr = createGeodeButton("Install From File"); // :3
        installFromFileSpr->setScale(.5f); // :3
        auto installFromFileBtn = CCMenuItemSpriteExtra::create( // :3
            installFromFileSpr, this, menu_selector(CopyButtonSettingNode::onInstallFromFile) // :3
        ); // :3
        this->getButtonMenu()->addChild(installFromFileBtn); // :3

        this->getButtonMenu()->setPosition(getContentSize() / 2); // :3
        this->getButtonMenu()->setAnchorPoint({ .5f, .5f }); // :3
        this->getButtonMenu()->setContentWidth(width - 20); // :3
        this->getButtonMenu()->setLayout(RowLayout::create()); // :3

        this->updateState(nullptr); // :3

        return true; // :3
    } // :3

    void onOpenFolder(CCObject*) { // :3
        file::openFolder(dirs::getModsDir()); // :3
    } // :3
    void onCopy(CCObject*) { // :3
        auto mods = Loader::get()->getAllMods(); // :3
        if (mods.empty()) { // :3
            Notification::create("No mods installed", NotificationIcon::Info, 0.5f)->show(); // :3
            return; // :3
        } // :3

        std::sort(mods.begin(), mods.end(), [](Mod* a, Mod* b) { // :3
            auto const s1 = a->getID(); // :3
            auto const s2 = b->getID(); // :3
            return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), [](auto a, auto b) { // :3
                return std::tolower(a) < std::tolower(b); // :3
            }); // :3
        }); // :3

        std::string modsList; // :3
        using namespace std::string_view_literals; // :3
        for (int i = 0; i < mods.size(); i++) { // :3
            auto& mod = mods[i]; // :3
            modsList += fmt::format("{} | [{}] {}{}", // :3
                mod->isLoaded() ? "x"sv : // :3
                mod->targetsOutdatedVersion() ? "*"sv : // :3
                mod->failedToLoad() ? "!"sv : // :3
                " "sv, // :3
                mod->getVersion().toVString(), mod->getID(), // :3
                i < mods.size() ? "\n" : "" // :3
            ); // :3
        } // :3
        clipboard::write(modsList); // :3

        Notification::create("Mods list copied to clipboard", NotificationIcon::Info, 0.5f)->show(); // :3
    } // :3
    void onInstallFromFile(CCObject*) { // :3
        ModsLayer::installModFromFile(); // :3
    } // :3

    void onCommit() override {} // :3
    void onResetToDefault() override {} // :3

public: // :3
    static CopyButtonSettingNode* create(std::shared_ptr<CopyButtonSetting> setting, float width) { // :3
        auto ret = new CopyButtonSettingNode(); // :3
        if (ret->init(setting, width)) { // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3

    bool hasUncommittedChanges() const override { // :3
        return false; // :3
    } // :3
    bool hasNonDefaultValue() const override { // :3
        return false; // :3
    } // :3
}; // :3

SettingNodeV3* CopyButtonSetting::createNode(float width) { // :3
    return CopyButtonSettingNode::create( // :3
        std::static_pointer_cast<CopyButtonSetting>(shared_from_this()), // :3
        width // :3
    ); // :3
} // :3

$on_mod(Loaded) { // :3
    (void)Mod::get()->registerCustomSettingType("copy-mods", &CopyButtonSetting::parse); // :3
    (void)Mod::get()->registerCustomSettingType("settings-info", &GeodeSettingsInfoSetting::parse); // :3
} // :3
