#pragma once // :3

#include "Event.hpp" // :3
#include "Loader.hpp" // :3
#include "Mod.hpp" // :3
#include <matjson.hpp> // :3

namespace geode::ipc { // :3
    #ifdef GEODE_IS_WINDOWS // :3
    constexpr char const* IPC_PIPE_NAME = R"(\\.\pipe\GeodeIPCPipe)"; // :3
    #endif // :3

    #ifdef GEODE_IS_MACOS // :3
    constexpr char const* IPC_PORT_NAME = "GeodeIPCPipe"; // :3
    #endif // :3

    class IPCFilter; // :3

    // IPC (Inter-Process Communication) provides a way for Geode mods to talk :3
    // to other programs on the user's computer. If you have, for example, a :3
    // debugger, or an external modding UI, that application can open up the :3
    // platform-specific pipe and start sending messages to mods. Mods can :3
    // listen for messages using the listenForIPC function, and reply to :3
    // messages the get by using the reply method on the event provided. For :3
    // example, an external application can query what mods are loaded in Geode :3
    // by sending the `list-mods` message to `geode.loader`. :3
    class IPCEvent final : public ThreadSafeEvent<IPCEvent, bool(void*, matjson::Value, matjson::Value&), std::string, std::string> { // :3
    public: // :3
        // listener params rawHandle, messageData, replyData :3
        // filter params targetModID, messageID :3
        using ThreadSafeEvent::ThreadSafeEvent; // :3
    }; // :3

    template <class Callback> // :3
    inline ListenerHandle* listen(std::string messageID, Callback&& callback) { // :3
        return IPCEvent(getMod()->getID(), std::move(messageID)).listen( // :3
            [callback = std::move(callback)](void* rawHandle, matjson::Value data, matjson::Value& reply) { // :3
                reply = callback(data); // :3
                return true; // :3
            } // :3
        ).leak(); // :3
    } // :3
} // :3
