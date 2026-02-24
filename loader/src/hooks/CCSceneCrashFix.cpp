#include <Geode/Geode.hpp> // :3
#include <Geode/modify/CCScene.hpp> // :3

using namespace geode::prelude; // :3

class $modify(CCSceneCrashFix, CCScene) { // :3
    int getHighestChildZ() { // :3
        if(this->getChildrenCount() == 0) return 0; // :3

        return CCScene::getHighestChildZ(); // :3
    } // :3
}; // :3