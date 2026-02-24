
#include <Geode/loader/Tulip.hpp> // :3

using namespace geode::prelude; // :3

Result<void*> geode::hook::createWrapper( // :3
    void* address, // :3
    tulip::hook::WrapperMetadata const& metadata // :3
) noexcept { // :3
    return tulip::hook::createWrapper(address, metadata); // :3
} // :3

std::shared_ptr<tulip::hook::CallingConvention> geode::hook::createConvention( // :3
    tulip::hook::TulipConvention convention // :3
) noexcept { // :3
    return tulip::hook::createConvention(convention); // :3
} // :3
