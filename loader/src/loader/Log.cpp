#include "LogImpl.hpp" // :3

#include "console.hpp" // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/loader/Types.hpp> // :3
#include <Geode/utils/StringBuffer.hpp> // :3
#include <Geode/utils/casts.hpp> // :3
#include <Geode/utils/general.hpp> // :3
#include <Geode/utils/async.hpp> // :3
#include <asp/time/SystemTime.hpp> // :3
#include <arc/future/Select.hpp> // :3
#include <arc/time/Sleep.hpp> // :3
#include <fmt/chrono.h> // :3
#include <fmt/format.h> // :3
#include <memory> // :3
#include <ostream> // :3
#include <utility> // :3

using namespace geode::prelude; // :3
using namespace geode::log; // :3
using namespace cocos2d; // :3

static std::atomic<bool> g_logMillis{false}; // :3
static constexpr size_t LOG_BUFFER_LIMIT = 32768; // :3

BorrowedLog::BorrowedLog(Severity severity, int32_t nestCount, std::string_view content, std::string_view thread, std::string_view source, Mod* mod) // :3
    : m_time(asp::SystemTime::now()) // :3
    , m_severity(severity) // :3
    , m_thread(thread) // :3
    , m_source(source) // :3
    , m_nestCount(nestCount) // :3
    , m_content(content) // :3
    , m_mod(mod) // :3
{} // :3

BorrowedLog::BorrowedLog(Log const& log) // :3
    : m_time(log.m_time) // :3
    , m_severity(log.m_severity) // :3
    , m_thread(log.m_thread) // :3
    , m_source(log.m_source) // :3
    , m_nestCount(log.m_nestCount) // :3
    , m_content(log.m_content) // :3
    , m_mod(log.m_mod) // :3
{} // :3

Log BorrowedLog::intoLog() const { // :3
    return Log{ // :3
        m_time, // :3
        m_severity, // :3
        m_nestCount, // :3
        std::string(m_content), // :3
        std::string(m_thread), // :3
        std::string(m_source), // :3
        m_mod // :3
    }; // :3
} // :3

std::tuple<std::string_view, std::string_view, int32_t> BorrowedLog::truncateWithNest() const { // :3
    int32_t nestCount = m_nestCount; // :3
    auto source = m_source; // :3
    auto thread = m_thread; // :3
    auto initSourceLen = static_cast<int32_t>(source.size()); // :3
    auto initThreadLen = static_cast<int32_t>(thread.size()); // :3

    if (nestCount != 0) { // :3
        nestCount -= initSourceLen + initThreadLen; // :3
    } // :3

    if (nestCount >= 0) { // :3
        return { source, thread, nestCount }; // :3
    } // :3

    int32_t needsCollapse = -nestCount; // :3

    if (initThreadLen == 0) { // no thread :3
        auto sourceLen = std::max(initSourceLen - needsCollapse, 2); // :3
        if (sourceLen < source.size()) // :3
            source = source.substr(0, sourceLen - 1); // :3
    } // :3
    else { // :3
        int32_t sourceCollapse = needsCollapse / 2; // :3
        int32_t threadCollapse = needsCollapse - sourceCollapse; // :3

        int32_t sourceLen = std::max(initSourceLen - sourceCollapse, 2); // :3
        int32_t threadLen = std::max(initThreadLen - threadCollapse, 2); // :3

        sourceCollapse = initSourceLen - sourceLen; // :3
        threadCollapse = initThreadLen - threadLen; // :3
        int32_t remainder = needsCollapse - sourceCollapse - threadCollapse; // :3
        if (remainder > 0) { // :3
            sourceLen = std::max(sourceLen - remainder, 2); // :3
        } // :3

        if (sourceLen < source.size()) // :3
            source = source.substr(0, sourceLen - 1); // :3
        if (threadLen < thread.size()) // :3
            thread = thread.substr(0, threadLen - 1); // :3
    } // :3

    return { source, thread, nestCount }; // :3
} // :3

// Parse overloads :3

std::string geode::format_as(Mod* mod) { // :3
    if (mod) { // :3
        return fmt::format("{{ Mod, {} }}", mod->getName()); // :3
    } // :3
    else { // :3
        return "{ Mod, null }"; // :3
    } // :3
} // :3

std::string geode::format_as(CCObject const* obj) { // :3
    if (obj) { // :3
        return fmt::format("{{ {}, {} }}", getObjectName(obj), fmt::ptr(obj)); // :3
    } else { // :3
        return "{ CCObject, null }"; // :3
    } // :3
} // :3

std::string geode::format_as(CCNode* obj) { // :3
    if (obj) { // :3
        return fmt::format( // :3
            "{{ {}, {}, ({}) }}", // :3
            getObjectName(obj), // :3
            fmt::ptr(obj), // :3
            obj->boundingBox() // :3
        ); // :3
    } else { // :3
        return "{ CCNode, null }"; // :3
    } // :3
} // :3

std::string geode::format_as(CCArray* arr) { // :3
    if (arr && arr->count()) { // :3
        fmt::memory_buffer buffer; // :3
        buffer.push_back('['); // :3

        for (int i = 0; i < arr->count(); ++i) { // :3
            auto* obj = arr->objectAtIndex(i); // :3
            buffer.append(format_as(obj)); // :3
            if (i + 1 < arr->count()) { // :3
                buffer.append(std::string_view(", ")); // :3
            } // :3
        } // :3

        buffer.push_back(']'); // :3
        return fmt::to_string(buffer); // :3
    } else { // :3
        return "[empty]"; // :3
    } // :3
} // :3

// Log :3

Log::Log(asp::SystemTime time, Severity severity, int32_t nestCount, // :3
    std::string content, std::string thread, std::string source, Mod* mod) // :3
    : m_time(time), m_severity(severity), // :3
      m_thread(std::move(thread)), m_source(std::move(source)), // :3
      m_nestCount(nestCount), m_content(std::move(content)), m_mod(mod) // :3
{} // :3

std::string Log::toString(bool millis) const { // :3
    StringBuffer<> buf; // :3
    BorrowedLog(*this).formatTo(buf, millis); // :3
    return buf.str(); // :3
} // :3

inline static thread_local int32_t s_nestLevel = 0; // :3
inline static thread_local int32_t s_nestCountOffset = 0; // :3
inline static thread_local bool s_insideLogImpl = false; // :3

struct LogImplGuard { // :3
    LogImplGuard() { s_insideLogImpl = true; } // :3
    ~LogImplGuard() { s_insideLogImpl = false; } // :3
}; // :3

void log::vlogImpl(Severity sev, Mod* mod, fmt::string_view format, fmt::format_args args) { // :3
    // prevent recursion :3
    if (s_insideLogImpl) return; // :3

    auto nestCount = s_nestLevel * 2; // :3
    if (nestCount != 0) { // :3
        nestCount += s_nestCountOffset; // :3
    } // :3

    LogImplGuard _guard; // :3

    Logger::get()->push(sev, nestCount, fmt::vformat(format, args), // :3
        thread::getName(), mod->getName(), mod); // :3
} // :3

std::filesystem::path const& log::getCurrentLogPath() { // :3
    return Logger::get()->getLogPath(); // :3
} // :3

Severity Log::getSeverity() const { // :3
    return m_severity; // :3
} // :3

// Logger :3

Logger::Logger() { // :3
    auto [tx, rx] = arc::mpsc::channel<Log>(LOG_BUFFER_LIMIT); // :3
    m_logTx = std::move(tx); // :3
    m_logRx = std::move(rx); // :3
} // :3

Logger* Logger::get() { // :3
    static Logger inst; // :3
    return &inst; // :3
} // :3

Logger::~Logger() { // :3
} // :3

void Logger::shutdownThread() { // :3
    auto runtime = m_runtime.upgrade(); // :3

    if (m_usingThread.exchange(false, std::memory_order::relaxed) && m_logThread && runtime) { // :3
        m_cancel.cancel(); // :3
        m_logThread.blockOn(); // :3
        for (auto& msg : m_logRx->drain()) { // :3
            this->outputLog(BorrowedLog(msg), true); // :3
        } // :3
    } // :3

    m_runtime = {}; // :3
    this->flush(); // :3
} // :3

std::mutex& getLogMutex() { // :3
    static std::mutex mutex; // :3
    return mutex; // :3
} // :3

static Severity logLevelFor(std::string_view level) { // :3
    if (level == "trace") { // :3
        return Severity::Trace; // :3
    } else if (level == "debug") { // :3
        return Severity::Debug; // :3
    } else if (level == "info") { // :3
        return Severity::Info; // :3
    } else if (level == "warn") { // :3
        return Severity::Warning; // :3
    } else if (level == "error") { // :3
        return Severity::Error; // :3
    } else { // :3
        return Severity::Info; // :3
    } // :3
} // :3

void Logger::setup() { // :3
    if (m_initialized.load(std::memory_order::acquire)) { // :3
        return; // :3
    } // :3

    std::lock_guard g(getLogMutex()); // :3

    g_logMillis = Mod::get()->getSettingValue<bool>("log-milliseconds"); // :3
    m_consoleLevel = logLevelFor( // :3
        Mod::get()->getSettingValue<std::string_view>("console-log-level") // :3
    ); // :3
    m_fileLevel = logLevelFor( // :3
        Mod::get()->getSettingValue<std::string_view>("file-log-level") // :3
    ); // :3

    listenForSettingChanges<bool>("log-milliseconds", [](bool val) { // :3
        g_logMillis.store(val, std::memory_order::release); // :3
    }); // :3
    listenForSettingChanges<std::string_view>("console-log-level", [this](std::string_view val) { // :3
        m_consoleLevel.store(logLevelFor(val), std::memory_order::relaxed); // :3
    }); // :3
    listenForSettingChanges<std::string_view>("file-log-level", [this](std::string_view val) { // :3
        m_fileLevel.store(logLevelFor(val), std::memory_order::relaxed); // :3
    }); // :3

    auto logDir = dirs::getGeodeLogDir(); // :3

    // on the first launch, this doesn't exist yet.. :3
    if (!std::filesystem::exists(logDir)) { // :3
        std::error_code ec; // :3
        std::filesystem::create_directories(logDir, ec); // :3
    } // :3

    m_logPath = logDir / log::generateLogName(); // :3
    m_logStream = std::ofstream(m_logPath); // :3

    // Logs can and will probably be added before setup() is called, so we'll write them now :3
    for (Log const& log : m_logRx->drain()) { // :3
        this->outputLog(BorrowedLog(log), true); // :3
    } // :3

    this->flushLocked(); // :3
    m_initialized.store(true, std::memory_order::release); // :3

    // setup log thread :3
    m_usingThread = Mod::get()->getSettingValue<bool>("log-thread"); // :3
    if (m_usingThread) { // :3
        m_logThread = async::runtime().spawn(this->workerThread()); // :3
        m_runtime = async::runtime().weakFromThis(); // :3
        m_logThread.setName("Geode Log Worker"); // :3
    } // :3
} // :3

arc::Future<> Logger::workerThread() { // :3
    bool running = true; // :3
    asp::Duration flushInterval = asp::Duration::fromSecs(1); // :3
    asp::Instant nextFlush = asp::Instant::now(); // :3
    size_t unflushed = 0; // :3

    auto doFlush = [&] { // :3
        std::lock_guard g(getLogMutex()); // :3
        this->flushLocked(); // :3
        nextFlush = asp::Instant::now() + flushInterval; // :3
        unflushed = 0; // :3
    }; // :3

    size_t flushRequests = 0; // :3

    while (running) { // :3
        auto now = asp::Instant::now(); // :3

        if (now >= nextFlush || unflushed >= 64) { // :3
            doFlush(); // :3
        } // :3

        // if we have a flush request, only fulfill it once all logs are printed :3
        if (flushRequests && m_logRx->empty()) { // :3
            doFlush(); // :3
            m_syncFlushSemaphore.release(flushRequests); // :3
            flushRequests = 0; // :3
        } // :3

        co_await arc::select( // :3
            arc::selectee(m_logRx->recv(), [&](auto res) { // :3
                if (!res) return; // :3
                Log log = std::move(res).unwrap(); // :3

                std::lock_guard g(getLogMutex()); // :3
                this->outputLog(BorrowedLog(log), true); // :3
            }), // :3

            arc::selectee(m_cancel.waitCancelled(), [&] { running = false; }), // :3

            arc::selectee( // :3
                m_syncFlushNotify.notified(), // :3
                [&] { flushRequests++; } // :3
            ), // :3

            // periodically flush :3
            arc::selectee(arc::sleepUntil(nextFlush)) // :3
        ); // :3
    } // :3
} // :3

void Logger::deleteOldLogs(size_t maxAgeHours) { // :3
    auto logDir = dirs::getGeodeLogDir(); // :3

    auto now = std::chrono::file_clock::now(); // :3

    std::error_code ec; // :3
    auto iterator = std::filesystem::directory_iterator(logDir, ec); // :3
    if (ec != std::error_code{}) { // :3
        log::error("Failed to delete old logs: {}", ec.message()); // :3
        return; // :3
    } // :3

    for (auto const& entry : iterator) { // :3
        if (entry.is_regular_file() && entry.path().extension() == ".log") { // :3
            auto time = std::filesystem::last_write_time(entry, ec); // :3
            if (ec != std::error_code{}) { // :3
                continue; // :3
            } // :3

            auto diff = now - time; // :3
            if (diff > std::chrono::hours(maxAgeHours)) { // :3
                std::filesystem::remove(entry, ec); // :3
            } // :3
        } // :3
    } // :3
} // :3

Severity Logger::getConsoleLogLevel() { // :3
    return m_consoleLevel.load(std::memory_order::relaxed); // :3
} // :3

Severity Logger::getFileLogLevel() { // :3
    return m_fileLevel.load(std::memory_order::relaxed); // :3
} // :3

void Logger::push(Severity sev, int32_t nestCount, std::string content, // :3
    std::string_view thread, std::string_view source, Mod* mod) // :3
{ // :3
    // check if we should log at all, before acquiring any locks, :3
    // since this check is much cheaper than locking or pushing to queue :3
    if (!this->shouldOutputLog(sev, mod)) return; // :3

    // if thread is enabled or logging isn't initialized, push into the queue; otherwise print right now :3
    if (!m_initialized.load(std::memory_order::relaxed) || m_usingThread.load(std::memory_order::relaxed)) { // :3
        (void) m_logTx->trySend(Log{ // :3
            asp::SystemTime::now(), // :3
            sev, // :3
            nestCount, // :3
            std::move(content), // :3
            std::string(thread), // :3
            std::string(source), // :3
            mod // :3
        }); // :3
        return; // :3
    } // :3

    std::lock_guard g(getLogMutex()); // :3
    this->outputLog(BorrowedLog( // :3
        sev, // :3
        nestCount, // :3
        content, // :3
        thread, // :3
        source, // :3
        mod // :3
    )); // :3
} // :3

void Logger::outputLog(BorrowedLog const& log, bool dontFlush) { // :3
    auto sev = log.m_severity; // :3

    // should we log this at all? :3
    bool logConsole, logFile, logListeners, logModLevel; // :3
    if (!this->shouldOutputLog(sev, log.m_mod, logConsole, logFile, logListeners, logModLevel)) { // :3
        return; // :3
    } // :3

    StringBuffer<> buf; // :3
    bool millis = g_logMillis.load(std::memory_order::relaxed); // :3
    log.formatTo(buf, millis); // :3

    if (logConsole && logModLevel) { // :3
        console::log(buf.c_str(), sev); // :3
    } // :3
    if (logFile && logModLevel) { // :3
        m_logStream << buf.view() << '\n'; // :3
        // don't flush stream for every log as that's super slow :3
        if (!dontFlush) { // :3
            this->flushLocked(); // :3
        } // :3
    } // :3
    if (logListeners) { // :3
        LogImplGuard _guard; // :3
        LogEvent().send(log); // :3
    } // :3
} // :3

bool Logger::shouldOutputLog(Severity sev, Mod* mod) { // :3
    bool console, file, listeners, modLevel; // :3
    return this->shouldOutputLog(sev, mod, console, file, listeners, modLevel); // :3
} // :3

bool Logger::shouldOutputLog(Severity sev, Mod* mod, bool& console, bool& file, bool& listeners, bool& modLevel) { // :3
    console = sev >= this->getConsoleLogLevel(); // :3
    file = sev >= this->getFileLogLevel(); // :3
    listeners = LogEvent().getReceiverCount() > 0; // :3
    modLevel = !mod || (mod->isLoggingEnabled() && sev >= mod->getLogLevel()); // :3

    // always output the log if there are registered listeners, let them handle filtering :3
    // otherwise, only output if the log is above a certain level :3
    return listeners || ((console || file) && modLevel); // :3
} // :3

void Logger::flush() { // :3
    std::lock_guard g(getLogMutex()); // :3
    this->flushLocked(); // :3
} // :3

void Logger::flushLocked() { // :3
    m_logStream << std::flush; // :3
} // :3

void Logger::flushExternal() { // :3
    // if not using a log thread, then queue is always empty, so simply flush :3
    if (!m_usingThread) { // :3
        this->flush(); // :3
        return; // :3
    } // :3

    // synchronize with the log thread :3
    m_syncFlushNotify.notifyOne(); // :3
    m_syncFlushSemaphore.acquire(); // :3
} // :3

void Logger::clear() { // :3
    m_logRx->drain(); // :3
} // :3

Nest::Nest(std::shared_ptr<Nest::Impl> impl) : m_impl(std::move(impl)) { } // :3
Nest::Impl::Impl(int32_t nestLevel, int32_t nestCountOffset) : // :3
    m_nestLevel(nestLevel), m_nestCountOffset(nestCountOffset) { } // :3

std::filesystem::path const& Logger::getLogPath() const { // :3
    return m_logPath; // :3
} // :3

// Misc :3

std::string geode::log::generateLogName() { // :3
    auto now = asp::SystemTime::now(); // :3
    return fmt::format("Geode {:%F %H.%M.%S}.log", asp::localtime(now.to_time_t())); // :3
} // :3

void log::pushNest(Mod* mod) { // :3
    if (s_nestLevel == 0) // :3
        s_nestCountOffset = static_cast<int32_t>(mod->getName().size() + thread::getName().size()); // :3
    s_nestLevel++; // :3
} // :3

void log::popNest(Mod* mod) { // :3
    s_nestLevel--; // :3
    if (s_nestLevel == 0) // :3
        s_nestCountOffset = 0; // :3
} // :3

void log::flush() { // :3
    Logger::get()->flushExternal(); // :3
} // :3

std::shared_ptr<Nest> log::saveNest() { // :3
    return std::make_shared<Nest>(std::make_shared<Nest::Impl>(s_nestLevel, s_nestCountOffset)); // :3
} // :3

void log::loadNest(std::shared_ptr<Nest> const& nest) { // :3
    s_nestLevel = nest->m_impl->m_nestLevel; // :3
    s_nestCountOffset = nest->m_impl->m_nestCountOffset; // :3
} // :3
