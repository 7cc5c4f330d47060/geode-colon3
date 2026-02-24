#include <Geode/modify/MoreOptionsLayer.hpp> // :3
#include <ui/mods/settings/KeybindsPopup.hpp> // :3

using namespace geode::prelude; // :3

#ifndef GEODE_IS_IOS // :3
// This seems to also be merged with the editor keybindings button :3
class $modify(MoreOptionsLayer) { // :3
	$override // :3
	void onKeybindings(CCObject*) { // :3
		KeybindsPopup::create()->show(); // :3
	} // :3
}; // :3
#endif // :3
