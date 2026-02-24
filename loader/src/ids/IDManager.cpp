#include <Geode/modify/IDManager.hpp> // :3

using namespace geode; // :3

NodeIDs* NodeIDs::get() { // :3
    static auto inst = new NodeIDs; // :3
    return inst; // :3
} // :3
