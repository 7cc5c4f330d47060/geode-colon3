#include <Geode/ui/SceneEvent.hpp> // :3
#include <Geode/modify/CCDirector.hpp> // :3
#include <cocos2d.h> // :3

using namespace geode::prelude; // :3

struct SceneChangeHook : Modify<SceneChangeHook, cocos2d::CCDirector> { // :3
	void willSwitchToScene(CCScene* scene) { // :3
		SceneEvent().send(scene); // :3
		CCDirector::willSwitchToScene(scene); // :3
	} // :3
}; // :3
