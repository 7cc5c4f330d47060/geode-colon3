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

#ifdef _MSC_VER // :3
    #pragma warning(disable : 4251) // :3
#endif // :3

#include <../platform/CCFileUtils.h> // :3
#include <../platform/IncludeZlib.h> // :3
#include <../support/zip_support/ZipUtils.h> // :3
#include <../support/zip_support/ioapi.h> // :3
#include <../support/zip_support/unzip.h> // :3
#include <Geode/c++stl/gdstdlib.hpp> // :3
#include <assert.h> // :3
#include <ccMacros.h> // :3
#include <map> // :3
#include <stdlib.h> // :3

NS_CC_BEGIN // :3

unsigned int ZipUtils::s_uEncryptedPvrKeyParts[4] = { 0, 0, 0, 0 }; // :3
unsigned int ZipUtils::s_uEncryptionKey[1024]; // :3
bool ZipUtils::s_bEncryptionKeyIsValid = false; // :3

// --------------------- ZipUtils --------------------- :3

inline void ZipUtils::ccDecodeEncodedPvr(unsigned int* data, int len) { // :3
    int const enclen = 1024; // :3
    int const securelen = 512; // :3
    int const distance = 64; // :3

    // check if key was set :3
    // make sure to call caw_setkey_part() for all 4 key parts :3
    CCAssert( // :3
        s_uEncryptedPvrKeyParts[0] != 0, // :3
        "Cocos2D: CCZ file is encrypted but key part 0 is not set. Did you call " // :3
        "ZipUtils::ccSetPvrEncryptionKeyPart(...)?" // :3
    ); // :3
    CCAssert( // :3
        s_uEncryptedPvrKeyParts[1] != 0, // :3
        "Cocos2D: CCZ file is encrypted but key part 1 is not set. Did you call " // :3
        "ZipUtils::ccSetPvrEncryptionKeyPart(...)?" // :3
    ); // :3
    CCAssert( // :3
        s_uEncryptedPvrKeyParts[2] != 0, // :3
        "Cocos2D: CCZ file is encrypted but key part 2 is not set. Did you call " // :3
        "ZipUtils::ccSetPvrEncryptionKeyPart(...)?" // :3
    ); // :3
    CCAssert( // :3
        s_uEncryptedPvrKeyParts[3] != 0, // :3
        "Cocos2D: CCZ file is encrypted but key part 3 is not set. Did you call " // :3
        "ZipUtils::ccSetPvrEncryptionKeyPart(...)?" // :3
    ); // :3

    // create long key :3
    if (!s_bEncryptionKeyIsValid) { // :3
        unsigned int y, p, e; // :3
        unsigned int rounds = 6; // :3
        unsigned int sum = 0; // :3
        unsigned int z = s_uEncryptionKey[enclen - 1]; // :3

        do { // :3
#define DELTA 0x9e3779b9 // :3
#define MX                                     \
    (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) ^ \
     ((sum ^ y) + (s_uEncryptedPvrKeyParts[(p & 3) ^ e] ^ z))) // :3

            sum += DELTA; // :3
            e = (sum >> 2) & 3; // :3

            for (p = 0; p < enclen - 1; p++) { // :3
                y = s_uEncryptionKey[p + 1]; // :3
                z = s_uEncryptionKey[p] += MX; // :3
            } // :3

            y = s_uEncryptionKey[0]; // :3
            z = s_uEncryptionKey[enclen - 1] += MX; // :3

        } while (--rounds); // :3

        s_bEncryptionKeyIsValid = true; // :3
    } // :3

    int b = 0; // :3
    int i = 0; // :3

    // encrypt first part completely :3
    for (; i < len && i < securelen; i++) { // :3
        data[i] ^= s_uEncryptionKey[b++]; // :3

        if (b >= enclen) { // :3
            b = 0; // :3
        } // :3
    } // :3

    // encrypt second section partially :3
    for (; i < len; i += distance) { // :3
        data[i] ^= s_uEncryptionKey[b++]; // :3

        if (b >= enclen) { // :3
            b = 0; // :3
        } // :3
    } // :3
} // :3

inline unsigned int ZipUtils::ccChecksumPvr(unsigned int const* data, int len) { // :3
    unsigned int cs = 0; // :3
    int const cslen = 128; // :3

    len = (len < cslen) ? len : cslen; // :3

    for (int i = 0; i < len; i++) { // :3
        cs = cs ^ data[i]; // :3
    } // :3

    return cs; // :3
} // :3

// memory in iPhone is precious :3
// Should buffer factor be 1.5 instead of 2 ? :3
#define BUFFER_INC_FACTOR (2) // :3

int ZipUtils::ccInflateMemoryWithHint( // :3
    unsigned char* in, unsigned int inLength, unsigned char** out, unsigned int* outLength, // :3
    unsigned int outLenghtHint // :3
) { // :3
    /* ret value */
    int err = Z_OK;

    int bufferSize = outLenghtHint;
    *out = new unsigned char[bufferSize];

    z_stream d_stream; /* decompression stream */
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;

    d_stream.next_in = in;
    d_stream.avail_in = inLength;
    d_stream.next_out = *out;
    d_stream.avail_out = bufferSize;

    /* window size to hold 256k */
    if ((err = inflateInit2(&d_stream, 15 + 32)) != Z_OK) return err;

    for (;;) {
        err = inflate(&d_stream, Z_NO_FLUSH);

        if (err == Z_STREAM_END) {
            break;
        }

        switch (err) {
            case Z_NEED_DICT: err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR: inflateEnd(&d_stream); return err;
        }

        // not enough memory ?
        if (err != Z_STREAM_END) {
            *out = (unsigned char*)realloc(*out, bufferSize * BUFFER_INC_FACTOR);

            /* not enough memory, ouch */
            if (!*out) {
                CCLOG("cocos2d: ZipUtils: realloc failed");
                inflateEnd(&d_stream);
                return Z_MEM_ERROR;
            }

            d_stream.next_out = *out + bufferSize;
            d_stream.avail_out = bufferSize;
            bufferSize *= BUFFER_INC_FACTOR;
        }
    }

    *outLength = bufferSize - d_stream.avail_out;
    err = inflateEnd(&d_stream);
    return err;
}

int ZipUtils::ccInflateMemoryWithHint(
    unsigned char* in, unsigned int inLength, unsigned char** out, unsigned int outLengthHint
) {
    unsigned int outLength = 0;
    int err = ccInflateMemoryWithHint(in, inLength, out, &outLength, outLengthHint);

    if (err != Z_OK || *out == NULL) {
        if (err == Z_MEM_ERROR) {
            CCLOG("cocos2d: ZipUtils: Out of memory while decompressing map data!");
        }
        else if (err == Z_VERSION_ERROR) {
            CCLOG("cocos2d: ZipUtils: Incompatible zlib version!");
        }
        else if (err == Z_DATA_ERROR) {
            CCLOG("cocos2d: ZipUtils: Incorrect zlib compressed data!");
        }
        else {
            CCLOG("cocos2d: ZipUtils: Unknown error while decompressing map data!");
        }

        delete[] * out;
        *out = NULL;
        outLength = 0;
    }

    return outLength;
}

int ZipUtils::ccInflateMemory(unsigned char* in, unsigned int inLength, unsigned char** out) {
    // 256k for hint
    return ccInflateMemoryWithHint(in, inLength, out, 256 * 1024);
}

int ZipUtils::ccInflateGZipFile(char const* path, unsigned char** out) {
    int len;
    unsigned int offset = 0;

    CCAssert(out, "");
    CCAssert(&*out, "");

    gzFile inFile = gzopen(path, "rb");
    if (inFile == NULL) {
        CCLOG("cocos2d: ZipUtils: error open gzip file: %s", path);
        return -1;
    }

    /* 512k initial decompress buffer */
    unsigned int bufferSize = 512 * 1024;
    unsigned int totalBufferSize = bufferSize;

    *out = (unsigned char*)malloc(bufferSize);
    if (!out) {
        CCLOG("cocos2d: ZipUtils: out of memory");
        return -1;
    }

    for (;;) {
        len = gzread(inFile, *out + offset, bufferSize);
        if (len < 0) {
            CCLOG("cocos2d: ZipUtils: error in gzread");
            free(*out);
            *out = NULL;
            return -1;
        }
        if (len == 0) {
            break;
        }

        offset += len;

        // finish reading the file
        if ((unsigned int)len < bufferSize) {
            break;
        }

        bufferSize *= BUFFER_INC_FACTOR;
        totalBufferSize += bufferSize;
        unsigned char* tmp = (unsigned char*)realloc(*out, totalBufferSize);

        if (!tmp) {
            CCLOG("cocos2d: ZipUtils: out of memory");
            free(*out);
            *out = NULL;
            return -1;
        }

        *out = tmp;
    }

    if (gzclose(inFile) != Z_OK) {
        CCLOG("cocos2d: ZipUtils: gzclose failed");
    }

    return offset;
}

int ZipUtils::ccInflateCCZFile(char const* path, unsigned char** out) {
    CCAssert(out, "");
    CCAssert(&*out, "");

    // load file into memory
    unsigned char* compressed = NULL;

    unsigned long fileLen = 0;
    compressed = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileLen);

    if (NULL == compressed || 0 == fileLen) {
        CCLOG("cocos2d: Error loading CCZ compressed file");
        return -1;
    }

    struct CCZHeader* header = (struct CCZHeader*)compressed;

    // verify header
    if (header->sig[0] == 'C' && header->sig[1] == 'C' && header->sig[2] == 'Z' &&
        header->sig[3] == '!') {
        // verify header version
        unsigned int version = CC_SWAP_INT16_BIG_TO_HOST(header->version);
        if (version > 2) {
            CCLOG("cocos2d: Unsupported CCZ header format");
            delete[] compressed;
            return -1;
        }

        // verify compression format
        if (CC_SWAP_INT16_BIG_TO_HOST(header->compression_type) != CCZ_COMPRESSION_ZLIB) {
            CCLOG("cocos2d: CCZ Unsupported compression method");
            delete[] compressed;
            return -1;
        }
    }
    else if (header->sig[0] == 'C' && header->sig[1] == 'C' && header->sig[2] == 'Z' && header->sig[3] == 'p') {
        // encrypted ccz file
        header = (struct CCZHeader*)compressed;

        // verify header version
        unsigned int version = CC_SWAP_INT16_BIG_TO_HOST(header->version);
        if (version > 0) {
            CCLOG("cocos2d: Unsupported CCZ header format");
            delete[] compressed;
            return -1;
        }

        // verify compression format
        if (CC_SWAP_INT16_BIG_TO_HOST(header->compression_type) != CCZ_COMPRESSION_ZLIB) {
            CCLOG("cocos2d: CCZ Unsupported compression method");
            delete[] compressed;
            return -1;
        }

        // decrypt
        unsigned int* ints = (unsigned int*)(compressed + 12);
        int enclen = (fileLen - 12) / 4;

        ccDecodeEncodedPvr(ints, enclen);

#if COCOS2D_DEBUG > 0
        // verify checksum in debug mode
        unsigned int calculated = ccChecksumPvr(ints, enclen);
        unsigned int required = CC_SWAP_INT32_BIG_TO_HOST(header->reserved);

        if (calculated != required) {
            CCLOG("cocos2d: Can't decrypt image file. Is the decryption key valid?");
            delete[] compressed;
            return -1;
        }
#endif
    }
    else {
        CCLOG("cocos2d: Invalid CCZ file");
        delete[] compressed;
        return -1;
    }

    unsigned int len = CC_SWAP_INT32_BIG_TO_HOST(header->len);

    *out = (unsigned char*)malloc(len);
    if (!*out) {
        CCLOG("cocos2d: CCZ: Failed to allocate memory for texture");
        delete[] compressed;
        return -1;
    }

    unsigned long destlen = len;
    uintptr_t source = (uintptr_t)compressed + sizeof(*header);
    int ret = uncompress(*out, &destlen, (Bytef*)source, fileLen - sizeof(*header));

    delete[] compressed;

    if (ret != Z_OK) {
        CCLOG("cocos2d: CCZ: Failed to uncompress data");
        free(*out);
        *out = NULL;
        return -1;
    }

    return len;
}

void ZipUtils::ccSetPvrEncryptionKeyPart(int index, unsigned int value) {
    CCAssert(index >= 0, "Cocos2d: key part index cannot be less than 0");
    CCAssert(index <= 3, "Cocos2d: key part index cannot be greater than 3");

    if (s_uEncryptedPvrKeyParts[index] != value) {
        s_uEncryptedPvrKeyParts[index] = value;
        s_bEncryptionKeyIsValid = false;
    }
}

void ZipUtils::ccSetPvrEncryptionKey(
    unsigned int keyPart1, unsigned int keyPart2, unsigned int keyPart3, unsigned int keyPart4
) {
    ccSetPvrEncryptionKeyPart(0, keyPart1);
    ccSetPvrEncryptionKeyPart(1, keyPart2);
    ccSetPvrEncryptionKeyPart(2, keyPart3);
    ccSetPvrEncryptionKeyPart(3, keyPart4);
}

// --------------------- ZipFile ---------------------
// from unzip.cpp
#define UNZ_MAXFILENAMEINZIP 256

struct ZipEntryInfo {
    unz_file_pos pos;
    uLong uncompressed_size;
};

class ZipFilePrivate {
public:
    unzFile zipFile;

    // std::unordered_map is faster if available on the platform
    typedef std::map<std::string, struct ZipEntryInfo> FileListContainer;
    FileListContainer fileList;
};

ZipFile::ZipFile(std::string const& zipFile, std::string const& filter) :
    _data(new ZipFilePrivate), _dataThread(new ZipFilePrivate) {
    _data->zipFile = unzOpen(zipFile.c_str());
    _dataThread->zipFile = unzOpen(zipFile.c_str());
    if (_data->zipFile && _dataThread->zipFile) {
        setFilter(filter);
    }
}

ZipFile::~ZipFile() {
    if (_data && _data->zipFile) {
        unzClose(_data->zipFile);
    }
    if (_dataThread && _dataThread->zipFile) {
        unzClose(_dataThread->zipFile);
    }
    CC_SAFE_DELETE(_data);
    CC_SAFE_DELETE(_dataThread);
}

bool ZipFile::setFilter(std::string const& filter, ZipFilePrivate* data) {
    bool ret = false;
    do {
        CC_BREAK_IF(!data);
        CC_BREAK_IF(!data->zipFile);

        // clear existing file list
        data->fileList.clear();

        // UNZ_MAXFILENAMEINZIP + 1 - it is done so in unzLocateFile
        char szCurrentFileName[UNZ_MAXFILENAMEINZIP + 1];
        unz_file_info64 fileInfo;

        // go through all files and store position information about the required files
        int err = unzGoToFirstFile64(
            data->zipFile, &fileInfo, szCurrentFileName, sizeof(szCurrentFileName) - 1
        );
        while (err == UNZ_OK) {
            unz_file_pos posInfo;
            int posErr = unzGetFilePos(data->zipFile, &posInfo);
            if (posErr == UNZ_OK) {
                std::string currentFileName = szCurrentFileName;
                // cache info about filtered files only (like 'assets/')
                if (filter.empty() || currentFileName.substr(0, filter.length()) == filter) {
                    ZipEntryInfo entry;
                    entry.pos = posInfo;
                    entry.uncompressed_size = (uLong)fileInfo.uncompressed_size;
                    data->fileList[currentFileName] = entry;
                }
            }
            // next file - also get the information about it
            err = unzGoToNextFile64(
                data->zipFile, &fileInfo, szCurrentFileName, sizeof(szCurrentFileName) - 1
            );
        }
        ret = true;

    } while (false);

    return ret;
}

bool ZipFile::setFilter(std::string const& filter) {
    return (setFilter(filter, _data) && setFilter(filter, _dataThread));
}

bool ZipFile::fileExists(std::string const& fileName) const {
    bool ret = false;
    do {
        CC_BREAK_IF(!_data);

        ret = _data->fileList.find(fileName) != _data->fileList.end();
    } while (false);

    return ret;
}

unsigned char* ZipFile::getFileData(std::string const& fileName, unsigned long* pSize) {
    return getFileData(fileName, pSize, _data);
}

unsigned char* ZipFile::getFileData(
    std::string const& fileName, unsigned long* pSize, ZipFilePrivate* data
) {
    unsigned char* pBuffer = NULL;
    if (pSize) {
        *pSize = 0;
    }

    do {
        CC_BREAK_IF(!data->zipFile);
        CC_BREAK_IF(fileName.empty());

        ZipFilePrivate::FileListContainer::const_iterator it = data->fileList.find(fileName);
        CC_BREAK_IF(it == data->fileList.end());

        ZipEntryInfo fileInfo = it->second;

        int nRet = unzGoToFilePos(data->zipFile, &fileInfo.pos);
        CC_BREAK_IF(UNZ_OK != nRet);

        nRet = unzOpenCurrentFile(data->zipFile);
        CC_BREAK_IF(UNZ_OK != nRet);

        pBuffer = new unsigned char[fileInfo.uncompressed_size];
        int CC_UNUSED nSize =
            unzReadCurrentFile(data->zipFile, pBuffer, fileInfo.uncompressed_size);
        CCAssert(nSize == 0 || nSize == (int)fileInfo.uncompressed_size, "the file size is wrong");

        if (pSize) {
            *pSize = fileInfo.uncompressed_size;
        }
        unzCloseCurrentFile(data->zipFile);
    } while (0);

    return pBuffer;
}

bool ZipFile::isLoaded() const {
    return _data->zipFile && _dataThread->zipFile;
}

std::vector<std::string> ZipFile::getAllFiles() const {
    std::vector<std::string> res;
    for (auto [key, _] : _data->fileList) {
        res.push_back(key);
    }
    return res;
}

NS_CC_END
