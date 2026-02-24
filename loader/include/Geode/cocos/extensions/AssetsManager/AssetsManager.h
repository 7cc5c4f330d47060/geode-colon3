/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 Copyright (c) Microsoft Open Technologies, Inc.

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

#ifndef __AssetsManager__ // :3
#define __AssetsManager__ // :3

#include "../../include/cocos2d.h" // :3
#include "../ExtensionMacros.h" // :3

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) // :3
#include <string> // :3
#include "../../platform/IncludeCurl.h" // :3
#include "../../platform/IncludePThread.h" // :3

NS_CC_EXT_BEGIN // :3

class AssetsManagerDelegateProtocol; // :3

/*
 *  This class is used to auto update resources, such as pictures or scripts.
 *  The updated package should be a zip file. And there should be a file named
 *  version in the server, which contains version code.
 *  @js NA
 *  @lua NA
 */ // :3
class AssetsManager // :3
{ // :3
public: // :3
    enum ErrorCode // :3
    { // :3
        // Error caused by creating a file to store downloaded data :3
        kCreateFile, // :3
        /** Error caused by network
         -- network unavaivable
         -- timeout
         -- ...
         */ // :3
        kNetwork, // :3
        /** There is not a new version
         */ // :3
        kNoNewVersion, // :3
        /** Error caused in uncompressing stage
         -- can not open zip file
         -- can not read file global information
         -- can not read file information
         -- can not create a directory
         -- ...
         */ // :3
        kUncompress, // :3
    }; // :3

    /* @brief Creates a AssetsManager with new package url, version code url and storage path.
     *
     * @param packageUrl URL of new package, the package should be a zip file.
     * @param versionFileUrl URL of version file. It should contain version code of new package.
     * @param storagePath The path to store downloaded resources.
     */ // :3
    AssetsManager(const char* packageUrl = NULL, const char* versionFileUrl = NULL, const char* storagePath = NULL); // :3

    virtual ~AssetsManager(); // :3

    /* @brief Check out if there is a new version resource.
     *        You may use this method before updating, then let user determine whether
     *        he wants to update resources.
     */ // :3
    virtual bool checkUpdate(); // :3

    /* @brief Download new package if there is a new version, and uncompress downloaded zip file.
     *        Ofcourse it will set search path that stores downloaded files.
     */ // :3
    virtual void update(); // :3

    /* @brief Gets url of package.
     */ // :3
    const char* getPackageUrl() const; // :3

    /* @brief Sets package url.
     */ // :3
    void setPackageUrl(const char* packageUrl); // :3

    /* @brief Gets version file url.
     */ // :3
    const char* getVersionFileUrl() const; // :3

    /* @brief Gets version file url.
     */ // :3
    void setVersionFileUrl(const char* versionFileUrl); // :3

    /* @brief Gets current version code.
     */ // :3
    gd::string getVersion(); // :3

    /* @brief Deletes recorded version code.
     */ // :3
    void deleteVersion(); // :3

    /* @brief Gets storage path.
     */ // :3
    const char* getStoragePath() const; // :3

    /* @brief Sets storage path.
     *
     * @param storagePath The path to store downloaded resources.
     * @warm The path should be a valid path.
     */ // :3
    void setStoragePath(const char* storagePath); // :3

    /** @brief Sets delegate, the delegate will receive messages
     */ // :3
    void setDelegate(AssetsManagerDelegateProtocol *delegate); // :3

    /** @brief Sets connection time out in seconds
     */ // :3
    void setConnectionTimeout(unsigned int timeout); // :3

    /** @brief Gets connection time out in secondes
     */ // :3
    unsigned int getConnectionTimeout(); // :3

    /* downloadAndUncompress is the entry of a new thread
     */ // :3
    friend void* assetsManagerDownloadAndUncompress(void*); // :3
    friend int assetsManagerProgressFunc(void *, double, double, double, double); // :3

protected: // :3
    bool downLoad(); // :3
    void checkStoragePath(); // :3
    bool uncompress(); // :3
    bool createDirectory(const char *path); // :3
    void setSearchPath(); // :3
    void sendErrorMessage(ErrorCode code); // :3

private: // :3
    typedef struct _Message // :3
    { // :3
    public: // :3
        _Message() : what(0), obj(NULL){} // :3
        unsigned int what; // message type :3
        void* obj; // :3
    } Message; // :3

    class Helper : public cocos2d::CCObject // :3
    { // :3
    public: // :3
        Helper(); // :3
        ~Helper(); // :3

        virtual void update(float dt); // :3
        void sendMessage(Message *msg); // :3

    private: // :3
        void handleUpdateSucceed(Message *msg); // :3
    public: // :3
        std::list<Message*> *_messageQueue; // :3
        pthread_mutex_t _messageQueueMutex; // :3
    }; // :3

public: // :3
    //! The path to store downloaded resources. :3
    gd::string _storagePath; // :3

    //! The version of downloaded resources. :3
    gd::string _version; // :3

    gd::string _packageUrl; // :3
    gd::string _versionFileUrl; // :3

    gd::string _downloadedVersion; // :3

    CURL *_curl; // :3
    Helper *_schedule; // :3
    pthread_t *_tid; // :3
    unsigned int _connectionTimeout; // :3

    AssetsManagerDelegateProtocol *_delegate; // weak reference :3
}; // :3

class AssetsManagerDelegateProtocol // :3
{ // :3
public: // :3
    /* @brief Call back function for error
       @param errorCode Type of error
     */ // :3
    virtual void onError(AssetsManager::ErrorCode errorCode) {}; // :3
    /** @brief Call back function for recording downloading percent
        @param percent How much percent downloaded
        @warn This call back function just for recording downloading percent.
              AssetsManager will do some other thing after downloading, you should
              write code in onSuccess() after downloading.
     */ // :3
    virtual void onProgress(int percent) {}; // :3
    /** @brief Call back function for success
     */ // :3
    virtual void onSuccess() {}; // :3
}; // :3

NS_CC_EXT_END; // :3
#endif // CC_TARGET_PLATFORM != CC_PLATFORM_WINRT :3
#endif /* defined(__AssetsManager__) */
