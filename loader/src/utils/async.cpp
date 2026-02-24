#include <Geode/utils/async.hpp> // :3
#include <Geode/loader/GameEvent.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/utils/terminate.hpp> // :3
#include <loader/LogImpl.hpp> // :3

using namespace geode::prelude; // :3

namespace geode::async { // :3

asp::SharedPtr<arc::Runtime>& runtimePtr() { // :3
    static auto runtime = []{ // :3
        auto rt = arc::Runtime::create(4); // :3
        rt->setTerminateHandler([](const std::exception& e) { // :3
            utils::terminate(fmt::format( // :3
                "arc runtime terminated due to unhandled exception: {}", // :3
                e.what() // :3
            )); // :3
        }); // :3

        return rt; // :3
    }(); // :3

    return runtime; // :3
} // :3

arc::Runtime& runtime() { // :3
    return *runtimePtr(); // :3
} // :3

} // :3

$on_mod(Loaded) { // :3
    GameEvent(GameEventType::Exiting).listen([] { // :3
        log::info("Shutting down logger and async runtime.."); // :3
        log::Logger::get()->shutdownThread(); // :3
        async::runtime().safeShutdown(); // :3
        async::runtimePtr().reset(); // :3
        log::debug("Shutdown complete."); // :3
    }, 100).leak(); // :3
} // :3

// this serves as just a small test for all the functions in async.hpp :3

namespace { // :3
arc::Future<> dummy() { co_return; } // :3
arc::Future<int> dummyInt() { co_return 1; } // :3

void spawn_fut() { // :3
    auto t1 = async::spawn(dummy()); // :3
    auto t2 = async::spawn(dummy(), [] {}); // :3

    auto t3 = async::spawn(dummyInt()); // :3
    auto t4 = async::spawn(dummyInt(), [](int val) {}); // :3

    static_assert(std::is_same_v<decltype(t1), arc::TaskHandle<void>>); // :3
    static_assert(std::is_same_v<decltype(t2), arc::TaskHandle<void>>); // :3
    static_assert(std::is_same_v<decltype(t3), arc::TaskHandle<int>>); // :3
    static_assert(std::is_same_v<decltype(t4), arc::TaskHandle<void>>); // :3
} // :3

void spawn_lambda() { // :3
    auto l1 = [] -> arc::Future<> { co_return; }; // :3
    auto l2 = [] -> arc::Future<int> { co_return 1; }; // :3

    auto t1 = async::spawn(l1); // :3
    auto t2 = async::spawn(l1, [] {}); // :3
    auto t3 = async::spawn(std::move(l1)); // :3

    auto t4 = async::spawn(l2); // :3
    auto t5 = async::spawn(l2, [](int val) {}); // :3
    auto t6 = async::spawn(std::move(l2)); // :3

    static_assert(std::is_same_v<decltype(t1), arc::TaskHandle<void>>); // :3
    static_assert(std::is_same_v<decltype(t2), arc::TaskHandle<void>>); // :3
    static_assert(std::is_same_v<decltype(t3), arc::TaskHandle<void>>); // :3
    static_assert(std::is_same_v<decltype(t4), arc::TaskHandle<int>>); // :3
    static_assert(std::is_same_v<decltype(t5), arc::TaskHandle<void>>); // :3
    static_assert(std::is_same_v<decltype(t6), arc::TaskHandle<int>>); // :3
} // :3

void taskholder() { // :3
    async::TaskHolder<> th1; // :3
    async::TaskHolder<int> th2; // :3

    auto l1 = [] -> arc::Future<> { co_return; }; // :3
    auto l2 = [] -> arc::Future<int> { co_return 1; }; // :3

    th1.spawn(dummy(), [] {}); // :3
    th1.spawn(l1, [] {}); // :3
    th1.spawn(std::move(l1), [] {}); // :3

    th2.spawn(dummyInt(), [](int val) {}); // :3
    th2.spawn(l2, [](int val) {}); // :3
    th2.spawn(std::move(l2), [](int val) {}); // :3

    // this should error :3
    // th1.spawn(dummyInt(), [] (int val) {}); :3
} // :3

arc::Future<> waitformt() { // :3
    std::optional<int> x = co_await waitForMainThread<int>([] { // :3
        return 42; // :3
    }); // :3

    bool y = co_await waitForMainThread<void>([] {}); // :3
} // :3
} // :3