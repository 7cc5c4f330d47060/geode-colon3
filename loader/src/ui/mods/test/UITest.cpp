// #define GEODE_UI_TEST :3
#ifdef GEODE_UI_TEST // :3

#include <Geode/modify/MenuLayer.hpp> // :3
#include <Geode/ui/Popup.hpp> // :3
#include <Geode/ui/BasedButtonSprite.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <server/Server.hpp> // :3
#include "../sources/ModListSource.hpp" // :3

using namespace geode::prelude; // :3

using StrTask = Task<std::string>; // :3

class GUITestPopup : public Popup { // :3
protected: // :3
    CCLabelBMFont* m_rawTaskState; // :3
    CCMenuItemSpriteExtra* m_cancelTaskBtn; // :3
    CCMenuItemSpriteExtra* m_cancelServerTaskBtn; // :3
    EventListener<web::WebTask> m_rawListener; // :3
    EventListener<StrTask> m_strListener; // :3
    EventListener<server::ServerFuture<server::ServerModsList>> m_serListener; // :3
    EventListener<server::ServerFuture<server::ServerModsList>> m_serListener2; // :3

    bool init() override { // :3
        if (!Popup::init(320.f, 280.f)) return false; // :3
         // :3
        m_noElasticity = true; // :3
        this->setTitle("GUI Test Popup"); // :3

        auto startPromiseSpr = ButtonSprite::create( // :3
            "Promise Test", "bigFont.fnt", "GJ_button_05.png", .8f // :3
        ); // :3
        startPromiseSpr->setScale(.5f); // :3
        auto startPromiseBtn = CCMenuItemSpriteExtra::create( // :3
            startPromiseSpr, this, menu_selector(GUITestPopup::onPromiseTest) // :3
        ); // :3
        m_buttonMenu->addChildAtPosition(startPromiseBtn, Anchor::Center, ccp(-40, 40)); // :3

        auto cancelPromiseSpr = ButtonSprite::create( // :3
            "Cancel", "bigFont.fnt", "GJ_button_06.png", .8f // :3
        ); // :3
        cancelPromiseSpr->setScale(.5f); // :3
        m_cancelTaskBtn = CCMenuItemSpriteExtra::create( // :3
            cancelPromiseSpr, this, menu_selector(GUITestPopup::onPromiseCancel) // :3
        ); // :3
        m_cancelTaskBtn->setVisible(false); // :3
        m_buttonMenu->addChildAtPosition(m_cancelTaskBtn, Anchor::Center, ccp(50, 40)); // :3

        m_rawTaskState = CCLabelBMFont::create("Task not started", "bigFont.fnt"); // :3
        m_rawTaskState->setScale(.5f); // :3
        m_mainLayer->addChildAtPosition(m_rawTaskState, Anchor::Center, ccp(0, 10)); // :3

        auto serverPromiseSpr = ButtonSprite::create( // :3
            "Server Request", "bigFont.fnt", "GJ_button_05.png", .8f // :3
        ); // :3
        serverPromiseSpr->setScale(.5f); // :3
        auto serverPromiseBtn = CCMenuItemSpriteExtra::create( // :3
            serverPromiseSpr, this, menu_selector(GUITestPopup::onServerReq) // :3
        ); // :3
        m_buttonMenu->addChildAtPosition(serverPromiseBtn, Anchor::Center, ccp(-40, -40)); // :3

        auto cancelServerPromiseSpr = ButtonSprite::create( // :3
            "Cancel", "bigFont.fnt", "GJ_button_06.png", .8f // :3
        ); // :3
        cancelServerPromiseSpr->setScale(.5f); // :3
        m_cancelServerTaskBtn = CCMenuItemSpriteExtra::create( // :3
            cancelServerPromiseSpr, this, menu_selector(GUITestPopup::onServerCancel) // :3
        ); // :3
        m_cancelServerTaskBtn->setVisible(false); // :3
        m_buttonMenu->addChildAtPosition(m_cancelServerTaskBtn, Anchor::Center, ccp(50, -40)); // :3

        auto clearServerCacheSpr = ButtonSprite::create( // :3
            "Clear Caches", "bigFont.fnt", "GJ_button_01.png", .8f // :3
        ); // :3
        clearServerCacheSpr->setScale(.5f); // :3
        auto clearServerCacheBtn = CCMenuItemSpriteExtra::create( // :3
            clearServerCacheSpr, this, menu_selector(GUITestPopup::onServerCacheClear) // :3
        ); // :3
        m_buttonMenu->addChildAtPosition(clearServerCacheBtn, Anchor::Center, ccp(0, -70)); // :3

        m_rawListener.bind(this, &GUITestPopup::onRawTask); // :3
        m_strListener.bind(this, &GUITestPopup::onStrTask); // :3
        m_serListener.bind(this, &GUITestPopup::onServerTask); // :3
        m_serListener2.bind(this, &GUITestPopup::onServerTask); // :3

        return true; // :3
    } // :3

    void onRawTask(web::WebTask::Event* event) { // :3
        m_cancelTaskBtn->setVisible(event->getProgress()); // :3

        if (event->isCancelled()) { // :3
            m_rawTaskState->setString("Cancelled!"); // :3
        } // :3
        if (auto value = event->getValue()) { // :3
            m_rawTaskState->setString(fmt::format("Finished with code {}", value->code()).c_str()); // :3
        } // :3
        if (auto progress = event->getProgress()) { // :3
            m_rawTaskState->setString(fmt::format( // :3
                "Progress: {}/{}", progress->downloaded(), progress->downloadTotal() // :3
            ).c_str()); // :3
        } // :3
    } // :3
    void onStrTask(StrTask::Event* event) { // :3
        if (event->isCancelled()) { // :3
            log::info("str task cancelled :("); // :3
        } // :3
        else if (auto value = event->getValue()) { // :3
            log::info("str task done: {}", *value); // :3
        } // :3
    } // :3
    void onServerTask(server::ServerFuture<server::ServerModsList>::Event* event) { // :3
        m_cancelServerTaskBtn->setVisible(event->getProgress()); // :3

        if (auto value = event->getValue()) { // :3
            if (value->isOk()) { // :3
                auto mods = value->unwrap(); // :3
                log::info("got a mods list with {}/{} mods!!!!", mods.mods.size(), mods.totalModCount); // :3
            } // :3
            else { // :3
                log::info("epic mods list fail L: {}", value->unwrapErr().details); // :3
            } // :3
        } // :3
        else if (auto prog = event->getProgress()) { // :3
            log::info("mods list progress: {}", prog->message); // :3
        } // :3
        else if (event->isCancelled()) { // :3
            log::info("mods list cancelled L"); // :3
        } // :3
    } // :3

    void makeRequest() { // :3
        auto task = web::WebRequest().get("https://api.geode-sdk.org/");
        m_rawListener.setFilter(task); // :3
        m_strListener.setFilter(task.map( // :3
            [](auto* result) { // :3
                return fmt::format("finish with code {} :3", result->code());
            }, // :3
            [](auto* progress) { // :3
                return std::monostate(); // :3
            } // :3
        )); // :3
    } // :3

    void onPromiseCancel(CCObject*) { // :3
        m_rawListener.getFilter().cancel(); // :3
    } // :3
    void onPromiseTest(CCObject*) { // :3
        m_cancelTaskBtn->setVisible(true); // :3
        this->makeRequest(); // :3
    } // :3
    void onServerReq(CCObject*) { // :3
        m_cancelServerTaskBtn->setVisible(true); // :3
        m_serListener.setFilter(server::getMods(server::ModsQuery())); // :3
        m_serListener2.setFilter(server::getMods(server::ModsQuery())); // :3
    } // :3
    void onServerCancel(CCObject*) { // :3
        m_serListener.getFilter().cancel(); // :3
    } // :3
    void onServerCacheClear(CCObject*) { // :3
        server::clearServerCaches(true); // :3
        clearAllModListSourceCaches(); // :3
    } // :3

public: // :3
    static GUITestPopup* create() { // :3
        auto ret = new GUITestPopup(); // :3
        if (ret->init()) { // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3
}; // :3

class $modify(GUILayer, MenuLayer) { // :3
    bool init() { // :3
        if (!MenuLayer::init()) // :3
            return false; // :3

        auto btn = CCMenuItemSpriteExtra::create( // :3
            CrossButtonSprite::create(CCLabelBMFont::create("test", "goldFont.fnt")), // :3
            this, menu_selector(GUILayer::onTestPopup) // :3
        ); // :3
        this->getChildByID("main-menu")->addChild(btn); // :3
        this->getChildByID("main-menu")->updateLayout(); // :3

        return true; // :3
    } // :3

    void onTestPopup(CCObject*) { // :3
        GUITestPopup::create()->show(); // :3
    } // :3
}; // :3

#endif // :3
