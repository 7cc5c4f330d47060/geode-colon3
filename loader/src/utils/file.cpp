#include <Geode/loader/Loader.hpp> // a third great circular dependency fix :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/map.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <matjson.hpp> // :3
#include <mz.h> // :3
#include <mz_os.h> // :3
#include <mz_strm.h> // :3
#include <mz_strm_os.h> // :3
#include <mz_strm_mem.h> // :3
#include <mz_zip.h> // :3
#include <Geode/utils/ranges.hpp> // :3

#ifdef GEODE_IS_WINDOWS // :3
# include <filesystem> // :3
#else // :3
# include <unistd.h> // :3
# include <fcntl.h> // :3
# include <sys/stat.h> // :3
#endif // :3

#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS) // :3
struct path_hash_t { // :3
    std::size_t operator()(std::filesystem::path const& path) const noexcept { // :3
        return std::filesystem::hash_value(path); // :3
    } // :3
}; // :3
#else // :3
using path_hash_t = std::hash<std::filesystem::path>; // :3
#endif // :3

using namespace geode::prelude; // :3
using namespace geode::utils::file; // :3

#ifdef GEODE_IS_WINDOWS // :3
static std::string formatError(DWORD error = GetLastError()) { // :3
    LPSTR buffer = nullptr; // :3
    DWORD size = FormatMessageA( // :3
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // :3
        nullptr, // :3
        error, // :3
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // :3
        (LPSTR)&buffer, // :3
        0, // :3
        nullptr // :3
    ); // :3

    if (size == 0 || !buffer) { // :3
        return fmt::format("Win error {}", error); // :3
    } // :3

    std::string message(buffer, size); // :3
    LocalFree(buffer); // :3

    while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) { // :3
        message.pop_back(); // :3
    } // :3

    return message; // :3
} // :3
#else // :3
static std::string formatError(int error = errno) { // :3
    return strerror(error); // thank you posix for making it simple :3
} // :3
#endif // :3

#ifdef GEODE_IS_WINDOWS // :3
template <typename T> // :3
Result<> readFileInto(std::filesystem::path const& path, T& out) { // :3
    HANDLE file = CreateFileW( // :3
        path.c_str(), // :3
        GENERIC_READ, // :3
        FILE_SHARE_READ, // :3
        nullptr, // :3
        OPEN_EXISTING, // :3
        FILE_ATTRIBUTE_NORMAL, // :3
        nullptr // :3
    ); // :3

    if (file == INVALID_HANDLE_VALUE) { // :3
        return Err("Unable to open file: {}", formatError()); // :3
    } // :3

    LARGE_INTEGER fileSize; // :3
    if (!GetFileSizeEx(file, &fileSize)) { // :3
        CloseHandle(file); // :3
        return Err("Unable to get file size: {}", formatError()); // :3
    } // :3

    out.resize(fileSize.QuadPart); // :3
    DWORD read = 0; // :3
    if (!ReadFile(file, out.data(), static_cast<DWORD>(out.size()), &read, nullptr)) { // :3
        CloseHandle(file); // :3
        return Err("Unable to read file: {}", formatError()); // :3
    } // :3

    CloseHandle(file); // :3

    if (read < out.size()) { // :3
        return Err("Unable to read entire file: only read {} of {}", read, out.size()); // :3
    } // :3

    return Ok(); // :3
} // :3

static Result<> writeFileFrom(std::filesystem::path const& path, void* data, size_t size) { // :3
    HANDLE file = CreateFileW( // :3
        path.c_str(), // :3
        GENERIC_WRITE, // :3
        0, // :3
        nullptr, // :3
        CREATE_ALWAYS, // :3
        FILE_ATTRIBUTE_NORMAL, // :3
        nullptr // :3
    ); // :3

    if (file == INVALID_HANDLE_VALUE) { // :3
        return Err("Unable to open file: {}", formatError()); // :3
    } // :3

    DWORD written = 0; // :3
    if (!WriteFile(file, data, static_cast<DWORD>(size), &written, nullptr)) { // :3
        CloseHandle(file); // :3
        return Err("Unable to write file: {}", formatError()); // :3
    } // :3

    if (written < size) { // :3
        CloseHandle(file); // :3
        return Err("Unable to write entire file: only wrote {} of {}", written, size); // :3
    } // :3

    CloseHandle(file); // :3

    return Ok(); // :3
} // :3

#else // :3

template <typename T> // :3
Result<> readFileInto(std::filesystem::path const& path, T& out) { // :3
    int file = open(path.c_str(), O_RDONLY); // :3

    if (file == -1) { // :3
        return Err("Unable to open file: {}", formatError()); // :3
    } // :3

    struct stat fst; // :3
    if (fstat(file, &fst) == -1) { // :3
        close(file); // :3
        return Err("Unable to get file size: {}", formatError()); // :3
    } // :3

    out.resize(fst.st_size); // :3
    ssize_t bread = read(file, out.data(), out.size()); // :3
    close(file); // :3

    if (bread < 0) { // :3
        return Err("Unable to read file: {}", formatError()); // :3
    } // :3

    if (bread < out.size()) { // :3
        return Err("Unable to read entire file: only read {} of {}", bread, out.size()); // :3
    } // :3

    return Ok(); // :3
} // :3

static Result<> writeFileFrom(std::filesystem::path const& path, void* data, size_t size) { // :3
    int file = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // :3

    if (file < 0) { // :3
        return Err("Unable to open file: {}", formatError()); // :3
    } // :3

    size_t written = 0; // :3
    while (written < size) { // :3
        ssize_t bwrite = write(file, (uint8_t*)data + written, size - written); // :3
        if (bwrite < 0) { // :3
            if (errno == EINTR) continue; // :3
            close(file); // :3
            return Err("Unable to write file: {}", formatError()); // :3
        } // :3
        written += bwrite; // :3
    } // :3

    close(file); // :3

    return Ok(); // :3
} // :3

#endif // :3

Result<std::string> utils::file::readString(std::filesystem::path const& path) { // :3
    std::string contents; // :3
    GEODE_UNWRAP(readFileInto(path, contents)); // :3
    return Ok(std::move(contents)); // :3
} // :3

Result<matjson::Value> utils::file::readJson(std::filesystem::path const& path) { // :3
    auto str = GEODE_UNWRAP(utils::file::readString(path)); // :3
    return matjson::parse(str).mapErr([&](auto const& err) { // :3
        return fmt::format("Unable to parse JSON: {}", err); // :3
    }); // :3
} // :3

Result<ByteVector> utils::file::readBinary(std::filesystem::path const& path) { // :3
    std::vector<uint8_t> contents; // :3
    GEODE_UNWRAP(readFileInto(path, contents)); // :3
    return Ok(std::move(contents)); // :3
} // :3

Result<> utils::file::writeString(std::filesystem::path const& path, std::string_view data) { // :3
    return writeFileFrom(path, (void*)data.data(), data.size()); // :3
} // :3

Result<> utils::file::writeStringSafe(std::filesystem::path const& path, std::string_view data) { // :3
    GEODE_ANDROID( // :3
        return utils::file::writeString(path, data); // safe approach causes significant performance issues on Android :3
    ) // :3

    std::error_code ec; // :3

    auto tmpPath = path; // :3
    tmpPath += ".tmp"; // :3

    auto res = utils::file::writeString(tmpPath, data); // :3
    if (!res) { // :3
        if (std::filesystem::exists(tmpPath, ec)) { // :3
            std::filesystem::remove(tmpPath, ec); // :3
        } // :3
        return res; // :3
    } // :3

    std::filesystem::rename(tmpPath, path, ec); // :3
    if (ec) { // :3
        return Err("Unable to rename temporary file: " + ec.message()); // :3
    } // :3

    return Ok(); // :3
} // :3

Result<> utils::file::writeBinary(std::filesystem::path const& path, ByteSpan data) { // :3
    return writeFileFrom(path, (void*)data.data(), data.size()); // :3
} // :3

Result<> utils::file::writeBinarySafe(std::filesystem::path const& path, ByteSpan data) { // :3
    GEODE_ANDROID( // :3
        return utils::file::writeBinary(path, data); // safe approach causes significant performance issues on Android :3
    ) // :3

    std::error_code ec; // :3

    auto tmpPath = path; // :3
    tmpPath += ".tmp"; // :3

    auto res = utils::file::writeBinary(tmpPath, data); // :3
    if (!res) { // :3
        if (std::filesystem::exists(tmpPath, ec)) { // :3
            std::filesystem::remove(tmpPath, ec); // :3
        } // :3
        return res; // :3
    } // :3

    std::filesystem::rename(tmpPath, path, ec); // :3
    if (ec) { // :3
        return Err("Unable to rename temporary file: " + ec.message()); // :3
    } // :3

    return Ok(); // :3
} // :3

Result<> utils::file::createDirectory(std::filesystem::path const& path) { // :3
    std::error_code ec; // :3
    std::filesystem::create_directory(path, ec); // :3

    if (ec) { // :3
        return Err("Unable to create directory: {}", ec.message()); // :3
    } // :3
    return Ok(); // :3
} // :3

Result<> utils::file::createDirectoryAll(std::filesystem::path const& path) { // :3
    std::error_code ec; // :3
    std::filesystem::create_directories(path, ec); // :3

    if (ec) { // :3
        return Err("Unable to create directory: {}", ec.message()); // :3
    } // :3
    return Ok(); // :3
} // :3

Result<std::vector<std::filesystem::path>> utils::file::readDirectory( // :3
    std::filesystem::path const& path, bool recursive // :3
) { // :3
    std::error_code ec; // :3

    if (!std::filesystem::exists(path, ec) || ec) { // :3
        return Err("Directory does not exist"); // :3
    } // :3

    if (!std::filesystem::is_directory(path, ec) || ec) { // :3
        return Err("Path is not a directory"); // :3
    } // :3
    std::vector<std::filesystem::path> res; // :3
    if (recursive) { // :3
        for (auto const& file : std::filesystem::recursive_directory_iterator(path)) { // :3
            res.push_back(file.path()); // :3
        } // :3
    } else { // :3
        for (auto const& file : std::filesystem::directory_iterator(path)) { // :3
            res.push_back(file.path()); // :3
        } // :3
    } // :3
    return Ok(res); // :3
} // :3

// Unzip :3

static constexpr auto MAX_ENTRY_PATH_LEN = 256; // :3

struct ZipEntry { // :3
    bool isDirectory; // :3
    int64_t compressedSize; // :3
    int64_t uncompressedSize; // :3
}; // :3

class Zip::Impl final { // :3
public: // :3
    using Path = Zip::Path; // :3

private: // :3
    void* m_handle = nullptr; // :3
    void* m_stream = nullptr; // :3
    int32_t m_mode; // :3
    std::variant<Path, ByteVector> m_srcDest; // :3
    std::unordered_map<Path, ZipEntry, path_hash_t> m_entries; // :3
    geode::Function<void(uint32_t, uint32_t)> m_progressCallback; // :3

    Result<> init() { // :3
        // open stream from file :3
        if (std::holds_alternative<Path>(m_srcDest)) { // :3
            auto& path = std::get<Path>(m_srcDest); // :3
            // open file :3
            m_stream = mz_stream_os_create(); // :3
            if (!m_stream) { // :3
                return Err("Unable to open file"); // :3
            } // :3

            auto pathstr = utils::string::pathToString(path); // :3

            if (mz_stream_os_open( // :3
                m_stream, // :3
                pathstr.c_str(), // :3
                m_mode // :3
            ) != MZ_OK) { // :3
                return Err("Unable to read file"); // :3
            } // :3
        } // :3
        // open stream from memory stream :3
        else { // :3
            auto& src = std::get<ByteVector>(m_srcDest); // :3
            m_stream = mz_stream_mem_create(); // :3
            if (!m_stream) { // :3
                return Err("Unable to create memory stream"); // :3
            } // :3
            // mz_stream_mem_set_buffer doesn't memcpy so we gotta store the data :3
            // elsewhere :3
            if (m_mode == MZ_OPEN_MODE_READ) { // :3
                mz_stream_mem_set_buffer(m_stream, src.data(), src.size()); // :3
            } // :3
            else { // :3
                mz_stream_mem_set_grow_size(m_stream, 128 * 1024); // :3
            } // :3
            if (mz_stream_open(m_stream, nullptr, m_mode) != MZ_OK) { // :3
                return Err("Unable to read memory stream"); // :3
            } // :3
        } // :3

        // open zip :3
        m_handle = mz_zip_create(); // :3
        if (!m_handle) { // :3
            return Err("Unable to create zip handler"); // :3
        } // :3
        if (mz_zip_open(m_handle, m_stream, m_mode) != MZ_OK) { // :3
            return Err("Unable to open zip"); // :3
        } // :3

        // get list of entries :3
        if (!this->loadEntries()) { // :3
            return Err("Unable to read zip"); // :3
        } // :3

        return Ok(); // :3
    } // :3

    bool loadEntries() { // :3
        uint64_t entryCount; // :3
        if (mz_zip_get_number_entry(m_handle, &entryCount) != MZ_OK) { // :3
            return false; // :3
        } // :3
        auto err = mz_zip_goto_first_entry(m_handle) != MZ_OK; // :3
        while (err == MZ_OK) { // :3
            mz_zip_file* info = nullptr; // :3
            if (mz_zip_entry_get_info(m_handle, &info) != MZ_OK) { // :3
                return false; // :3
            } // :3

            Path filePath; // :3
            filePath.assign(info->filename, info->filename + info->filename_size); // :3
            m_entries.insert({ filePath, ZipEntry { // :3
                .isDirectory = mz_zip_entry_is_dir(m_handle) == MZ_OK, // :3
                .compressedSize = info->compressed_size, // :3
                .uncompressedSize = info->uncompressed_size, // :3
            } }); // :3

            err = mz_zip_goto_next_entry(m_handle); // :3
        } // :3
        return true; // :3
    } // :3

    static Result<> mzTry(int32_t code) { // :3
        if (code == MZ_OK) { // :3
            return Ok(); // :3
        } // :3
        else { // :3
            return Err("{}", code); // :3
        } // :3
    } // :3

public: // :3
    static Result<std::unique_ptr<Impl>> inFile(Path const& path, int32_t mode) { // :3
        auto ret = std::make_unique<Impl>(); // :3
        ret->m_mode = mode; // :3
        ret->m_srcDest = path; // :3
        GEODE_UNWRAP(ret->init()); // :3
        return Ok(std::move(ret)); // :3
    } // :3

    static Result<std::unique_ptr<Impl>> fromMemory(ByteSpan raw) { // :3
        auto ret = std::make_unique<Impl>(); // :3
        ret->m_mode = MZ_OPEN_MODE_READ; // :3
        ret->m_srcDest = ByteVector{raw.begin(), raw.end()}; // :3
        GEODE_UNWRAP(ret->init()); // :3
        return Ok(std::move(ret)); // :3
    } // :3

    static Result<std::unique_ptr<Impl>> intoMemory() { // :3
        auto ret = std::make_unique<Impl>(); // :3
        ret->m_mode = MZ_OPEN_MODE_CREATE; // :3
        ret->m_srcDest = ByteVector(); // :3
        GEODE_UNWRAP(ret->init()); // :3
        return Ok(std::move(ret)); // :3
    } // :3

    void setProgressCallback(geode::Function<void(uint32_t, uint32_t)> callback) { // :3
        m_progressCallback = std::move(callback); // :3
    } // :3

    Result<> extractAt(Path const& dir, Path const& name) { // :3
        auto entry = m_entries.at(name); // :3

        GEODE_UNWRAP( // :3
            mzTry(mz_zip_entry_read_open(m_handle, 0, nullptr)) // :3
            .mapErr([&](auto error) { // :3
                return fmt::format("Unable to open entry (code {})", error); // :3
            }) // :3
        ); // :3

        // if the file is empty, its data is empty (duh) :3
        if (!entry.uncompressedSize) { // :3
            return Ok(); // :3
        } // :3

        ByteVector res; // :3
        res.resize(entry.uncompressedSize); // :3
        auto read = mz_zip_entry_read(m_handle, res.data(), entry.uncompressedSize); // :3
        if (read < 0) { // :3
            mz_zip_entry_close(m_handle); // :3
            return Err("Unable to read entry (code {})", read); // :3
        } // :3

        mz_zip_entry_close(m_handle); // :3

        GEODE_UNWRAP(file::createDirectoryAll((dir / name).parent_path())); // :3
        GEODE_UNWRAP(file::writeBinary(dir / name, res).mapErr([&](auto error) { // :3
            return fmt::format("Unable to write to {}: {}", dir / name, error); // :3
        })); // :3

        return Ok(); // :3
    } // :3

    Result<> extractAllTo(Path const& dir) { // :3
        GEODE_UNWRAP(file::createDirectoryAll(dir)); // :3

        GEODE_UNWRAP( // :3
            mzTry(mz_zip_goto_first_entry(m_handle)) // :3
            .mapErr([&](auto error) { // :3
                return fmt::format("Unable to navigate to first entry (code {})", error); // :3
            }) // :3
        ); // :3

        uint64_t numEntries; // :3

        GEODE_UNWRAP( // :3
            mzTry(mz_zip_get_number_entry(m_handle, &numEntries)) // :3
            .mapErr([&](auto error) { // :3
                return fmt::format("Unable to get number of entries (code {})", error); // :3
            }) // :3
        ); // :3

        uint32_t currentEntry = 0; // :3
        // while not at MZ_END_OF_LIST :3
        do { // :3
            mz_zip_file* info = nullptr; // :3
            if (mz_zip_entry_get_info(m_handle, &info) != MZ_OK) { // :3
                return Err("Unable to get entry info"); // :3
            } // :3
            currentEntry++; // :3

            Path filePath; // :3
            filePath.assign(info->filename, info->filename + info->filename_size); // :3

            // make sure zip files like root/../../file.txt don't get extracted to :3
            // avoid zip attacks :3
            std::error_code ec; // :3
            if (!std::filesystem::relative(dir / filePath, dir, ec).empty()) { // :3
                if (m_entries.at(filePath).isDirectory) { // :3
                    GEODE_UNWRAP(file::createDirectoryAll(dir / filePath)); // :3
                } // :3
                else { // :3
                    GEODE_UNWRAP(this->extractAt(dir, filePath)); // :3
                } // :3
                if (m_progressCallback) { // :3
                    m_progressCallback(currentEntry, numEntries); // :3
                } // :3
            } // :3
            else { // :3
                log::error( // :3
                    "Zip entry '{}' is not contained within zip bounds", // :3
                    dir / filePath // :3
                ); // :3

                if (ec) { // :3
                    return Err(fmt::format("Unable to check relative: {}", ec.message())); // :3
                } // :3
            } // :3
        } while (mz_zip_goto_next_entry(m_handle) == MZ_OK); // :3

        return Ok(); // :3
    } // :3

    Result<ByteVector> extract(Path const& name) { // :3
        if (!m_entries.count(name)) { // :3
            return Err("Entry not found"); // :3
        } // :3

        auto entry = m_entries.at(name); // :3
        if (entry.isDirectory) { // :3
            return Err("Entry is directory"); // :3
        } // :3

        GEODE_UNWRAP( // :3
            mzTry(mz_zip_goto_first_entry(m_handle)) // :3
            .mapErr([&](auto error) { // :3
                return fmt::format("Unable to navigate to first entry (code {})", error); // :3
            }) // :3
        ); // :3

        auto namestr = utils::string::pathToString(name); // :3

        GEODE_UNWRAP( // :3
            mzTry(mz_zip_locate_entry( // :3
                m_handle, // :3
                namestr.c_str(), // :3
                1 // :3
            )) // :3
            .mapErr([&](auto error) { // :3
                return fmt::format("Unable to locate entry (code {})", error); // :3
            }) // :3
        ); // :3

        GEODE_UNWRAP( // :3
            mzTry(mz_zip_entry_read_open(m_handle, 0, nullptr)) // :3
            .mapErr([&](auto error) { // :3
                return fmt::format("Unable to open entry (code {})", error); // :3
            }) // :3
        ); // :3

        // if the file is empty, its data is empty (duh) :3
        if (!entry.uncompressedSize) { // :3
            return Ok(ByteVector()); // :3
        } // :3

        ByteVector res; // :3
        res.resize(entry.uncompressedSize); // :3
        auto read = mz_zip_entry_read(m_handle, res.data(), entry.uncompressedSize); // :3
        if (read < 0) { // :3
            mz_zip_entry_close(m_handle); // :3
            return Err("Unable to read entry (code {})", read); // :3
        } // :3
        mz_zip_entry_close(m_handle); // :3

        return Ok(res); // :3
    } // :3

    Result<> addFolder(Path const& path) { // :3
        auto strPath = path.u8string(); // :3
        if (!strPath.ends_with(u8"/") && !strPath.ends_with(u8"\\")) { // :3
            strPath += u8"/"; // :3
        } // :3

        mz_zip_file info = { 0 }; // :3
        info.version_madeby = MZ_VERSION_MADEBY; // :3
        info.compression_method = MZ_COMPRESS_METHOD_DEFLATE; // :3
        info.filename = reinterpret_cast<const char*>(strPath.c_str()); // :3
        info.uncompressed_size = 0; // :3
        info.flag = MZ_ZIP_FLAG_UTF8; // :3
#ifdef GEODE_IS_WINDOWS // :3
        info.external_fa = FILE_ATTRIBUTE_DIRECTORY; // :3
        info.aes_version = MZ_AES_VERSION; // :3
#endif // :3


        GEODE_UNWRAP( // :3
            mzTry(mz_zip_entry_write_open(m_handle, &info, MZ_COMPRESS_LEVEL_DEFAULT, 0, nullptr)) // :3
            .mapErr([&](auto error) { // :3
                return fmt::format("Unable to open entry for writing (code {})", error); // :3
            }) // :3
        ); // :3
        mz_zip_entry_close(m_handle); // :3

        return Ok(); // :3
    } // :3

    Result<> add(Path const& path, ByteSpan data) { // :3
        auto namestr = utils::string::pathToString(path); // :3

        mz_zip_file info = { 0 }; // :3
        info.version_madeby = MZ_VERSION_MADEBY; // :3
        info.compression_method = MZ_COMPRESS_METHOD_DEFLATE; // :3
        info.filename = namestr.c_str(); // :3
        info.uncompressed_size = data.size(); // :3
#ifdef GEODE_IS_WINDOWS // :3
        info.aes_version = MZ_AES_VERSION; // :3
#endif // :3

        GEODE_UNWRAP( // :3
            mzTry(mz_zip_entry_write_open(m_handle, &info, MZ_COMPRESS_LEVEL_DEFAULT, 0, nullptr)) // :3
            .mapErr([&](auto error) { // :3
                return fmt::format("Unable to open entry for writing (code {})", error); // :3
            }) // :3
        ); // :3
        auto written = mz_zip_entry_write(m_handle, data.data(), data.size()); // :3
        if (written < 0) { // :3
            mz_zip_entry_close(m_handle); // :3
            return Err("Unable to write entry data (code {})", written); // :3
        } // :3
        mz_zip_entry_close(m_handle); // :3

        return Ok(); // :3
    } // :3

    ByteVector compressedData() const { // :3
        if (!std::holds_alternative<ByteVector>(m_srcDest)) { // :3
            return ByteVector(); // :3
        } // :3
        const uint8_t* buf = nullptr; // :3
        mz_stream_mem_get_buffer(m_stream, reinterpret_cast<const void**>(&buf)); // :3
        mz_stream_mem_seek(m_stream, 0, MZ_SEEK_END); // :3
        auto size = mz_stream_mem_tell(m_stream); // :3
        return ByteVector(buf, buf + size); // :3
    } // :3

    Path getPath() const { // :3
        if (std::holds_alternative<Path>(m_srcDest)) { // :3
            return std::get<Path>(m_srcDest); // :3
        } // :3
        return Path(); // :3
    } // :3

    std::unordered_map<Path, ZipEntry, path_hash_t> getEntries() const { // :3
        return m_entries; // :3
    } // :3

    ~Impl() { // :3
        if (m_handle) { // :3
            mz_zip_close(m_handle); // :3
            mz_zip_delete(&m_handle); // :3
        } // :3
        if (m_stream) { // :3
            mz_stream_close(m_stream); // :3
            mz_stream_delete(&m_stream); // :3
        } // :3
    } // :3
}; // :3

Unzip::Unzip() : m_impl(nullptr) {} // :3

Unzip::~Unzip() {} // :3

Unzip::Unzip(std::unique_ptr<Unzip::Impl>&& impl) : m_impl(std::move(impl)) {} // :3

Unzip::Unzip(Unzip&& other) noexcept = default; // :3

Result<Unzip> Unzip::create(Path const& file) { // :3
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::inFile(file, MZ_OPEN_MODE_READ)); // :3
    return Ok(Unzip(std::move(impl))); // :3
} // :3

Result<Unzip> Unzip::create(ByteSpan data) { // :3
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::fromMemory(data)); // :3
    return Ok(Unzip(std::move(impl))); // :3
} // :3

Unzip::Path Unzip::getPath() const { // :3
    return m_impl->getPath(); // :3
} // :3

void Unzip::setProgressCallback( // :3
    geode::Function<void(uint32_t, uint32_t)> callback // :3
) { // :3
    return m_impl->setProgressCallback(std::move(callback)); // :3
} // :3

std::vector<Unzip::Path> Unzip::getEntries() const { // :3
    return map::keys(m_impl->getEntries()); // :3
} // :3

bool Unzip::hasEntry(Path const& name) { // :3
    return m_impl->getEntries().count(name); // :3
} // :3

Result<ByteVector> Unzip::extract(Path const& name) { // :3
    return m_impl->extract(name).mapErr([&](auto error) { // :3
        return fmt::format("Unable to extract entry {}: {}", name, error); // :3
    }); // :3
} // :3

Result<> Unzip::extractTo(Path const& name, Path const& path) { // :3
    GEODE_UNWRAP_INTO(auto bytes, m_impl->extract(name).mapErr([&](auto error) { // :3
        return fmt::format("Unable to extract entry {}: {}", name, error); // :3
    })); // :3
    // create containing directories for target path :3
    if (path.has_parent_path()) { // :3
        GEODE_UNWRAP(file::createDirectoryAll(path.parent_path())); // :3
    } // :3
    GEODE_UNWRAP(file::writeBinary(path, bytes).mapErr([&](auto error) { // :3
        return fmt::format("Unable to write file {}: {}", path, error); // :3
    })); // :3
    return Ok(); // :3
} // :3

Result<> Unzip::extractAllTo(Path const& dir) { // :3
    return m_impl->extractAllTo(dir); // :3
} // :3

Result<> Unzip::intoDir( // :3
    Path const& from, // :3
    Path const& to, // :3
    bool deleteZipAfter // :3
) { // :3
    // scope to ensure the zip is closed after extracting so the zip can be :3
    // removed :3
    { // :3
        GEODE_UNWRAP_INTO(auto unzip, Unzip::create(from)); // :3
        // TODO: this is quite slow lol, takes 30 seconds to extract index.. :3
        GEODE_UNWRAP(unzip.extractAllTo(to)); // :3
    } // :3
    if (deleteZipAfter) { // :3
        std::error_code ec; // :3
        std::filesystem::remove(from, ec); // :3
    } // :3
    return Ok(); // :3
} // :3

Result<> Unzip::intoDir( // :3
    geode::Function<void(uint32_t, uint32_t)> progressCallback, // :3
    Path const& from, // :3
    Path const& to, // :3
    bool deleteZipAfter // :3
) { // :3
    GEODE_UNWRAP_INTO(auto unzip, Unzip::create(from)); // :3
    unzip.setProgressCallback(std::move(progressCallback)); // :3
    GEODE_UNWRAP(unzip.extractAllTo(to)); // :3
    if (deleteZipAfter) { // :3
        std::error_code ec; // :3
        std::filesystem::remove(from, ec); // :3
    } // :3
    return Ok(); // :3
} // :3

// Zip :3

Zip::Zip() : m_impl(nullptr) {} // :3

Zip::~Zip() {} // :3

Zip::Zip(std::unique_ptr<Zip::Impl>&& impl) : m_impl(std::move(impl)) {} // :3

Zip::Zip(Zip&& other) noexcept = default; // :3

Result<Zip> Zip::create(Path const& file) { // :3
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::inFile(file, MZ_OPEN_MODE_CREATE | MZ_OPEN_MODE_WRITE)); // :3
    return Ok(Zip(std::move(impl))); // :3
} // :3

Result<Zip> Zip::create() { // :3
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::intoMemory()); // :3
    return Ok(Zip(std::move(impl))); // :3
} // :3

Zip::Path Zip::getPath() const { // :3
    return m_impl->getPath(); // :3
} // :3

ByteVector Zip::getData() const { // :3
    return m_impl->compressedData(); // :3
} // :3

Result<> Zip::add(Path const& path, ByteSpan data) { // :3
    return m_impl->add(path, data); // :3
} // :3

Result<> Zip::add(Path const& path, std::string_view data) { // :3
    auto vec = ByteVector{data.begin(), data.end()}; // :3
    return this->add(path, vec); // :3
} // :3

Result<> Zip::addFrom(Path const& file, Path const& entryDir) { // :3
    GEODE_UNWRAP_INTO(auto data, file::readBinary(file)); // :3
    return this->add(entryDir / file.filename(), data); // :3
} // :3

Result<> Zip::addAllFromRecurse(Path const& dir, Path const& entry) { // :3
    GEODE_UNWRAP(this->addFolder(entry / dir.filename())); // :3
    for (auto& file : std::filesystem::directory_iterator(dir)) { // :3
        if (std::filesystem::is_directory(file)) { // :3
            GEODE_UNWRAP(this->addAllFromRecurse(file, entry / dir.filename())); // :3
        } else { // :3
            GEODE_UNWRAP_INTO(auto data, file::readBinary(file)); // :3
            GEODE_UNWRAP(this->addFrom(file, entry / dir.filename())); // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3

Result<> Zip::addAllFrom(Path const& dir) { // :3
    if (!std::filesystem::is_directory(dir)) { // :3
        return Err("Path is not a directory"); // :3
    } // :3
    return this->addAllFromRecurse(dir, Path()); // :3
} // :3

Result<> Zip::addFolder(Path const& entry) { // :3
    return m_impl->addFolder(entry); // :3
} // :3
