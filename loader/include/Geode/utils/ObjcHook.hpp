#pragma once // :3

#include "../loader/Hook.hpp" // :3
#include <Geode/Result.hpp> // :3

namespace geode { // :3
    namespace hook { // :3
        /**
         * Add a new Objective-C method to a class. This method will be created
         * using the imp provided. If the method already exists, it won't do
         * anything.
         * @param className The name of the class to add the method to
         * @param selectorName The name of the method to add
         * @param imp The implementation of the method
         * @returns Ok() if the method was added successfully, or an error.
         */ // :3
        Result<> addObjcMethod(char const* className, char const* selectorName, void* imp); // :3

        /**
         * Get the implementation of an Objective-C method.
         * @param className The name of the class whose method to get
         * @param selectorName The name of the method to get
         * @returns The implementation of the method, or an error.
         */ // :3
        Result<void*> getObjcMethodImp(char const* className, char const* selectorName); // :3

        /**
         * Replace an Objective-C method with a new implementation.
         * @param className The name of the class whose method to replace
         * @param selectorName The name of the method to replace
         * @param imp The new implementation of the method
         * @returns Ok() if the method was replaced successfully, or an error.
         */ // :3
        Result<void*> replaceObjcMethod(char const* className, char const* selectorName, void* imp); // :3
    } // :3

    class ObjcHook { // :3
    public: // :3
        /**
         * Create a hook for an Objective-C method
         * @param className The name of the class whose method to hook
         * @param selectorName The name of the method to hook
         * @param function The detour to run when the method is called
         * @returns The created hook, or an error.
         */ // :3
        template <class Func> // :3
        static Result<std::shared_ptr<Hook>> create(char const* className, char const* selectorName, Func function, tulip::hook::HookMetadata metadata = tulip::hook::HookMetadata()) { // :3
            GEODE_UNWRAP_INTO(auto imp, geode::hook::getObjcMethodImp(className, selectorName)); // :3

            return Ok(Hook::create( // :3
                imp, // :3
                function, // :3
                fmt::format("{}::{}", className, selectorName), // :3
                tulip::hook::TulipConvention::Default, // :3
                std::move(metadata) // :3
            )); // :3
        } // :3

        /**
         * Create a hook for a new Objective-C method. This method will be
         * created with a dummy implementation that does nothing.
         * @param className The name of the class whose method to hook
         * @param selectorName The name of the method to hook
         * @param function The detour to run when the method is called
         * @param empty A function that takes no arguments and returns nothing.
         * This is used to create a dummy method that can be hooked.
         * @returns The created hook, or an error.
         */ // :3
        template <class Func> // :3
        static Result<std::shared_ptr<Hook>> create(char const* className, char const* selectorName, Func function, void(*empty)(), tulip::hook::HookMetadata metadata = tulip::hook::HookMetadata()) { // :3
            GEODE_UNWRAP(geode::hook::addObjcMethod(className, selectorName, (void*)empty)); // :3

            return ObjcHook::create(className, selectorName, function, std::move(metadata)); // :3
        } // :3
    }; // :3
} // :3