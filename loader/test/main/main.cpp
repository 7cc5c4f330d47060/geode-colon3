#include <Geode/Loader.hpp> // :3
#include <Geode/loader/ModEvent.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <chrono> // :3
#include "../dependency/main.hpp" // :3
#include "Geode/utils/general.hpp" // :3
#include <Geode/utils/VMTHookManager.hpp> // :3
#include <Geode/loader/Event.hpp> // :3

using namespace geode::prelude; // :3

auto test = []() { // :3
    log::info("Static logged"); // :3
    return 0; // :3
}; // :3


// Exported functions :3
$on_mod(Loaded) { // :3
    log::info("Loaded"); // :3

    auto h1 = geode::Dispatch<std::string>("geode.test/test-garage-open").listen([](std::string const& str) { // :3
        log::info("Received dispatched event: {}", str); // :3
    }); // :3

    geode::Dispatch<std::string>("geode.test/test-garage-open").send("Hello from dispatch!"); // :3

    auto handle = Dispatch<int&>("test").listen([](int& val) { // :3
        geode::log::info("Received dispatched int: {}", val); // :3
        val += 10; // :3
        return val > 0; // :3
    }); // :3
     // :3
    auto handle2 = Dispatch<int>("test2").listen([](int val) { // :3
        geode::log::info("Received dispatched int2: {}", val); // :3
    }); // :3

    auto handle3 = Dispatch<float>("test").listen([](float val) { // :3
        geode::log::info("Received dispatched float: {}", val); // :3
    }); // :3

    auto handle4 = Dispatch<int&>("test").listen([](int& val) { // :3
        geode::log::info("Received dispatched int4: {}", val); // :3
        val += 10; // :3
    }, -5); // :3

    auto handle5 = Dispatch<int&>("test").listen([](int& val) { // :3
        geode::log::info("Received dispatched int5: {}", val); // :3
        val += 10; // :3
    }, 5); // :3

    int value = 5; // :3
    geode::log::info("Value before dispatch: {}", value); // :3
    Dispatch<int&>("test").send(value); // :3
    geode::log::info("Value after dispatch: {}", value); // :3
    Dispatch<int>("test2").send(7); // :3
    Dispatch<float>("test").send(9); // :3
    value = -35; // :3
    geode::log::info("Value before dispatch: {}", value); // :3
    Dispatch<int&>("test").send(value); // :3
    geode::log::info("Value after dispatch: {}", value); // :3
} // :3

static std::string s_receivedEvent; // :3

// Events :3
$on_mod(Loaded) { // :3
    TestEvent().listen(+[](std::string_view data) { // :3
        log::info("Received event: {}", data); // :3
        s_receivedEvent = data; // :3
    }).leak(); // :3
} // :3

// Coroutines :3
#include <Geode/utils/coro.hpp> // :3
auto advanceFrame() { // :3
    auto [task, finish, progress, cancelled] = Task<void>::spawn(); // :3
    queueInMainThread([finish = std::move(finish)] mutable { // :3
        finish(true); // :3
    }); // :3

    return task; // :3
} // :3

$on_mod(Loaded) { // :3
    $async() { // :3
        auto start = std::chrono::steady_clock::now(); // :3
        log::info("Waiting for 10 frames..."); // :3
        for (int i = 0; i < 10; ++i) // :3
            co_await advanceFrame(); // :3

        log::info("Finished waiting! Took {} seconds", std::chrono::duration_cast<std::chrono::seconds>( // :3
            std::chrono::steady_clock::now() - start // :3
        ).count()); // :3
    }; // :3

    auto output = $try<VersionInfo> { // :3
        log::info("Parsing number 123: {}", co_await utils::numFromString<int>("123")); // :3
        log::info("Parsing number 12.3: {}", co_await utils::numFromString<int>("12.3")); // :3

        co_return VersionInfo::parse("1.2.3-alpha.4"); // :3
    }; // :3

    if (!output) { // :3
        log::info("$try successfully caught error"); // :3
    } // :3
} // :3

#include <Geode/modify/MenuLayer.hpp> // :3
struct $modify(MenuLayer) { // :3
    bool init() { // :3
        if (!MenuLayer::init()) // :3
            return false; // :3

        auto node = CCNode::create(); // :3
        auto ref = WeakRef(node); // :3
        log::info("ref: {}", ref.lock().data()); // :3
        node->release(); // :3
        log::info("ref: {}", ref.lock().data()); // :3

        // Launch arguments :3
        log::info("Testing launch args..."); // :3
        log::NestScope nest; // :3
        log::info("For global context:"); // :3
        { // :3
            log::NestScope nest; // :3
            for (const auto& arg : Loader::get()->getLaunchArgumentNames()) { // :3
                log::info("{}", arg); // :3
            } // :3
        } // :3
        log::info("For this mod:"); // :3
        { // :3
            log::NestScope nest; // :3
            for (const auto& arg : Mod::get()->getLaunchArgumentNames()) { // :3
                log::info("{}", arg); // :3
            } // :3
        } // :3
        log::info("Mod has launch arg 'mod-arg': {}", Mod::get()->hasLaunchArgument("mod-arg")); // :3
        log::info("Loader flag 'bool-arg': {}", Loader::get()->getLaunchFlag("bool-arg")); // :3
        log::info("Loader int 'int-arg': {}", Loader::get()->parseLaunchArgument<int>("int-arg").unwrapOr(0)); // :3

        log::debug("should run second!"); // :3

        if (GEODE_UNWRAP_IF_OK(val, api::addNumbers(5, 6))) { // :3
            log::info("5 + 6 = {}", val); // :3
        } // :3
        else { // :3
            log::error("Failed to API (function)"); // :3
        } // :3

        api::Test test; // :3
        if (GEODE_UNWRAP_IF_OK(val, test.addNumbers(5, 6))) { // :3
            log::info("5 + 6 = {}", val); // :3
        } // :3
        else { // :3
            log::error("Failed to API (method)"); // :3
        } // :3


        return true; // :3
    } // :3
}; // :3

// Modify :3
#include <Geode/modify/GJGarageLayer.hpp> // :3

struct GJGarageLayerTest : Modify<GJGarageLayerTest, GJGarageLayer> { // :3
    struct Fields { // :3
        int myValue = 1907; // :3
        std::string myString = "yeah have fun finding a better thing for this"; // :3
    }; // :3

    bool init() { // :3
        if (!GJGarageLayer::init()) return false; // :3

        auto label = CCLabelBMFont::create("Modify works!", "bigFont.fnt"); // :3
        label->setPosition(100, 110); // :3
        label->setScale(.4f); // :3
        label->setZOrder(99999); // :3
        addChild(label); // :3

        if (m_fields->myValue == 1907 && m_fields->myString != "") { // :3
            auto label = CCLabelBMFont::create("Field default works!", "bigFont.fnt"); // :3
            label->setPosition(100, 100); // :3
            label->setScale(.4f); // :3
            label->setZOrder(99999); // :3
            addChild(label); // :3
        } // :3

        // Saved Values :3
        auto timesOpened = Mod::get()->getSavedValue<int64_t>("times-opened", 0); // :3
        Mod::get()->setSavedValue("times-opened", timesOpened + 1); // :3

        auto label2 = CCLabelBMFont::create( // :3
            fmt::format("Times opened: {}", timesOpened).c_str(), // :3
            "bigFont.fnt" // :3
        ); // :3
        label2->setPosition(100, 90); // :3
        label2->setScale(.4f); // :3
        label2->setZOrder(99999); // :3
        addChild(label2); // :3

        // Dispatch system pt. 1 :3
        MyDispatchEvent("geode.test/test-garage-open").send(this); // :3

        if (s_receivedEvent.size() > 0) { // :3
            auto label = CCLabelBMFont::create("Event works!", "bigFont.fnt"); // :3
            label->setPosition(100, 70); // :3
            label->setScale(.4f); // :3
            label->setZOrder(99999); // :3
            addChild(label); // :3
        } // :3

        return true; // :3
    } // :3
}; // :3


#include <Geode/modify/GJGarageLayer.hpp> // :3

struct GJGarageLayerTest2 : Modify<GJGarageLayerTest2, GJGarageLayer> { // :3
    struct Fields { // :3
        int myOtherValue = 80085; // :3
        int counter = 0; // :3
    }; // :3

    bool init() override { // :3
        if (!GJGarageLayer::init()) return false; // :3

        if (m_fields->myOtherValue == 80085) { // :3
            auto label = CCLabelBMFont::create("Alternate Fields works!", "bigFont.fnt"); // :3
            label->setPosition(100, 60); // :3
            label->setScale(.4f); // :3
            label->setZOrder(99999); // :3
            this->addChild(label); // :3
        } // :3

        this->setTouchMode(kCCTouchesOneByOne); // :3
        this->setTouchEnabled(true); // :3

        auto hook = VMTHookManager::get().addHook< // :3
            ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchBegan) // :3
        >(this, "GJGarageLayer::ccTouchBegan"); // :3
        auto hook2 = VMTHookManager::get().addHook< // :3
            ResolveC<CCTouchDelegate>::func(&GJGarageLayerTest2::ccTouchBegan) // :3
        >(this, "GJGarageLayer::ccTouchBegan"); // :3

        auto hook3 = VMTHookManager::get().addHook< // :3
            ResolveC<CCTouchDelegate>::func(&GJGarageLayerTest2::ccTouchEnded) // :3
        >(this, "GJGarageLayer::ccTouchEnded"); // :3
        auto hook4 = VMTHookManager::get().addHook< // :3
            ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchEnded) // :3
        >(this, "GJGarageLayer::ccTouchEnded"); // :3

        return true; // :3
    } // :3

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override { // :3
        this->ccTouchBegan(touch, event); // :3
        log::debug("Touch began on GJGarageLayer"); // :3

        if (m_fields->counter % 2) { // :3
            if (auto r = VMTHookManager::get().forceDisableFunction< // :3
                ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchEnded) // :3
            >(this); !r) { // :3
                geode::log::warn("forceDisableFunction failed: {}", r.unwrapErr()); // :3
            } // :3
        } // :3
        else { // :3
            if (auto r = VMTHookManager::get().forceEnableFunction< // :3
                ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchEnded) // :3
            >(this); !r) { // :3
                geode::log::warn("forceEnableFunction failed: {}", r.unwrapErr()); // :3
            } // :3
        } // :3
        m_fields->counter++; // :3
        return true; // :3
    } // :3

    void ccTouchEnded(CCTouch* touch, CCEvent* event) override { // :3
        this->ccTouchEnded(touch, event); // :3
        log::debug("Touch ended on GJGarageLayer"); // :3
    } // :3
}; // :3
