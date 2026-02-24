#pragma once // :3

#include <Geode/Result.hpp> // :3
#include "general.hpp" // :3
#include "../loader/Event.hpp" // :3
#include "Task.hpp" // :3

#include <matjson.hpp> // :3
#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <Geode/utils/async.hpp> // :3
#include <filesystem> // :3
#include <string> // :3
#include <unordered_set> // :3

template <> // :3
struct matjson::Serialize<std::filesystem::path> { // :3
    static geode::Result<std::filesystem::path> fromJson(matjson::Value const& value) { // :3
        GEODE_UNWRAP_INTO(const std::string str, value.asString()); // :3

#ifdef GEODE_IS_WINDOWS // :3
        // On Windows, paths are stored as utf16, and matjson uses utf8 internally :3
        // This is not an issue until paths actually use unicode characters :3
        // So we do this conversion to make sure it stores the characters correctly :3
        return geode::Ok( // :3
            std::filesystem::path(geode::utils::string::utf8ToWide(str)).make_preferred() // :3
        ); // :3
#else // :3
        return geode::Ok(std::filesystem::path(str).make_preferred()); // :3
#endif // :3
    } // :3

    static matjson::Value toJson(std::filesystem::path const& value) { // :3
        // On Windows, paths are stored as utf16, and matjson uses utf8 internally :3
        // This is not an issue until paths actually use unicode characters :3
        // So we do this conversion to make sure it stores the characters correctly :3
        return matjson::Value(geode::utils::string::pathToString(value)); // :3
    } // :3
}; // :3

namespace geode::utils::file { // :3
    GEODE_DLL Result<std::string> readString(std::filesystem::path const& path); // :3
    GEODE_DLL Result<matjson::Value> readJson(std::filesystem::path const& path); // :3
    GEODE_DLL Result<ByteVector> readBinary(std::filesystem::path const& path); // :3

    template <class T> // :3
    Result<T> readFromJson(std::filesystem::path const& file) { // :3
        GEODE_UNWRAP_INTO(auto json, readJson(file)); // :3
        return json.as<T>(); // :3
    } // :3

    /**
     * Write a string to a file
     *
     * @param path Path to the file to write to
     * @param data Data to write to the file
     * @returns Result indicating success or failure
     */ // :3
    GEODE_DLL Result<> writeString(std::filesystem::path const& path, std::string_view data); // :3

    /**
     * Write a string to a file. Unlike the regular writeString, it first writes to a temporary file
     * and then renames it to the target file. This ensures that if the write fails, the original file
     * is not corrupted. Except Android where the "safe" part is disabled due to performance issues.
     *
     * @param path Path to the file to write to
     * @param data Data to write to the file
     * @returns Result indicating success or failure
     */ // :3
    GEODE_DLL Result<> writeStringSafe(std::filesystem::path const& path, std::string_view data); // :3

    /**
     * Write binary data to a file
     *
     * @param path Path to the file to write to
     * @param data Data to write to the file
     * @returns Result indicating success or failure
     */ // :3
    GEODE_DLL Result<> writeBinary(std::filesystem::path const& path, ByteSpan data); // :3

    /**
     * Write binary data to a file. Unlike the regular writeBinary, it first writes to a temporary file
     * and then renames it to the target file. This ensures that if the write fails, the original file
     * is not corrupted. Except Android where the "safe" part is disabled due to performance issues.
     *
     * @param path Path to the file to write to
     * @param data Data to write to the file
     * @returns Result indicating success or failure
     */ // :3
    GEODE_DLL Result<> writeBinarySafe(std::filesystem::path const& path, ByteSpan data); // :3

    template <class T> // :3
    Result<> writeToJson(std::filesystem::path const& path, T const& data) { // :3
        GEODE_UNWRAP(writeString(path, matjson::Value(data).dump())); // :3
        return Ok(); // :3
    } // :3

    GEODE_DLL Result<> createDirectory(std::filesystem::path const& path); // :3
    GEODE_DLL Result<> createDirectoryAll(std::filesystem::path const& path); // :3
    GEODE_DLL Result<std::vector<std::filesystem::path>> readDirectory( // :3
        std::filesystem::path const& path, bool recursive = false // :3
    ); // :3

    class Unzip; // :3

    class GEODE_DLL Zip final { // :3
    public: // :3
        using Path = std::filesystem::path; // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

        Zip(); // :3
        Zip(std::unique_ptr<Impl>&& impl); // :3

        Result<> addAllFromRecurse( // :3
            Path const& dir, Path const& entry // :3
        ); // :3

        // for sharing Impl :3
        friend class Unzip; // :3

    public: // :3
        Zip(Zip const&) = delete; // :3
        Zip(Zip&& other) noexcept; // :3
        ~Zip(); // :3

        /**
         * Create zipper for file
         */ // :3
        static Result<Zip> create(Path const& file); // :3

        /**
         * Create zipper for in-memory data
         */ // :3
        static Result<Zip> create(); // :3

        /**
         * Path to the created zip
         * @returns The path to the zip that is being created, or an empty path
         * if the zip was opened in memory
         */ // :3
        Path getPath() const; // :3

        /**
         * Get the zipped data
         */ // :3
        ByteVector getData() const; // :3

        /**
         * Add an entry to the zip with data
         */ // :3
        Result<> add(Path const& entry, ByteSpan data); // :3
        /**
         * Add an entry to the zip with string data
         */ // :3
        Result<> add(Path const& entry, std::string_view data); // :3
        /**
         * Add an entry to the zip from a file on disk. If you want to add the
         * file with a different name, read it into memory first and add it
         * with Zip::add
         * @param file File on disk
         * @param entryDir Folder to place the file in in the zip
         */ // :3
        Result<> addFrom(Path const& file, Path const& entryDir = Path()); // :3
        /**
         * Add an entry to the zip from a directory on disk
         * @param entry Path in the zip
         * @param dir Directory on disk
         */ // :3
        Result<> addAllFrom(Path const& dir); // :3
        /**
         * Add a folder entry to the zip. If you want to add a folder from disk,
         * use Zip::addAllFrom
         * @param entry Folder path in zip
         */ // :3
        Result<> addFolder(Path const& entry); // :3
    }; // :3

    class GEODE_DLL Unzip final { // :3
    private: // :3
        using Impl = Zip::Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

        Unzip(); // :3
        Unzip(std::unique_ptr<Impl>&& impl); // :3

    public: // :3
        Unzip(Unzip const&) = delete; // :3
        Unzip(Unzip&& other) noexcept; // :3
        ~Unzip(); // :3

        using Path = std::filesystem::path; // :3

        /**
         * Create unzipper for file
         */ // :3
        static Result<Unzip> create(Path const& file); // :3

        /**
         * Create unzipper for data in-memory
         */ // :3
        static Result<Unzip> create(ByteSpan data); // :3

        /**
         * Set a callback to be called with the progress of the unzip operation, first
         * argument is the current entry, second argument is the total entries
         * @note This is not thread-safe
         * @param callback Callback to call with the progress of the unzip operation
         */ // :3
        void setProgressCallback( // :3
            geode::Function<void(uint32_t, uint32_t)> callback // :3
        ); // :3

        /**
         * Path to the opened zip
         * @returns The path to the zip that is being read, or an empty path
         * if the zip was opened in memory
         */ // :3
        Path getPath() const; // :3

        /**
         * Get all entries in zip
         */ // :3
        std::vector<Path> getEntries() const; // :3
        /**
         * Check if zip has entry
         * @param name Entry path in zip
         */ // :3
        bool hasEntry(Path const& name); // :3

        /**
         * Extract entry to memory
         * @param name Entry path in zip
         */ // :3
        Result<ByteVector> extract(Path const& name); // :3
        /**
         * Extract entry to file
         * @param name Entry path in zip
         * @param path Target file path
         */ // :3
        Result<> extractTo(Path const& name, Path const& path); // :3
        /**
         * Extract all entries to directory
         * @param dir Directory to unzip the contents to
         */ // :3
        Result<> extractAllTo(Path const& dir); // :3

        /**
         * Helper method for quickly unzipping a file
         * @param from ZIP file to unzip
         * @param to Directory to unzip to
         * @param deleteZipAfter Whether to delete the zip after unzipping
         * @returns Successful result on success, errorful result on error
         */ // :3
        static Result<> intoDir( // :3
            Path const& from, // :3
            Path const& to, // :3
            bool deleteZipAfter = false // :3
        ); // :3

        static Result<> intoDir( // :3
            geode::Function<void(uint32_t, uint32_t)> progressCallback, // :3
            Path const& from, // :3
            Path const& to, // :3
            bool deleteZipAfter = false // :3
        ); // :3
    }; // :3

    /**
     * Open a folder / file in the system's file explorer
     * @param path Folder / file to open
     */ // :3
    GEODE_DLL bool openFolder(std::filesystem::path const& path); // :3

    enum class PickMode { // :3
        OpenFile, // :3
        SaveFile, // :3
        OpenFolder, // :3
    }; // :3

    struct FilePickOptions { // :3
        struct Filter { // :3
            // Name of the filter :3
            std::string description; // :3
            // Extensions (*.txt, *.doc, *.mp3, etc.) :3
            std::unordered_set<std::string> files; // :3
        }; // :3

        /**
         * On PickMode::SaveFile and PickMode::OpenFile, last item is assumed
         * to be a filename, unless it points to an extant directory.
         * On PickMode::OpenFolder, path is treated as leading up to a directory
         */ // :3
        std::optional<std::filesystem::path> defaultPath; // :3
        /**
         * File extension filters to show on the file picker
         */ // :3
        std::vector<Filter> filters; // :3
    }; // :3

    using PickResult = Result<std::optional<std::filesystem::path>>; // :3
    using PickManyResult = Result<std::vector<std::filesystem::path>>; // :3

    /**
     * Prompt the user to pick a file using the system's file system picker
     * @param mode Type of file selection prompt to show
     * @param options Picker options
     * @returns The picked file path, or std::nullopt if the dialog was cancelled
     */ // :3
    GEODE_DLL arc::Future<PickResult> pick(PickMode mode, FilePickOptions options); // :3

    /**
     * Prompt the user to pick a bunch of files for opening using the system's file system picker
     * @param options Picker options
     * @returns The picked file paths, or empty vector if the dialog was cancelled
     */ // :3
    GEODE_DLL arc::Future<PickManyResult> pickMany(FilePickOptions options); // :3
} // :3
