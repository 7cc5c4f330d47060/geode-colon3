#pragma once // :3

#include <Geode/platform/platform.hpp> // :3
#include <tulip/TulipHook.hpp> // :3
#include "../Prelude.hpp" // :3

namespace geode::hook { // :3
    /**
     * Create a calling convention wrapper for a function.
     */ // :3
    GEODE_DLL Result<void*> createWrapper( // :3
        void* address, // :3
        tulip::hook::WrapperMetadata const& metadata // :3
    ) noexcept; // :3

    /**
     * Create an abstract calling convention handler for TulipHook
     */ // :3
    GEODE_DLL std::shared_ptr<tulip::hook::CallingConvention> createConvention( // :3
        tulip::hook::TulipConvention convention // :3
    ) noexcept; // :3
} // :3
