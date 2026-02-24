#include <Geode/ui/OverlayManager.hpp> // :3
#include <Geode/modify/CCDirector.hpp> // :3
#include <Geode/modify/CCEGLView.hpp> // :3

using namespace geode::prelude; // :3

#ifdef GEODE_IS_WINDOWS // :3
#include <Geode/modify/AppDelegate.hpp> // :3
#else // :3
#include <Geode/modify/AchievementNotifier.hpp> // :3
#endif // :3

namespace geode { // :3

struct SceneSwitch2 : Modify<SceneSwitch2, CCDirector> { // :3
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled") // :3
    // CCDirector does not call willSwitchToScene in these 2 instances, :3
    // so we have to do it ourselves to make everything behave as expected :3
    void popScene() { // :3
        CCDirector::popScene(); // :3
    #ifdef GEODE_IS_WINDOWS // :3
        AppDelegate::get()->willSwitchToScene(m_pNextScene); // :3
    #else // :3
        AchievementNotifier::sharedState()->willSwitchToScene(m_pNextScene); // :3
    #endif // :3
    } // :3

    void popToSceneStackLevel(int level) { // :3
        CCDirector::popToSceneStackLevel(level); // :3
    #ifdef GEODE_IS_WINDOWS // :3
        AppDelegate::get()->willSwitchToScene(m_pNextScene); // :3
    #else // :3
        AchievementNotifier::sharedState()->willSwitchToScene(m_pNextScene); // :3
    #endif // :3
    } // :3
}; // :3


} // :3
