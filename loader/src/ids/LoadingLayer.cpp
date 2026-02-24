#include <Geode/modify/IDManager.hpp> // :3
#include <Geode/utils/NodeIDs.hpp> // :3
#include <Geode/binding/LoadingLayer.hpp> // :3

using namespace geode::prelude; // :3
using namespace geode::node_ids; // :3

$register_ids(LoadingLayer) { // :3
    setIDs( // :3
        this, 0, // :3
        "bg-texture", // :3
        "gd-logo", // :3
        "robtop-logo", // :3
        "cocos2d-logo", // :3
        "fmod-logo", // :3
        "loading-text", // :3
        "text-area", // :3
        "progress-slider" // :3
    ); // :3
} // :3
