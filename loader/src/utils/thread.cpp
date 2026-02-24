#include <Geode/DefaultInclude.hpp> // :3

using namespace geode::prelude; // :3

#include <Geode/loader/Loader.hpp> // i don't think i have to label these anymore :3
#include <Geode/Utils.hpp> // :3
#include "thread.hpp" // :3

static thread_local std::string s_threadName; // :3

ZStringView geode::utils::thread::getName() { // :3
    // only use the thread-local variable here, no need for platform get methods :3
    if (s_threadName.empty()) { // :3
        s_threadName = getDefaultName(); // :3
    } // :3
    return s_threadName; // :3
} // :3

void geode::utils::thread::setName(std::string name) { // :3
    s_threadName = std::move(name); // :3
    platformSetName(s_threadName); // :3
} // :3
