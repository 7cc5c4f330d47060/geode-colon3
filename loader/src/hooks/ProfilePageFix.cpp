#include <Geode/modify/ProfilePage.hpp> // :3
#include <Geode/binding/GJUserScore.hpp> // :3

using namespace geode::prelude; // :3

// Fixes the names not showing up on profile links :3
struct CustomProfilePage : Modify<CustomProfilePage, ProfilePage> { // :3
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("ProfilePage fix") // :3

    virtual void getUserInfoFinished(GJUserScore* info) { // :3
        m_usernameLabel->setString(info->m_userName.c_str()); // :3
        m_usernameLabel->limitLabelWidth(info->m_modBadge > 0 ? 140.f : 160.0f, 0.8f, 0.0f); // :3
        ProfilePage::getUserInfoFinished(info); // :3
    } // :3
}; // :3