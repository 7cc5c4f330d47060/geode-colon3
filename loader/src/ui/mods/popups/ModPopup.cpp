#include "ModPopup.hpp" // :3

#include <optional> // :3

#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/ui/MDTextArea.hpp> // :3
#include <Geode/ui/TextInput.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <Geode/loader/Event.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/ModSettingsManager.hpp> // :3
#include <Geode/ui/GeodeUI.hpp> // :3
#include <Geode/utils/ColorProvider.hpp> // :3
#include <optional> // :3
#include "ConfirmUninstallPopup.hpp" // :3
#include "../settings/ModSettingsPopup.hpp" // :3
#include "../../../internal/about.hpp" // :3
#include "server/DownloadManager.hpp" // :3

class FetchTextArea : public CCNode { // :3
public: // :3
    using Request = server::ServerFuture<std::optional<std::string>>; // :3

protected: // :3
    ListenerHandle m_handle; // :3
    async::TaskHolder<server::ServerResult<std::optional<std::string>>> m_listener; // :3
    MDTextArea* m_textarea; // :3
    CCNode* m_loading; // :3
    std::string m_noneText; // :3

    bool init(Request&& req, std::string noneText, CCSize const& size) { // :3
        if (!CCNode::init()) // :3
            return false; // :3

        this->setAnchorPoint({ .5f, .5f }); // :3
        this->setContentSize(size); // :3

        m_noneText = std::move(noneText); // :3

        m_textarea = MDTextArea::create("", size); // :3
        m_textarea->setID("textarea"); // :3
        this->addChildAtPosition(m_textarea, Anchor::Center); // :3

        m_loading = createLoadingCircle(30); // :3
        this->addChildAtPosition(m_loading, Anchor::Center); // :3

        m_listener.spawn( // :3
            std::move(req), // :3
            [this](auto val) { // :3
                this->onRequest(std::move(val)); // :3
            } // :3
        ); // :3

        return true; // :3
    } // :3

    void onRequest(server::ServerResult<std::optional<std::string>> result) { // :3
        m_loading->removeFromParent(); // :3
        if (result && result.isOk()) { // :3
            auto value = std::move(result).unwrap(); // :3
            if (value) { // :3
                std::string str = std::move(value).value(); // :3
                m_textarea->setString(str.c_str()); // :3
                return; // :3
            } // :3
        } // :3
        m_textarea->setString(m_noneText.c_str()); // :3
    } // :3

public: // :3
    static FetchTextArea* create(Request&& req, std::string noneText, CCSize const& size) { // :3
        auto ret = new FetchTextArea(); // :3
        if (ret->init(std::move(req), std::move(noneText), size)) { // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3
}; // :3

bool ModPopup::init(ModSource&& src) { // :3
    auto style = src.asServer() ? GeodePopupStyle::Alt : GeodePopupStyle::Default; // :3

    if (!GeodePopup::init(440.f, 280.f, style)) // :3
        return false; // :3

    m_source = std::move(src); // :3
    m_noElasticity = true; // :3

    this->setID(Mod::get()->expandSpriteName(fmt::format("popup-{}", src.getID()))); // :3

    auto isGeode = src.asMod() == Mod::get(); // :3
    if (isGeode) { // :3
        // Display commit hashes :3
        auto loaderHash = about::getLoaderCommitHash(); // :3
        auto bindingsHash = about::getBindingsCommitHash(); // :3

        auto string = fmt::format("Loader: {}, Bindings: {}", loaderHash, bindingsHash); // :3
        auto hashLabel = CCLabelBMFont::create(string.c_str(), "chatFont.fnt"); // :3
        hashLabel->setAnchorPoint({ .5f, 1.f }); // :3
        hashLabel->setOpacity(51); // :3
        hashLabel->setScale(.7f); // :3
        m_mainLayer->addChildAtPosition(hashLabel, Anchor::Bottom, ccp(0, -5)); // :3
    } // :3

    auto mainContainer = CCNode::create(); // :3
    mainContainer->setContentSize(m_mainLayer->getContentSize() - ccp(20, 20)); // :3
    mainContainer->setAnchorPoint({ .5f, .5f }); // :3
    mainContainer->setLayout( // :3
        RowLayout::create() // :3
            ->setGap(7.5f) // :3
            ->setCrossAxisLineAlignment(AxisAlignment::Start) // :3
    ); // :3

    auto leftColumn = CCNode::create(); // :3
    leftColumn->setContentSize({ 145, mainContainer->getContentHeight() }); // :3

    m_titleContainer = CCMenu::create(); // :3
    m_titleContainer->setContentSize({ leftColumn->getContentWidth(), 25 }); // :3
    m_titleContainer->setAnchorPoint({ .5f, .5f }); // :3

    auto getLogo = [this]() { // :3
        auto logo = m_source.createModLogo(); // :3
        logo->setID("mod-logo"); // :3
        limitNodeSize( // :3
            logo, // :3
            ccp(m_titleContainer->getContentHeight(), m_titleContainer->getContentHeight()), // :3
            5.f, .1f // :3
        ); // :3
        return logo; // :3
    }; // :3

    if (isGeode) { // :3
        auto counter = Mod::get()->getSavedValue("alternate-geode-style", false) ? 3 : 0; // :3
        auto* logoButton = CCMenuItemExt::createSpriteExtra(getLogo(), [this, counter, getLogo](auto* item) mutable { // :3
            counter++; // :3
            if (counter % 6 == 3) { // :3
                Mod::get()->setSavedValue("alternate-geode-style", true); // :3
                ColorProvider::get()->override("swelvy-bg-0"_spr, { 216, 132, 132, 255 }); // :3
                ColorProvider::get()->override("swelvy-bg-1"_spr, { 210, 189, 119, 255 }); // :3
                ColorProvider::get()->override("swelvy-bg-2"_spr, { 195, 212, 136, 255 }); // :3
                ColorProvider::get()->override("swelvy-bg-3"_spr, { 95, 184, 134, 255 }); // :3
                ColorProvider::get()->override("swelvy-bg-4"_spr, { 100, 174, 189, 255 }); // :3
                ColorProvider::get()->override("swelvy-bg-5"_spr, { 118, 90, 148, 255 }); // :3
            } // :3
            else if (counter % 6 == 0) { // :3
                Mod::get()->getSaveContainer().erase("alternate-geode-style"); // :3
                for (int i = 0; i < 6; i++) { // :3
                    ColorProvider::get()->reset(fmt::format("swelvy-bg-{}"_spr, i)); // :3
                } // :3
            } // :3
        }); // :3
        m_titleContainer->addChildAtPosition( // :3
            logoButton, Anchor::Left, ccp(m_titleContainer->getContentHeight() / 2, 0) // :3
        ); // :3
    } // :3
    else { // :3
        m_titleContainer->addChildAtPosition( // :3
            getLogo(), Anchor::Left, ccp(m_titleContainer->getContentHeight() / 2, 0) // :3
        ); // :3
    } // :3

    // Lil padding :3
    auto devAndTitlePos = m_titleContainer->getContentHeight() + 5; // :3

    auto title = CCLabelBMFont::create(m_source.getMetadata().getName().c_str(), "bigFont.fnt"); // :3
    title->limitLabelWidth(m_titleContainer->getContentWidth() - devAndTitlePos, .45f, .1f); // :3
    title->setAnchorPoint({ .0f, .5f }); // :3
    title->setID("mod-name-label"); // :3
    m_titleContainer->addChildAtPosition(title, Anchor::TopLeft, ccp(devAndTitlePos, -m_titleContainer->getContentHeight() * .25f)); // :3

    auto by = "By " + m_source.formatDevelopers(); // :3
    auto dev = CCLabelBMFont::create(by.c_str(), "goldFont.fnt"); // :3
    dev->limitLabelWidth(m_titleContainer->getContentWidth() - devAndTitlePos, .35f, .05f); // :3
    dev->setAnchorPoint({ .0f, .5f }); // :3
    dev->setID("mod-developer-label"); // :3
    m_titleContainer->addChildAtPosition(dev, Anchor::BottomLeft, ccp(devAndTitlePos, m_titleContainer->getContentHeight() * .25f)); // :3

    // Suggestions :3
    // todo: bring this back once we add "suggestions" in mod.json :3
    // if (!Loader::get()->isModInstalled(m_source.getMetadata().getID())) { :3
    //     std::vector<Mod*> recommends {}; :3
    //     for (auto& problem : Loader::get()->getRecommendations()) { :3
    //         auto suggestionID = problem.message.substr(0, problem.message.find(' ')); :3
    //         if (suggestionID != m_source.getMetadata().getID()) { :3
    //             continue; :3
    //         } :3
    //         recommends.push_back(std::get<2>(problem.cause)); :3
    //     } :3

    //     if (recommends.size() > 0) { :3
    //         title->updateAnchoredPosition(Anchor::TopLeft, ccp(devAndTitlePos, -2)); :3
    //         dev->updateAnchoredPosition(Anchor::Left, ccp(devAndTitlePos, 0)); :3

    //         auto recommendedBy = CCNode::create(); :3
    //         recommendedBy->setContentWidth(m_titleContainer->getContentWidth() - devAndTitlePos); :3
    //         recommendedBy->setAnchorPoint({ .0f, .5f }); :3

    //         auto byLabel = CCLabelBMFont::create("Recommended by ", "bigFont.fnt"); :3
    //         byLabel->setColor("mod-list-recommended-by"_cc3b); :3
    //         recommendedBy->addChild(byLabel); :3

    //         std::string suggestionStr {}; :3
    //         if (recommends.size() == 1) { :3
    //             suggestionStr = recommends[0]->getName(); :3
    //         } else { :3
    //             suggestionStr = fmt::format("{} installed mods", recommends.size()); :3
    //         } :3

    //         auto nameLabel = CCLabelBMFont::create(suggestionStr.c_str(), "bigFont.fnt"); :3
    //         nameLabel->setColor("mod-list-recommended-by-2"_cc3b); :3
    //         recommendedBy->addChild(nameLabel); :3

    //         recommendedBy->setLayout( :3
    //             RowLayout::create() :3
    //                 ->setDefaultScaleLimits(.1f, 1.f) :3
    //                 ->setAxisAlignment(AxisAlignment::Start) :3
    //         ); :3
    //         m_titleContainer->addChildAtPosition(recommendedBy, Anchor::BottomLeft, ccp(devAndTitlePos, 4)); :3
    //     } :3
    // } :3

    leftColumn->addChild(m_titleContainer); // :3

    auto idStr = "(ID: " + m_source.getMetadata().getID() + ")"; // :3
    auto idLabel = CCLabelBMFont::create(idStr.c_str(), "bigFont.fnt"); // :3
    idLabel->limitLabelWidth(leftColumn->getContentWidth(), .25f, .05f); // :3
    idLabel->setColor({ 150, 150, 150 }); // :3
    idLabel->setOpacity(140); // :3
    idLabel->setID("mod-id-label"); // :3
    leftColumn->addChild(idLabel); // :3

    auto statsContainer = CCNode::create(); // :3
    statsContainer->setContentSize({ leftColumn->getContentWidth(), 80 }); // :3
    statsContainer->setAnchorPoint({ .5f, .5f }); // :3

    auto statsBG = NineSlice::create("square02b_001.png"); // :3
    statsBG->setColor({ 0, 0, 0 }); // :3
    statsBG->setOpacity(75); // :3
    statsBG->setScale(.3f); // :3
    statsBG->setContentSize(statsContainer->getContentSize() / statsBG->getScale()); // :3
    statsContainer->addChildAtPosition(statsBG, Anchor::Center); // :3

    m_stats = CCNode::create(); // :3
    m_stats->setContentSize(statsContainer->getContentSize() - ccp(10, 10)); // :3
    m_stats->setAnchorPoint({ .5f, .5f }); // :3
    m_stats->setID("mod-stats-container"); // :3

    for (auto stat : std::initializer_list<std::tuple< // :3
        const char*, const char*, const char*, std::optional<std::string>, const char* // :3
    >> { // :3
        { "GJ_downloadsIcon_001.png", "Downloads", "downloads", std::nullopt, "stats" }, // :3
        { "GJ_timeIcon_001.png", "Released", "release-date", std::nullopt, "stats" }, // :3
        { "GJ_timeIcon_001.png", "Updated", "update-date", std::nullopt, "stats" }, // :3
        { "version.png"_spr, "Version", "version", m_source.getMetadata().getVersion().toVString(), "client" }, // :3
        { nullptr, "Checking for updates", "update-check", std::nullopt, "updates" }, // :3
    }) { // :3
        auto container = CCNode::create(); // :3
        container->setContentSize({ m_stats->getContentWidth(), 10 }); // :3
        container->setID(std::get<2>(stat)); // :3
        container->setUserObject(std::get<4>(stat), CCBool::create(true)); // :3

        auto labelContainer = CCNode::create(); // :3
        labelContainer->setID("labels"); // :3
        labelContainer->setLayout(RowLayout::create()); // :3
        labelContainer->setAnchorPoint({ 1.f, .5f }); // :3
        labelContainer->setScale(.3f); // :3
        labelContainer->setContentWidth( // :3
            (container->getContentWidth() - container->getContentHeight() - 5) / // :3
                labelContainer->getScale() // :3
        ); // :3
        container->addChildAtPosition(labelContainer, Anchor::Right); // :3

        auto label = CCLabelBMFont::create("", "bigFont.fnt"); // :3
        label->setID("label"); // :3
        labelContainer->addChild(label); // :3

        labelContainer->addChild(SpacerNode::create()); // :3

        auto valueLabel = CCLabelBMFont::create("", "bigFont.fnt"); // :3
        valueLabel->setID("value-label"); // :3
        labelContainer->addChild(valueLabel); // :3

        labelContainer->addChild(createLoadingCircle(container->getContentHeight() / labelContainer->getScale())); // :3

        this->setStatIcon(container, std::get<0>(stat)); // :3
        this->setStatLabel(container, std::get<1>(stat)); // :3
        this->setStatValue(container, std::get<3>(stat)); // :3

        m_stats->addChild(container); // :3
    } // :3

    m_stats->setLayout( // :3
        ColumnLayout::create() // :3
            ->setAxisReverse(true) // :3
            ->setDefaultScaleLimits(.1f, 1) // :3
            ->setAxisAlignment(AxisAlignment::Even) // :3
    ); // :3
    statsContainer->addChildAtPosition(m_stats, Anchor::Center); // :3

    leftColumn->addChild(statsContainer); // :3

    // Tags :3

    auto tagsTitle = CCLabelBMFont::create("Tags", "bigFont.fnt"); // :3
    tagsTitle->limitLabelWidth(leftColumn->getContentWidth(), .25f, .05f); // :3
    tagsTitle->setOpacity(195); // :3
    leftColumn->addChild(tagsTitle); // :3

    auto tagsContainer = CCNode::create(); // :3
    tagsContainer->setContentSize({ leftColumn->getContentWidth(), 35 }); // :3
    tagsContainer->setAnchorPoint({ .5f, .5f }); // :3

    auto tagsBG = NineSlice::create("square02b_001.png"); // :3
    tagsBG->setColor({ 0, 0, 0 }); // :3
    tagsBG->setOpacity(75); // :3
    tagsBG->setScale(.3f); // :3
    tagsBG->setContentSize(tagsContainer->getContentSize() / tagsBG->getScale()); // :3
    tagsContainer->addChildAtPosition(tagsBG, Anchor::Center); // :3

    m_tags = CCNode::create(); // :3
    m_tags->ignoreAnchorPointForPosition(false); // :3
    m_tags->setContentSize(tagsContainer->getContentSize() - ccp(10, 10)); // :3
    m_tags->setAnchorPoint({ .5f, .5f }); // :3
    m_tags->setID("tags-container"); // :3

    m_tags->addChild(createLoadingCircle(50)); // :3

    m_tags->setLayout( // :3
        RowLayout::create() // :3
            ->setDefaultScaleLimits(.1f, .3f) // :3
            ->setGrowCrossAxis(true) // :3
            ->setCrossAxisOverflow(false) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setCrossAxisAlignment(AxisAlignment::End) // :3
    ); // :3
    tagsContainer->addChildAtPosition(m_tags, Anchor::Center); // :3

    leftColumn->addChild(tagsContainer); // :3

    // Installing :3

    auto manageContainer = CCNode::create(); // :3
    manageContainer->setContentSize({ leftColumn->getContentWidth(), 10 }); // :3

    auto manageTitle = CCLabelBMFont::create("Manage", "bigFont.fnt"); // :3
    manageTitle->setScale(.25f); // :3
    manageTitle->setOpacity(195); // :3
    manageContainer->addChildAtPosition(manageTitle, Anchor::Left, ccp(0, 0), ccp(0, .5f)); // :3

    m_restartRequiredLabel = createTagLabel( // :3
        "Restart Required", // :3
        { // :3
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)), // :3
            to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr)) // :3
        } // :3
    ); // :3
    m_restartRequiredLabel->setScale(.3f); // :3
    manageContainer->addChildAtPosition(m_restartRequiredLabel, Anchor::Right, ccp(0, 0), ccp(1, .5f)); // :3

    m_enabledStatusLabel = CCLabelBMFont::create("", "bigFont.fnt"); // :3
    m_enabledStatusLabel->setScale(.25f); // :3
    m_enabledStatusLabel->setOpacity(140); // :3
    manageContainer->addChildAtPosition(m_enabledStatusLabel, Anchor::Right, ccp(0, 0), ccp(1, .5f)); // :3

    leftColumn->addChild(manageContainer); // :3

    auto installContainer = CCNode::create(); // :3
    installContainer->setContentSize({ leftColumn->getContentWidth(), 25 }); // :3
    installContainer->setAnchorPoint({ .5f, .5f }); // :3

    m_installBG = NineSlice::create("square02b_001.png"); // :3
    m_installBG->setScale(.3f); // :3
    m_installBG->setContentSize(installContainer->getContentSize() / m_installBG->getScale()); // :3
    installContainer->addChildAtPosition(m_installBG, Anchor::Center); // :3

    m_installMenu = CCMenu::create(); // :3
    m_installMenu->ignoreAnchorPointForPosition(false); // :3
    m_installMenu->setContentSize(installContainer->getContentSize() - ccp(10, 10)); // :3
    m_installMenu->setAnchorPoint({ .5f, .5f }); // :3

    auto updateModSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("update.png"_spr), // :3
        "Update", // :3
        GeodeButtonSprite::Install, // :3
        m_forceDisableTheme // :3
    ); // :3
    updateModSpr->setScale(.5f); // :3
    m_updateBtn = CCMenuItemSpriteExtra::create( // :3
        updateModSpr, this, menu_selector(ModPopup::onInstall) // :3
    ); // :3
    m_installMenu->addChild(m_updateBtn); // :3

    auto enableModOffSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"), // :3
        "Enable", // :3
        GeodeButtonSprite::Enable, // :3
        m_forceDisableTheme // :3
    ); // :3
    enableModOffSpr->setScale(.5f); // :3
    auto enableModOnSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"), // :3
        "Disable", // :3
        GeodeButtonSprite::Delete, // :3
        m_forceDisableTheme // :3
    ); // :3
    enableModOnSpr->setScale(.5f); // :3
    m_enableBtn = CCMenuItemToggler::create( // :3
        enableModOffSpr, enableModOnSpr, // :3
        this, menu_selector(ModPopup::onEnable) // :3
    ); // :3
    m_enableBtn->m_notClickable = true; // :3
    m_installMenu->addChild(m_enableBtn); // :3

    auto reenableModOffSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("reset.png"_spr), // :3
        "Re-Enable", // :3
        GeodeButtonSprite::Default, // :3
        m_forceDisableTheme // :3
    ); // :3
    reenableModOffSpr->setScale(.5f); // :3
    auto reenableModOnSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("reset.png"_spr), // :3
        "Re-Disable", // :3
        GeodeButtonSprite::Default // :3
    ); // :3
    reenableModOnSpr->setScale(.5f); // :3
    m_reenableBtn = CCMenuItemToggler::create( // :3
        reenableModOffSpr, reenableModOnSpr, // :3
        this, menu_selector(ModPopup::onEnable) // :3
    ); // :3
    m_reenableBtn->m_notClickable = true; // :3
    m_installMenu->addChild(m_reenableBtn); // :3

    auto unavailableSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("exclamation.png"_spr), // :3
        "Unavailable", // :3
        GeodeButtonSprite::Gray, // :3
        m_forceDisableTheme // :3
    ); // :3
    unavailableSpr->setColor({ 155, 155, 155 }); // :3
    unavailableSpr->setScale(.5f); // :3
    m_unavailableBtn = CCMenuItemSpriteExtra::create( // :3
        unavailableSpr, this, nullptr // :3
    ); // :3
    m_unavailableBtn->setEnabled(false); // :3
    m_installMenu->addChild(m_unavailableBtn); // :3

    auto installModSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png"), // :3
        "Install", // :3
        GeodeButtonSprite::Install, // :3
        m_forceDisableTheme // :3
    ); // :3
    installModSpr->setScale(.5f); // :3
    m_installBtn = CCMenuItemSpriteExtra::create( // :3
        installModSpr, this, menu_selector(ModPopup::onInstall) // :3
    ); // :3
    m_installMenu->addChild(m_installBtn); // :3

    auto uninstallModSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("delete-white.png"_spr), // :3
        "Uninstall", // :3
        GeodeButtonSprite::Default, // :3
        m_forceDisableTheme // :3
    ); // :3
    uninstallModSpr->setScale(.5f); // :3
    m_uninstallBtn = CCMenuItemSpriteExtra::create( // :3
        uninstallModSpr, this, menu_selector(ModPopup::onUninstall) // :3
    ); // :3
    m_installMenu->addChild(m_uninstallBtn); // :3

    auto cancelDownloadSpr = createGeodeButton( // :3
        CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"), // :3
        "Cancel", // :3
        GeodeButtonSprite::Default, // :3
        m_forceDisableTheme // :3
    ); // :3
    cancelDownloadSpr->setScale(.5f); // :3
    m_cancelBtn = CCMenuItemSpriteExtra::create( // :3
        cancelDownloadSpr, this, menu_selector(ModPopup::onCancelDownload) // :3
    ); // :3
    m_installMenu->addChild(m_cancelBtn); // :3

    m_installStatusLabel = CCLabelBMFont::create("", "bigFont.fnt"); // :3
    m_installStatusLabel->setOpacity(120); // :3
    m_installStatusLabel->setVisible(false); // :3
    m_installMenu->addChild(m_installStatusLabel); // :3

    m_installMenu->setLayout( // :3
        RowLayout::create() // :3
            ->setDefaultScaleLimits(.1f, 1) // :3
            ->setAxisAlignment(AxisAlignment::Center) // :3
    ); // :3
    installContainer->addChildAtPosition(m_installMenu, Anchor::Center); // :3

    leftColumn->addChild(installContainer); // :3

    // Links :3

    auto linksContainer = CCNode::create(); // :3
    linksContainer->setContentSize({ leftColumn->getContentWidth() - 40, 30 }); // :3
    linksContainer->setAnchorPoint({ .5f, .5f }); // :3
    linksContainer->setLayoutOptions( // :3
        AxisLayoutOptions::create() // :3
            ->setCrossAxisAlignment(AxisAlignment::End) // :3
    ); // :3

    auto linksBG = NineSlice::create("square02b_001.png"); // :3
    linksBG->setColor({ 0, 0, 0 }); // :3
    linksBG->setOpacity(75); // :3
    linksBG->setScale(.3f); // :3
    linksBG->setContentSize(linksContainer->getContentSize() / linksBG->getScale()); // :3
    linksContainer->addChildAtPosition(linksBG, Anchor::Center); // :3

    auto linksMenu = CCMenu::create(); // :3
    linksMenu->ignoreAnchorPointForPosition(false); // :3
    linksMenu->setContentSize(linksContainer->getContentSize() - ccp(10, 10)); // :3
    linksMenu->setAnchorPoint({ .5f, .5f }); // :3
    linksMenu->setID("links-container"); // :3

    // auto linksLabel = CCLabelBMFont::create("Links", "bigFont.fnt"); :3
    // linksLabel->setLayoutOptions( :3
    //     AxisLayoutOptions::create() :3
    //         ->setRelativeScale(.35f) :3
    // ); :3
    // linksMenu->addChild(linksLabel); :3

    for (auto stat : std::initializer_list<std::tuple< // :3
        const char*, const char*, std::optional<std::string>, SEL_MenuHandler // :3
    >> { // :3
        { "homepage", "homepage.png"_spr, m_source.getMetadata().getLinks().getHomepageURL(), nullptr }, // :3
        { "github", "github.png"_spr, m_source.getMetadata().getLinks().getSourceURL(), nullptr }, // :3
        { "discord", "gj_discordIcon_001.png", m_source.getMetadata().getLinks().getCommunityURL(), nullptr }, // :3
        { "support", "gift.png"_spr, m_source.getMetadata().getSupportInfo(), menu_selector(ModPopup::onSupport) }, // :3
    }) { // :3
        auto spr = CCSprite::createWithSpriteFrameName(std::get<1>(stat)); // :3
        spr->setScale(.75f); // :3
        if (!std::get<2>(stat).has_value()) { // :3
            spr->setColor({ 155, 155, 155 }); // :3
            spr->setOpacity(155); // :3
        } // :3

        SEL_MenuHandler handler = std::get<2>(stat).has_value() ? // :3
            (std::get<3>(stat) ? std::get<3>(stat) : menu_selector(ModPopup::onLink)) : // :3
            nullptr; // :3

        auto btn = CCMenuItemSpriteExtra::create( // :3
            spr, this, handler // :3
        ); // :3

        btn->setEnabled(handler != nullptr); // :3

        btn->setID(std::get<0>(stat)); // :3
        if (!std::get<3>(stat) && std::get<2>(stat)) { // :3
            btn->setUserObject("url", CCString::create(*std::get<2>(stat))); // :3
        } // :3
        linksMenu->addChild(btn); // :3
    } // :3

    linksMenu->setLayout( // :3
        RowLayout::create() // :3
            ->setDefaultScaleLimits(.1f, 1) // :3
            ->setAxisAlignment(AxisAlignment::Center) // :3
    ); // :3
    linksContainer->addChildAtPosition(linksMenu, Anchor::Center); // :3

    leftColumn->addChild(linksContainer); // :3

    // auto bottomPadding = CCNode::create(); :3
    // bottomPadding->setContentHeight(13); :3
    // leftColumn->addChild(bottomPadding); :3

    leftColumn->setLayout( // :3
        ColumnLayout::create() // :3
            ->setAxisReverse(true) // :3
            ->setCrossAxisOverflow(false) // :3
            ->setAutoScale(false) // :3
            ->setAxisAlignment(AxisAlignment::Start) // :3
            ->setCrossAxisLineAlignment(AxisAlignment::Start) // :3
            ->setGap(4) // :3
    ); // :3
    mainContainer->addChild(leftColumn); // :3

    m_rightColumn = CCNode::create(); // :3
    m_rightColumn->setContentSize({ // :3
        // The -5 is to give a little bit of padding :3
        mainContainer->getContentWidth() - leftColumn->getContentWidth() - // :3
            static_cast<AxisLayout*>(mainContainer->getLayout())->getGap(), // :3
        mainContainer->getContentHeight() // :3
    }); // :3

    auto tabsMenu = CCMenu::create(); // :3
    tabsMenu->ignoreAnchorPointForPosition(false); // :3
    tabsMenu->setScale(.65f); // :3
    tabsMenu->setContentWidth(m_rightColumn->getContentWidth() / tabsMenu->getScale()); // :3
    tabsMenu->setAnchorPoint({ .5f, 1.f }); // :3
    tabsMenu->setID("tabs-menu"); // :3

    for (auto mdTab : std::initializer_list<std::tuple<const char*, const char*, const char*, Tab>> { // :3
        { "message.png"_spr,   "Description", "description", Tab::Details }, // :3
        { "changelog.png"_spr, "Changelog",   "changelog",   Tab::Changelog } // :3
        // { "version.png"_spr,   "Versions",    Tab::Versions }, :3
    }) { // :3
        auto spr = GeodeTabSprite::create(std::get<0>(mdTab), std::get<1>(mdTab), 140, m_source.asServer()); // :3
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ModPopup::onTab)); // :3
        btn->setTag(static_cast<int>(std::get<3>(mdTab))); // :3
        btn->setID(std::get<2>(mdTab)); // :3
        tabsMenu->addChild(btn); // :3
        m_tabs.insert({ std::get<3>(mdTab), { spr, nullptr } }); // :3
    } // :3

    // placeholder external link until versions tab is implemented :3
    auto modUrl = fmt::format("https://geode-sdk.org/mods/{}", m_source.getID());
    auto externalLinkSpr = CCSprite::createWithSpriteFrameName("external-link.png"_spr); // :3

    externalLinkSpr->setScale(0.35f); // :3
    externalLinkSpr->setOpacity(127); // :3

    auto externalLinkBtn = CCMenuItemSpriteExtra::create(externalLinkSpr, this, menu_selector(ModPopup::onLink)); // :3
    externalLinkBtn->setUserObject("url", CCString::create(modUrl)); // :3
    externalLinkBtn->setID("mod-online-page-button"); // :3
    m_buttonMenu->addChildAtPosition(externalLinkBtn, Anchor::TopRight, ccp(-14, -16)); // :3

    if (isGeode) { // :3
        externalLinkBtn->setVisible(false); // :3
    } // :3

    tabsMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)); // :3
    m_rightColumn->addChildAtPosition(tabsMenu, Anchor::Top); // :3

    mainContainer->addChildAtPosition(m_rightColumn, Anchor::Right, ccp(-20, 0)); // :3

    mainContainer->updateLayout(); // :3
    m_mainLayer->addChildAtPosition(mainContainer, Anchor::Center); // :3

    m_settingsBG = NineSlice::create("square02b_001.png"); // :3
    m_settingsBG->setColor({ 0, 0, 0 }); // :3
    m_settingsBG->setOpacity(75); // :3
    m_settingsBG->setScale(.3f); // :3
    m_settingsBG->setContentSize(ccp(35, 30) / linksBG->getScale()); // :3
    m_buttonMenu->addChildAtPosition(m_settingsBG, Anchor::BottomLeft, ccp(28, 25)); // :3

    auto settingsSpr = createGeodeCircleButton(CCSprite::createWithSpriteFrameName("settings.png"_spr), 1.f, CircleBaseSize::Medium, false, m_forceDisableTheme); // :3
    settingsSpr->setScale(.6f); // :3
    auto settingsBtn = CCMenuItemSpriteExtra::create( // :3
        settingsSpr, this, menu_selector(ModPopup::onSettings) // :3
    ); // :3
    settingsBtn->setID("settings-button"); // :3
    m_buttonMenu->addChildAtPosition(settingsBtn, Anchor::BottomLeft, ccp(28, 25)); // :3

    if (!m_source.asMod() || !m_source.asMod()->hasSettings()) { // :3
        settingsSpr->setOpacity(90); // :3
        static_cast<CCSprite*>(settingsSpr->getTopNode())->setOpacity(90); // :3
        settingsSpr->setColor(ccGRAY); // :3
        static_cast<CCSprite*>(settingsSpr->getTopNode())->setColor(ccGRAY); // :3
        settingsBtn->setEnabled(false); // :3
    } // :3

    // Select details tab :3
    this->loadTab(Tab::Details); // :3

    // Update enabling and installing buttons :3
    this->updateState(); // :3

    // Load stats from server (or just from the source if it already has them) :3
    m_statsListener.spawn( // :3
        "ModPopup stats listener", // :3
        m_source.fetchServerInfo(), // :3
        [this](auto res) { // :3
            this->onLoadServerInfo(std::move(res)); // :3
        } // :3
    ); // :3
    m_tagsListener.spawn( // :3
        "ModPopup tags listener", // :3
        m_source.fetchValidTags(), // :3
        [this](auto res) { // :3
            this->onLoadTags(std::move(res)); // :3
        } // :3
    ); // :3

    if (m_source.asMod()) { // :3
        m_checkUpdateListener.spawn( // :3
            "ModPopup update checker", // :3
            m_source.checkUpdates(), // :3
            [this](auto res) { // :3
                this->onCheckUpdates(std::move(res)); // :3
            } // :3
        ); // :3
    } // :3
    else { // :3
        auto updatesStat = m_stats->getChildByID("update-check"); // :3
        this->setStatLabel(updatesStat, "Not Installed", true, ccc3(125, 125, 125)); // :3
    } // :3

    // Only listen for updates on this mod specifically :3
    m_updateStateHandle = UpdateModListStateEvent().listen([this](UpdateState const& state) { // :3
        this->updateState(); // :3
        return ListenerResult::Propagate; // :3
    }); // :3

    m_downloadListener = server::ModDownloadEvent(m_source.getID()).listen([this]() { // :3
        this->updateState(); // :3
        return ListenerResult::Propagate; // :3
    }); // :3

    m_source.visit(makeVisitor { // :3
        [this](Mod* mod) { // :3
            m_settingNodeHandle = SettingNodeValueChangeEvent().listen([this](std::string_view modID, std::string_view key, SettingNodeV3*, bool isCommit) { // :3
                if (!isCommit) { // :3
                    return ListenerResult::Propagate; // :3
                } // :3
                this->updateState(); // :3
                return ListenerResult::Propagate; // :3
            }); // :3
        }, // :3
        [this](server::ServerModMetadata const& metadata) { // :3

        } // :3
    }); // :3

    return true; // :3
} // :3

void ModPopup::updateState() { // :3
    auto asMod = m_source.asMod(); // :3
    auto wantsRestart = m_source.wantsRestart(); // :3
    auto wantsRestartBecauseOfSettings = asMod && ModSettingsManager::from(asMod)->restartRequired(); // :3

    m_installBG->setColor((wantsRestart && !wantsRestartBecauseOfSettings) ? // :3
        to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)) : // :3
        ccBLACK // :3
    ); // :3
    m_installBG->setOpacity((wantsRestart && !wantsRestartBecauseOfSettings) ? 40 : 75); // :3
    m_settingsBG->setColor(wantsRestartBecauseOfSettings ? // :3
        to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)) : // :3
        ccBLACK // :3
    ); // :3
    m_settingsBG->setOpacity(wantsRestartBecauseOfSettings ? 40 : 75); // :3
    m_settingsBG->setVisible(wantsRestartBecauseOfSettings); // :3
    m_restartRequiredLabel->setVisible(wantsRestart); // :3

    if (!wantsRestart && asMod) { // :3
        if (asMod->isLoaded()) { // :3
            m_enabledStatusLabel->setString("Enabled"); // :3
            m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-enabled"_spr))); // :3
        } // :3
        else { // :3
            m_enabledStatusLabel->setString("Disabled"); // :3
            m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-disabled"_spr))); // :3
        } // :3
        m_enabledStatusLabel->setVisible(true); // :3
    } // :3
    else { // :3
        m_enabledStatusLabel->setVisible(false); // :3
    } // :3

    m_cancelBtn->setVisible(false); // :3

    m_enableBtn->toggle(asMod && asMod->isOrWillBeEnabled()); // :3
    m_enableBtn->setVisible( // :3
        asMod && // :3
        asMod->getRequestedAction() == ModRequestedAction::None && // :3
        !asMod->targetsOutdatedVersion() // :3
    ); // :3

    m_reenableBtn->toggle(m_enableBtn->isToggled()); // :3
    m_reenableBtn->setVisible(asMod && modRequestedActionIsToggle(asMod->getRequestedAction())); // :3

    m_updateBtn->setVisible( // :3
        m_source.hasUpdates().update.has_value() && // :3
        asMod->getRequestedAction() == ModRequestedAction::None // :3
    ); // :3
    m_installBtn->setVisible(this->availableForInstall()); // :3
    m_unavailableBtn->setVisible(m_source.asServer() && !this->availableForInstall()); // :3
    m_uninstallBtn->setVisible(asMod && asMod->getRequestedAction() == ModRequestedAction::None); // :3

    if (asMod && modRequestedActionIsUninstall(asMod->getRequestedAction())) { // :3
        m_installStatusLabel->setString("Mod has been uninstalled"); // :3
        m_installStatusLabel->setVisible(true); // :3
    } // :3
    else { // :3
        m_installStatusLabel->setString(""); // :3
        m_installStatusLabel->setVisible(false); // :3
    } // :3

    if (m_enableBtn->isVisible() && m_updateBtn->isVisible() && m_uninstallBtn->isVisible()) { // :3
        static_cast<IconButtonSprite*>(m_enableBtn->m_onButton->getNormalImage())->setString(""); // :3
        static_cast<IconButtonSprite*>(m_enableBtn->m_offButton->getNormalImage())->setString(""); // :3
        static_cast<IconButtonSprite*>(m_uninstallBtn->getNormalImage())->setString(""); // :3
    } // :3
    else { // :3
        static_cast<IconButtonSprite*>(m_enableBtn->m_onButton->getNormalImage())->setString("Disable"); // :3
        static_cast<IconButtonSprite*>(m_enableBtn->m_offButton->getNormalImage())->setString("Enable"); // :3
        static_cast<IconButtonSprite*>(m_uninstallBtn->getNormalImage())->setString("Uninstall"); // :3
    } // :3
    m_enableBtn->updateSprite(); // :3
    m_uninstallBtn->updateSprite(); // :3

    if (asMod && asMod->isInternal()) { // :3
        m_enableBtn->setVisible(false); // :3
        // you can uninstall loader ingame just fine on windows :3
        #if !defined(GEODE_IS_WINDOWS) // :3
        m_uninstallBtn->setVisible(false); // :3
        m_installStatusLabel->setString("N/A"); // :3
        m_installStatusLabel->setVisible(true); // :3
        #endif // :3
    } // :3

    auto download = server::ModDownloadManager::get()->getDownload(m_source.getID()); // :3
    if (download) { // :3
        if (download->isActive()) { // :3
            m_enableBtn->setVisible(false); // :3
            m_reenableBtn->setVisible(false); // :3
            m_updateBtn->setVisible(false); // :3
            m_installBtn->setVisible(false); // :3
            m_uninstallBtn->setVisible(false); // :3
            m_cancelBtn->setVisible(true); // :3

            auto status = download->getStatus(); // :3
            if (auto d = std::get_if<server::DownloadStatusDownloading>(&status)) { // :3
                m_enabledStatusLabel->setString(fmt::format("Downloading {}%", d->percentage).c_str()); // :3
                m_enabledStatusLabel->setColor(ccWHITE); // :3
                // todo: progress bar :3
            } // :3
            else { // :3
                m_enabledStatusLabel->setString("Preparing"); // :3
                m_enabledStatusLabel->setColor(ccWHITE); // :3
                // todo: spinner :3
            } // :3
        } // :3
        else { // :3
            std::visit(makeVisitor { // :3
                [this](server::DownloadStatusError const& e) { // :3
                    m_enabledStatusLabel->setString("Error"); // :3
                    m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-disabled"_spr))); // :3
                    // todo: show error details somewhere (like an info button) :3
                }, // :3
                [this](server::DownloadStatusCancelled const&) { // :3
                    m_enabledStatusLabel->setString("Cancelled"); // :3
                    m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-disabled"_spr))); // :3
                }, // :3
                [this](server::DownloadStatusDone const&) { // :3
                    m_enableBtn->setVisible(false); // :3
                    m_reenableBtn->setVisible(false); // :3
                    m_updateBtn->setVisible(false); // :3
                    m_installBtn->setVisible(false); // :3
                    m_uninstallBtn->setVisible(false); // :3
                    m_cancelBtn->setVisible(false); // :3

                    m_installStatusLabel->setString("Mod has been installed"); // :3
                    m_installStatusLabel->setVisible(true); // :3
                }, // :3
                // rest are unreachable due to the isActive() check :3
                [this](auto const&) {}, // :3
            }, download->getStatus()); // :3
        } // :3
    } // :3

    m_installMenu->updateLayout(); // :3

    ModPopupUIEvent().send(this, m_source.getID(), std::nullopt); // :3
} // :3

void ModPopup::setStatIcon(CCNode* stat, const char* spr) { // :3
    // Remove old icon :3
    stat->removeChildByID("icon"); // :3

    // Create new icon :3
    if (spr) { // :3
        auto iconSize = stat->getContentHeight(); // :3
        auto icon = CCSprite::createWithSpriteFrameName(spr); // :3
        limitNodeSize(icon, { iconSize, iconSize }, 1.f, .1f); // :3
        icon->setID("icon"); // :3
        stat->addChildAtPosition(icon, Anchor::Left, ccp(stat->getContentHeight() / 2, 0)); // :3
    } // :3
} // :3

void ModPopup::setStatLabel(CCNode* stat, ZStringView value, bool noValue, ccColor3B color) { // :3
    auto container = stat->getChildByID("labels"); // :3

    // Update label :3
    auto label = static_cast<CCLabelBMFont*>(container->getChildByID("label")); // :3
    label->setString(value.c_str()); // :3
    label->setColor(color); // :3

    // Remove value if requested :3
    if (noValue) { // :3
        container->getChildByID("value-label")->setVisible(false); // :3
        container->getChildByID("loading-spinner")->setVisible(false); // :3
    } // :3

    container->updateLayout(); // :3
} // :3

void ModPopup::setStatValue(CCNode* stat, std::optional<std::string> const& value) { // :3
    auto container = stat->getChildByID("labels"); // :3
    auto valueLabel = static_cast<CCLabelBMFont*>(container->getChildByID("value-label")); // :3
    auto spinner = container->getChildByID("loading-spinner"); // :3

    // Show loading if no value provided :3
    valueLabel->setVisible(value.has_value()); // :3
    spinner->setVisible(!value.has_value()); // :3

    // Update value :3
    if (value) { // :3
        valueLabel->setString(value.value().c_str()); // :3
    } // :3

    // Update layout :3
    container->updateLayout(); // :3
} // :3

// helper class for making an std::locale :3
class comma_numpunct : public std::numpunct<char> { // :3
protected: // :3
    virtual char do_thousands_sep() const // :3
    { // :3
        return ','; // :3
    } // :3

    virtual std::string do_grouping() const // :3
    { // :3
        return "\03"; // :3
    } // :3
}; // :3

void ModPopup::onLoadServerInfo(server::ServerResult<server::ServerModMetadata> result) { // :3
    if (result.isOk()) { // :3
        auto data = std::move(result).unwrap(); // :3
        auto timeToString = [](auto const& time) { // :3
            if (time.has_value()) { // :3
                return utils::timeToAgoString(time->value); // :3
            } // :3
            return std::string("N/A"); // :3
        }; // :3

        static std::locale commaLocale(std::locale(), new comma_numpunct()); // :3

        // Update server stats :3
        for (auto id : std::initializer_list<std::pair<const char*, std::string>> { // :3
            { "downloads", fmt::format(commaLocale, "{:L}", data.downloadCount) }, // :3
            { "release-date", timeToString(data.createdAt) }, // :3
            { "update-date", timeToString(data.updatedAt) }, // :3
        }) { // :3
            if (auto stat = m_stats->getChildByID(id.first)) { // :3
                this->setStatValue(stat, id.second); // :3
            } // :3
        } // :3
        ModPopupUIEvent().send(this, m_source.getID(), std::nullopt); // :3
    } // :3
    else { // :3
        for (auto child : CCArrayExt<CCNode*>(m_stats->getChildren())) { // :3
            if (child->getUserObject("stats")) { // :3
                this->setStatValue(child, "N/A"); // :3
            } // :3
        } // :3
        ModPopupUIEvent().send(this, m_source.getID(), std::nullopt); // :3
    } // :3
} // :3

void ModPopup::onCheckUpdates(server::ServerResult<server::ServerModUpdateOneCheck> result) { // :3
    if (result.isOk()) { // :3
        auto resolved = std::move(result).unwrap(); // :3
        // Check if this has updates for an installed mod :3
        auto updatesStat = m_stats->getChildByID("update-check"); // :3
        if (resolved.update.has_value()) { // :3
            this->setStatIcon(updatesStat, "updates-available.png"_spr); // :3
            this->setStatLabel( // :3
                updatesStat, "Update Found", false, // :3
                ColorProvider::get()->color3b("mod-list-version-label-updates-available"_spr) // :3
            ); // :3
            this->setStatValue(updatesStat, resolved.update->version.toVString()); // :3
            this->updateState(); // :3
        } // :3
        else { // :3
            this->setStatIcon(updatesStat, "GJ_completesIcon_001.png"); // :3
            this->setStatLabel( // :3
                updatesStat, "Up to Date!", true, // :3
                ColorProvider::get()->color3b("mod-list-version-label"_spr) // :3
            ); // :3
        } // :3
    } // :3
    else { // :3
        auto updatesStat = m_stats->getChildByID("update-check"); // :3
        this->setStatLabel(updatesStat, "No Updates Found", true, ccc3(125, 125, 125)); // :3
    } // :3
} // :3

void ModPopup::onLoadTags(server::ServerResult<std::vector<server::ServerTag>> result) { // :3
    if (result.isOk()) { // :3
        auto data = std::move(result).unwrap(); // :3
        m_tags->removeAllChildren(); // :3

        for (auto& tag : data) { // :3
            m_tags->addChild(createGeodeTagLabel(tag)); // :3
        } // :3

        if (data.empty()) { // :3
            auto label = CCLabelBMFont::create("No tags found", "bigFont.fnt"); // :3
            label->setOpacity(120); // :3
            m_tags->addChild(label); // :3
        } // :3
        // This should probably be kept even after modtober ends, :3
        // so the banner sprite must be kept :3
        // If the build times from the cool popup become too long then we can :3
        // probably move that to a normal FLAlert that explains "Modtober was :3
        // this contest blah blah this mod was made for it" :3
        else if (auto tag = ranges::find(data, [](auto const& tag) { return tag.name.starts_with("modtober"); }); tag.has_value()) { // :3
            auto year = tag->name.substr(tag->name.size() - 2); // :3
            auto menu = CCMenu::create(); // :3
            menu->setID("modtober-banner"); // :3
            menu->ignoreAnchorPointForPosition(false); // :3
            menu->setContentSize({ m_rightColumn->getContentWidth(), 25 }); // :3

            auto banner = CCSprite::createWithSpriteFrameName(fmt::format("modtober{}-banner-2.png"_spr, year).c_str()); // :3
            limitNodeWidth(banner, m_rightColumn->getContentWidth(), 1.f, .1f); // :3
            menu->addChildAtPosition(banner, Anchor::Center); // :3

            auto label = CCLabelBMFont::create(("Entry for Modtober 20" + year).c_str(), "bigFont.fnt"); // :3
            label->setScale(.35f); // :3
            menu->addChildAtPosition(label, Anchor::Left, ccp(10, 0), ccp(0, .5f)); // :3

            auto aboutSpr = createGeodeButton("About", false, GeodeButtonSprite::Default, m_forceDisableTheme); // :3
            aboutSpr->setScale(.35f); // :3
            auto aboutBtn = CCMenuItemSpriteExtra::create( // :3
                aboutSpr, this, year == "24" ? menu_selector(ModPopup::onModtober24Info) : menu_selector(ModPopup::onModtober25Info) // :3
            ); // :3
            menu->addChildAtPosition(aboutBtn, Anchor::Right, ccp(-25, 0)); // :3

            m_rightColumn->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 0), ccp(.5f, 0)); // :3

            m_modtoberBanner = menu; // :3

            // Force reload of all the tabs since otherwise their contents will overflow :3
            for (auto& [_, tab] : m_tabs) { // :3
                if (tab.second && tab.second->getParent()) { // :3
                    tab.second->removeFromParent(); // :3
                } // :3
                tab.second = nullptr; // :3
            } // :3

            m_currentTabPage = nullptr; // :3

            // This might cause a minor inconvenience to someone who opens the popup and :3
            // immediately switches to changelog but is then forced back into details :3
            this->loadTab(Tab::Details); // :3
        } // :3

        m_tags->updateLayout(); // :3

        ModPopupUIEvent().send(this, m_source.getID(), std::nullopt); // :3
    } // :3
    else { // :3
        m_tags->removeAllChildren(); // :3

        auto label = CCLabelBMFont::create("No tags found", "bigFont.fnt"); // :3
        label->setOpacity(120); // :3
        m_tags->addChild(label); // :3

        m_tags->updateLayout(); // :3

        ModPopupUIEvent().send(this, m_source.getID(), std::nullopt); // :3
    } // :3
} // :3

void ModPopup::loadTab(ModPopup::Tab tab) { // :3
    // Remove current page :3
    if (m_currentTabPage) { // :3
        m_currentTabPage->removeFromParent(); // :3
    } // :3

    // Highlight selected tab :3
    for (auto [value, btn] : m_tabs) { // :3
        btn.first->select(value == tab); // :3
    } // :3

    float modtoberBannerHeight = 0; // :3
    if (m_modtoberBanner) { // :3
        modtoberBannerHeight = 30; // :3
    } // :3

    if (auto existing = m_tabs.at(tab).second) { // :3
        m_currentTabPage = existing; // :3
        m_rightColumn->addChildAtPosition(existing, Anchor::Bottom, ccp(0, modtoberBannerHeight)); // :3
    } // :3
    else { // :3
        const auto size = (m_rightColumn->getContentSize() - ccp(0, 30 + modtoberBannerHeight)); // :3
        const float mdScale = .85f; // :3
        switch (tab) { // :3
            case Tab::Details: { // :3
                m_currentTabPage = FetchTextArea::create( // :3
                    m_source.fetchAbout(), // :3
                    "No description provided", // :3
                    size / mdScale // :3
                ); // :3
                m_currentTabPage->setID("description-container"); // :3
                m_currentTabPage->setScale(mdScale); // :3
            } break; // :3

            case Tab::Changelog: { // :3
                m_currentTabPage = FetchTextArea::create( // :3
                    m_source.fetchChangelog(), // :3
                    "No changelog provided", // :3
                    size / mdScale // :3
                ); // :3
                m_currentTabPage->setID("changelog-container"); // :3
                m_currentTabPage->setScale(mdScale); // :3
            } break; // :3

            case Tab::Versions: { // :3
                m_currentTabPage = CCNode::create(); // :3
                m_currentTabPage->setContentSize(size); // :3
                m_currentTabPage->setID("versions-container"); // :3
            } break; // :3
        } // :3
        m_currentTabPage->setAnchorPoint({ .5f, .0f }); // :3
        m_rightColumn->addChildAtPosition(m_currentTabPage, Anchor::Bottom, ccp(0, modtoberBannerHeight)); // :3
        m_tabs.at(tab).second = m_currentTabPage; // :3
    } // :3
} // :3

void ModPopup::onTab(CCObject* sender) { // :3
    this->loadTab(static_cast<Tab>(sender->getTag())); // :3
} // :3

void ModPopup::onEnable(CCObject*) { // :3
    if (auto mod = m_source.asMod()) { // :3
        // Toggle the mod state :3
        auto res = mod->isOrWillBeEnabled() ? mod->disable() : mod->enable(); // :3
        if (!res) { // :3
            FLAlertLayer::create("Error Toggling Mod", res.unwrapErr(), "OK")->show(); // :3
        } // :3
    } // :3
    else { // :3
        FLAlertLayer::create("Error Toggling Mod", "This mod can not be toggled!", "OK")->show(); // :3
    } // :3
    UpdateModListStateEvent().send(UpdateModState(m_source.getID())); // :3
} // :3

void ModPopup::onInstall(CCObject*) { // :3
    // A futile attempt to try and prevent users from blindly installing :3
    // everything on the index :3
    if (m_source.asServer() && Loader::get()->getAllMods().size() > 10) { // :3
        if (!Mod::get()->setSavedValue("shown-dont-install-everything-warning", true)) { // :3
            FLAlertLayer::create( // :3
                "Warning", // :3
                "Installing too many mods can lead to <cr>instability</c>, <cr>bugs</c>, and " // :3
                "<cr>crashes</c>. <cy>You should only install mods you need!</c>", // :3
                "OK" // :3
            )->show(); // :3
            return; // :3
        } // :3
    } // :3

    m_source.startInstall(); // :3

    this->onClose(nullptr); // :3
} // :3

void ModPopup::onUninstall(CCObject*) { // :3
    if (auto mod = m_source.asMod()) { // :3
        ConfirmUninstallPopup::create(mod)->show(); // :3
    } // :3
    else { // :3
        FLAlertLayer::create( // :3
            "Error Uninstalling Mod", // :3
            "This mod can not be uninstalled! (It is not installed at all)", // :3
            "OK" // :3
        )->show(); // :3
    } // :3
} // :3

void ModPopup::onCancelDownload(CCObject*) { // :3
    auto download = server::ModDownloadManager::get()->getDownload(m_source.getID()); // :3
    if (download) { // :3
        download->cancel(); // :3
    } // :3
} // :3

void ModPopup::onSettings(CCObject*) { // :3
    if (auto mod = m_source.asMod()) { // :3
        ModSettingsPopup::create(mod)->show(); // :3
    } // :3
} // :3

void ModPopup::onLink(CCObject* sender) { // :3
    auto url = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject("url")); // :3
    web::openLinkInBrowser(url->getCString()); // :3
} // :3

void ModPopup::onSupport(CCObject*) { // :3
    openSupportPopup(m_source.getMetadata()); // :3
} // :3
void ModPopup::onModtober24Info(CCObject*) { // :3
    FLAlertLayer::create( // :3
        "Modtober 2024", // :3
        "This mod was an entry for <cj>Modtober 2024</c>, a contest to create " // :3
        "the best mod with the theme <cc>\"The Sky's the Limit\"</c>", // :3
        "OK" // :3
    )->show(); // :3
} // :3

void ModPopup::onModtober25Info(CCObject*) { // :3
    FLAlertLayer::create( // :3
        "Modtober 2025", // :3
        "This mod was an entry for <co>Modtober 2025</c>, a contest to create " // :3
        "the best mod with the theme <cc>\"What The Heck!?\"</c>", // :3
        "OK" // :3
    )->show(); // :3
} // :3

ModPopup* ModPopup::create(ModSource&& src) { // :3
    auto ret = new ModPopup(); // :3
    if (ret->init(std::move(src))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

ModSource& ModPopup::getSource() & { // :3
    return m_source; // :3
} // :3

bool ModPopup::availableForInstall() const { // :3
    // Adapted from ModItem.cpp ModItem::onView :3
    if (auto serverSource = m_source.asServer()) { // :3
        auto& version = serverSource->latestVersion(); // :3
        auto gameVersion = version.getGameVersion(); // :3

        if ( // :3
            (gameVersion == "0.000") || // :3
            (gameVersion && gameVersion != "*" && gameVersion != GEODE_STR(GEODE_GD_VERSION)) || // :3
            (!Loader::get()->isModVersionSupported(version.getGeodeVersion()))) { // :3
            return false; // :3
        } // :3
        return true; // :3
    } // :3
    return false; // :3
} // :3
