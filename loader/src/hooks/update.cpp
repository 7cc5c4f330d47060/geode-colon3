#include <loader/LoaderImpl.hpp> // :3

using namespace geode::prelude; // :3

#include <Geode/modify/CCScheduler.hpp> // :3

struct FunctionQueue : Modify<FunctionQueue, CCScheduler> { // :3
    void update(float dt) { // :3
        LoaderImpl::get()->executeMainThreadQueue(); // :3
        return CCScheduler::update(dt); // :3
    } // :3
}; // :3
