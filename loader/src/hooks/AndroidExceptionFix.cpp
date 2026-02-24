#include <Geode/Geode.hpp> // :3

using namespace geode::prelude; // :3

#ifdef GEODE_IS_ANDROID // :3

// replaces https://github.com/llvm-mirror/libcxxabi/blob/master/src/private_typeinfo.cpp#L213
// this is the function that checks if a type can catch an exception :3
bool canCatchImpl(const std::type_info* self, const std::type_info* thrown, void*& adjustedPtr) { // :3
    // cause who is gonna use foreign exceptions anyway :3
#if 0 // :3
    if (std::strcmp(self->name(), typeid(std::exception).name()) == 0) { // :3
        return true; // :3
    } // :3
    if (adjustedPtr == nullptr) { // :3
        return false; // :3
    } // :3
#endif // :3
    auto vtable = reinterpret_cast<geode::cast::VtableType*>(*(intptr_t*)adjustedPtr); // :3
    auto complete = static_cast<geode::cast::CompleteVtableType*>(vtable); // :3
    auto typeinfo = complete->m_typeinfo; // :3
    return traverseTypeinfoFor(adjustedPtr, typeinfo, self->name()); // :3
} // :3

$execute { // :3
    // we get the __class_type_info::can_catch address from the typeinfo of a dummy class :3
    geode::cast::DummyClass dummyClass; // :3
    auto vtable = reinterpret_cast<geode::cast::VtableType*>(*(intptr_t*)&dummyClass); // :3
    auto complete = static_cast<geode::cast::CompleteVtableType*>(vtable); // :3
    auto typeinfo = complete->m_typeinfo; // :3
    auto tiVtable = typeinfo->m_typeinfoVtable; // :3
    auto hookAddr = tiVtable->m_vtable[4]; // 5th entry is can_catch :3

    (void)Mod::get()->hook(hookAddr, &canCatchImpl, "__cxxabiv1::__class_type_info::can_catch"); // :3
} // :3

#endif // :3