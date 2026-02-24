/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/ // :3
#ifndef __SUPPORT_ZIPUTILS_H__ // :3
#define __SUPPORT_ZIPUTILS_H__ // :3

#include <string> // :3
#include <filesystem> // :3
#include "../../platform/CCPlatformDefine.h" // :3
#include "../../platform/CCPlatformConfig.h" // :3
#include "../../include/ccMacros.h" // :3
#include "zipMacro.h" // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // :3
#include "../../platform/android/CCFileUtilsAndroid.h" // :3
#endif // :3

namespace cocos2d // :3
{ // :3
    /* XXX: pragma pack ??? */
    /** @struct CCZHeader
    */ // :3
    struct CCZHeader { // :3
        unsigned char   sig[4];             // signature. Should be 'CCZ!' 4 bytes :3
        unsigned short  compression_type;   // should 0 :3
        unsigned short  version;            // should be 2 (although version type==1 is also supported) :3
        unsigned int    reserved;           // Reserved for users. :3
        unsigned int    len;                // size of the uncompressed file :3
    }; // :3

    enum { // :3
        CCZ_COMPRESSION_ZLIB,               // zlib format. :3
        CCZ_COMPRESSION_BZIP2,              // bzip2 format (not supported yet) :3
        CCZ_COMPRESSION_GZIP,               // gzip format (not supported yet) :3
        CCZ_COMPRESSION_NONE,               // plain (not supported yet) :3
    }; // :3

    class CC_DLL ZipUtils // :3
    { // :3
        GEODE_FRIEND_MODIFY; // :3
    public: // :3
        /**
        * Inflates either zlib or gzip deflated memory. The inflated memory is
        * expected to be freed by the caller.
        *
        * It will allocate 256k for the destination buffer. If it is not enough it will multiply the previous buffer size per 2, until there is enough memory.
        * @returns the length of the deflated buffer
        *
        @since v0.8.1
        */ // :3
        static int ccInflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out); // :3

        /**
        * Inflates either zlib or gzip deflated memory. The inflated memory is
        * expected to be freed by the caller.
        *
        * outLenghtHint is assumed to be the needed room to allocate the inflated buffer.
        *
        * @returns the length of the deflated buffer
        *
        @since v1.0.0
        */ // :3
        static int ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int outLenghtHint); // :3

        /** inflates a GZip file into memory
        *
        * @returns the length of the deflated buffer
        *
        * @since v0.99.5
        */ // :3
        static int ccInflateGZipFile(const char *filename, unsigned char **out); // :3

        /** inflates a CCZ file into memory
        *
        * @returns the length of the deflated buffer
        *
        * @since v0.99.5
        */ // :3
        static int ccInflateCCZFile(const char *filename, unsigned char **out); // :3

        /** Sets the pvr.ccz encryption key parts separately for added
        * security.
        *
        * Example: If the key used to encrypt the pvr.ccz file is
        * 0xaaaaaaaabbbbbbbbccccccccdddddddd you will call this function 4
        * different times, preferably from 4 different source files, as follows
        *
        * ZipUtils::ccSetPvrEncryptionKeyPart(0, 0xaaaaaaaa);
        * ZipUtils::ccSetPvrEncryptionKeyPart(1, 0xbbbbbbbb);
        * ZipUtils::ccSetPvrEncryptionKeyPart(2, 0xcccccccc);
        * ZipUtils::ccSetPvrEncryptionKeyPart(3, 0xdddddddd);
        *
        * Splitting the key into 4 parts and calling the function
        * from 4 different source files increases the difficulty to
        * reverse engineer the encryption key. Be aware that encrpytion
        * is *never* 100% secure and the key code can be cracked by
        * knowledgable persons.
        *
        * IMPORTANT: Be sure to call ccSetPvrEncryptionKey or
        * ccSetPvrEncryptionKeyPart with all of the key parts *before* loading
        * the spritesheet or decryption will fail and the spritesheet
        * will fail to load.
        *
        * @param index part of the key [0..3]
        * @param value value of the key part
        */ // :3
        static void ccSetPvrEncryptionKeyPart(int index, unsigned int value); // :3

        /** Sets the pvr.ccz encryption key.
        *
        * Example: If the key used to encrypt the pvr.ccz file is
        * 0xaaaaaaaabbbbbbbbccccccccdddddddd you will call this function with
        * the key split into 4 parts as follows
        *
        * ZipUtils::ccSetPvrEncryptionKey(0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc, 0xdddddddd);
        *
        * Note that using this function makes it easier to reverse engineer and
        * discover the complete key because the key parts are present in one
        * function call.
        *
        * IMPORTANT: Be sure to call ccSetPvrEncryptionKey or
        * ccSetPvrEncryptionKeyPart with all of the key parts *before* loading
        * the spritesheet or decryption will fail and the spritesheet
        * will fail to load.
        *
        * @param keyPart1 the key value part 1.
        * @param keyPart2 the key value part 2.
        * @param keyPart3 the key value part 3.
        * @param keyPart4 the key value part 4.
        */ // :3
        static void ccSetPvrEncryptionKey(unsigned int keyPart1, unsigned int keyPart2, unsigned int keyPart3, unsigned int keyPart4); // :3

        static gd::string base64DecodeEnc(gd::string const&, gd::string); // :3
        static gd::string base64EncodeEnc(gd::string const&, gd::string); // :3
        static gd::string base64URLDecode(gd::string const&); // :3
        static gd::string base64URLEncode(gd::string const&); // :3
        static int ccDeflateMemory(unsigned char* data, unsigned int size, unsigned char** out); // :3
        static int ccDeflateMemoryWithHint(unsigned char*, unsigned int, unsigned char**, unsigned int); // :3
        static gd::string compressString(gd::string const& data, bool encrypt, int encryptionKey); // :3
        static gd::string decompressString(gd::string const& data, bool encrypt, int encryptionKey); // :3
        static gd::string decompressString2(unsigned char* data, bool encrypt, int size, int encryptionKey); // :3
        static gd::string encryptDecrypt(gd::string const& data, int encryptionKey); // :3
        static gd::string encryptDecryptWKey(gd::string const&, gd::string); // :3
        static unsigned char hexToChar(const gd::string&); // :3
        static gd::string urlDecode(const gd::string&); // :3

    private: // :3
        static int ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength, // :3
                                           unsigned int outLenghtHint); // :3
        static inline void ccDecodeEncodedPvr (unsigned int *data, int len); // :3
        static inline unsigned int ccChecksumPvr(const unsigned int *data, int len); // :3

        static unsigned int s_uEncryptedPvrKeyParts[4]; // :3
        static unsigned int s_uEncryptionKey[1024]; // :3
        static bool s_bEncryptionKeyIsValid; // :3
    }; // :3

    // forward declaration :3
    class ZipFilePrivate; // :3

    /**
    * Zip file - reader helper class.
    *
    * It will cache the file list of a particular zip file with positions inside an archive,
    * so it would be much faster to read some particular files or to check their existance.
    *
    * @since v2.0.5
    */ // :3
    class GEODE_DLL ZipFile // :3
    { // :3
    public: // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // :3
        friend class CCFileUtilsAndroid; // :3
#endif // :3

        /**
        * Constructor, open zip file and store file list.
        *
        * @param zipFile Zip file name
        * @param filter The first part of file names, which should be accessible.
        *               For example, "assets/". Other files will be missed.
        *
        * @since v2.0.5
        */ // :3
        ZipFile(const std::string &zipFile, const std::string &filter = std::string()); // :3
        virtual ~ZipFile(); // :3

        /**
         * Custom function added for geode; returns if the
         * zip file was successfully decoded.
         *
         * @return true if the zip was successfully loaded,
         *         false otherwise.
         *
         * @since geode v1.0.0
         */ // :3
        bool isLoaded() const; // :3

        bool unzipAllTo(std::filesystem::path const& path); // :3

        /**
        * Regenerate accessible file list based on a new filter string.
        *
        * @param filter New filter string (first part of files names)
        * @return true whenever zip file is open successfully and it is possible to locate
        *              at least the first file, false otherwise
        *
        * @since v2.0.5
        */ // :3
        bool setFilter(const std::string &filter); // :3

        /**
        * Check does a file exists or not in zip file
        *
        * @param fileName File to be checked on existance
        * @return true whenever file exists, false otherwise
        *
        * @since v2.0.5
        */ // :3
        bool fileExists(const std::string &fileName) const; // :3

        /**
        * Get resource file data from a zip file.
        * @param fileName File name
        * @param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
        * @return Upon success, a pointer to the data is returned, otherwise NULL.
        * @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
        *
        * @since v2.0.5
        */ // :3
        unsigned char *getFileData(const std::string &fileName, unsigned long *pSize); // :3

        /**
         * Custom function added for geode; returns all of
         * the files in the zip that match the current filter.
         *
         * @return Vector of filenames
         *
         * @since geode v1.0.0
         */ // :3
        std::vector<std::string> getAllFiles() const; // :3

    private: // :3
        bool setFilter(const std::string &filer, ZipFilePrivate *data); // :3
        unsigned char *getFileData(const std::string &fileName, unsigned long *pSize, ZipFilePrivate *data); // :3

        /** Internal data like zip file pointer / file list array and so on */
        ZipFilePrivate *_data;
        /** Another data used not in main thread */
        ZipFilePrivate *_dataThread;
    };
} // end of namespace cocos2d
#endif // __SUPPORT_ZIPUTILS_H__
