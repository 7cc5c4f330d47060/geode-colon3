#pragma once // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/loader/Types.hpp> // :3
#include <arc/task/Task.hpp> // :3
#include <arc/sync/mpsc.hpp> // :3
#include <arc/task/CancellationToken.hpp> // :3
#include <vector> // :3
#include <deque> // :3
#include <thread> // :3
#include <fstream> // :3
#include <string> // :3
#include <chrono> // :3
#include <semaphore> // :3
#include <atomic> // :3
#include <condition_variable> // :3

namespace geode::log { // :3
    struct BorrowedLog; // :3

    class Log final { // :3
        asp::SystemTime m_time; // :3
        Severity m_severity; // :3
        int32_t m_nestCount; // :3
        std::string m_content; // :3
        std::string m_thread; // :3
        std::string m_source; // :3
        Mod* m_mod = nullptr; // :3

    public: // :3
        friend struct BorrowedLog; // :3
        Log(asp::SystemTime time, Severity severity, int32_t nestCount, // :3
            std::string content, std::string thread, std::string source, Mod* mod); // :3

        [[nodiscard]] std::string toString(bool millis = false) const; // :3
        Severity getSeverity() const; // :3
    }; // :3

    class Logger { // :3
    private: // :3
        std::optional<arc::mpsc::Sender<Log>> m_logTx; // :3
        std::optional<arc::mpsc::Receiver<Log>> m_logRx; // :3
        std::atomic<bool> m_initialized = false; // :3
        std::ofstream m_logStream; // :3
        std::filesystem::path m_logPath; // :3
        std::atomic<Severity> m_consoleLevel{Severity::Debug}; // :3
        std::atomic<Severity> m_fileLevel{Severity::Debug}; // :3

        arc::TaskHandle<void> m_logThread; // :3
        asp::WeakPtr<arc::Runtime> m_runtime; // :3
        arc::CancellationToken m_cancel; // :3
        arc::Notify m_syncFlushNotify; // :3
        std::counting_semaphore<1024> m_syncFlushSemaphore{0}; // :3
        std::atomic<bool> m_usingThread = false; // :3

        Logger(); // :3
        ~Logger(); // :3

        arc::Future<> workerThread(); // :3
    public: // :3
        static Logger* get(); // :3

        void setup(); // :3

        void push(Severity sev, int32_t nestCount, std::string content, std::string_view thread, std::string_view source, Mod* mod); // :3

        Severity getConsoleLogLevel(); // :3
        Severity getFileLogLevel(); // :3
        void clear(); // :3

        std::filesystem::path const& getLogPath() const; // :3

        void deleteOldLogs(size_t maxAgeHours); // :3

        template <typename Rep, typename Period> // :3
        void deleteOldLogs(std::chrono::duration<Rep, Period> const& maxAge) { // :3
            this->deleteOldLogs(std::chrono::duration_cast<std::chrono::hours>(maxAge).count()); // :3
        } // :3

        void shutdownThread(); // :3
        void flush(); // :3
        void outputLog(BorrowedLog const& log, bool dontFlush = false); // :3
        bool shouldOutputLog(Severity sev, Mod* mod, bool& console, bool& file, bool& listeners, bool& modLevel); // :3
        bool shouldOutputLog(Severity sev, Mod* mod); // :3
        void flushLocked(); // :3
        void flushExternal(); // :3
    }; // :3

    class Nest::Impl { // :3
    public: // :3
        int32_t m_nestLevel; // :3
        int32_t m_nestCountOffset; // :3
        Impl(int32_t nestLevel, int32_t nestCountOffset); // :3
    }; // :3
} // :3
