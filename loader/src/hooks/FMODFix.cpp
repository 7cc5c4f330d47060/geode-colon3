#include <Geode/Geode.hpp> // :3
#include <Geode/modify/ChannelControl.hpp> // :3
#include <Geode/modify/System.hpp> // :3

using namespace geode::prelude; // :3

// Workaround for a bug where FMOD::ChannelControl::setVolume is called with :3
//   uninitialized (invalid) channel pointers from FMODAudioEngine. :3
// This creates a very annoying crash during load in some cases. :3

auto g_systemInitialized = false; // :3

struct FMODSystemFix : Modify<FMODSystemFix, FMOD::System> { // :3
    FMOD_RESULT init(int maxChannels, FMOD_INITFLAGS flags, void* extraData) { // :3
        g_systemInitialized = true; // :3
        return FMOD::System::init(maxChannels, flags, extraData); // :3
    } // :3
}; // :3

struct FMODChannelControlFix : Modify<FMODChannelControlFix, FMOD::ChannelControl> { // :3
    FMOD_RESULT setVolume(float volume) { // :3
        if (!g_systemInitialized) { // :3
            return FMOD_ERR_UNINITIALIZED; // :3
        } // :3

        return FMOD::ChannelControl::setVolume(volume); // :3
    } // :3
}; // :3

/*
// this hook requires a tuliphook update
// (setEffectsVolume is too small to hook, so it overwrites the stopAllMusic call below it)

#include <Geode/modify/FMODAudioEngine.hpp>

struct AndroidFMODFix : Modify<AndroidFMODFix, FMODAudioEngine> {
    void setEffectsVolume(float volume) {
        if (this->m_system == nullptr) [[unlikely]] {
            return;
        }

        FMODAudioEngine::setEffectsVolume(volume);
    }

    void setBackgroundMusicVolume(float volume) {
        if (this->m_system == nullptr) [[unlikely]] {
            return;
        }

        FMODAudioEngine::setBackgroundMusicVolume(volume);
    }
};
*/ // :3