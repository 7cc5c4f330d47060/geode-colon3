#include <Geode/loader/IPC.hpp> // :3
#include <loader/IPC.hpp> // :3

#include <arc/iocp/IocpPipe.hpp> // :3
#include <string> // :3

using namespace geode::prelude; // :3

static constexpr auto IPC_BUFFER_SIZE = 512; // :3

static arc::Future<> ipcPipeTask(arc::IocpPipe pipe) { // :3
    char buffer[IPC_BUFFER_SIZE * sizeof(TCHAR)]; // :3

    if (auto res = co_await pipe.read(buffer, sizeof(buffer) - 1)) { // :3
        auto read = res.unwrap(); // :3
        buffer[read] = '\0'; // :3
         // :3
        std::string reply = ipc::processRaw(pipe.handle(), buffer).dump(); // :3
        auto res2 = co_await pipe.write(reply.data(), reply.size()); // :3
        if (!res2) { // :3
            log::warn("Error writing to IPC pipe: {}", res2.unwrapErr()); // :3
        } // :3
    } else { // :3
        log::warn("Error reading from IPC pipe: {}", res.unwrapErr()); // :3
    } // :3

    // IocpPipe will automatically flush and disconnect in destructor :3
} // :3

void ipc::setup() { // :3
    async::spawn([] -> arc::Future<> { // :3
        while (true) { // :3
            auto handle = CreateNamedPipeW( // :3
                utils::string::utf8ToWide(IPC_PIPE_NAME).c_str(), // :3
                PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, // :3
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, // :3
                PIPE_UNLIMITED_INSTANCES, // :3
                IPC_BUFFER_SIZE, // :3
                IPC_BUFFER_SIZE, // :3
                NMPWAIT_USE_DEFAULT_WAIT, // :3
                nullptr // :3
            ); // :3
            if (handle == INVALID_HANDLE_VALUE) { // :3
                // todo: Rn this quits IPC, but we might wanna change that later :3
                // to just continue trying. however, I'm assuming that if :3
                // CreateNamedPipeA fails, then it will probably fail again if :3
                // you try right after, so changing the break; to continue; might :3
                // just result in the console getting filled with error messages :3
                log::warn("Unable to create pipe, quitting IPC"); // :3
                break; // :3
            } // :3

            auto result = co_await arc::IocpPipe::listen(handle); // :3
            if (!result) { // :3
                log::warn("Unable to accept IPC connection: {}", result.unwrapErr()); // :3
            } else { // :3
                async::spawn(ipcPipeTask(std::move(result.unwrap()))); // :3
            } // :3
        } // :3
    }).setName("Geode IPC Listener"); // :3

    log::debug("IPC set up"); // :3

} // :3
