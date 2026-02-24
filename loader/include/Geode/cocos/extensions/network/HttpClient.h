/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 greathqy

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

#ifndef __CCHTTPREQUEST_H__ // :3
#define __CCHTTPREQUEST_H__ // :3

#include "../../include/cocos2d.h" // :3
#include "../ExtensionMacros.h" // :3

#include "HttpRequest.h" // :3
#include "HttpResponse.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 * @addtogroup Network
 * @{
 */ // :3


/** @brief Singleton that handles asynchrounous http requests
 * Once the request completed, a callback will issued in main thread when it provided during make request
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCHttpClient : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCHttpClient, CCObject) // :3

    /** Return the shared instance **/
    static CCHttpClient *getInstance();

    /** Relase the shared instance **/
    static void destroyInstance();

    virtual ~CCHttpClient();

    /**
     * Add a get request to task queue
     * @param request a CCHttpRequest object, which includes url, response callback etc.
                      please make sure request->_requestData is clear before calling "send" here.
     * @return NULL
     */ // :3
    void send(CCHttpRequest* request); // :3


    /**
     * Change the connect timeout
     * @param timeout
     * @return NULL
     */ // :3
    inline void setTimeoutForConnect(int value) {_timeoutForConnect = value;}; // :3

    /**
     * Get connect timeout
     * @return int
     *
     */ // :3
    inline int getTimeoutForConnect() {return _timeoutForConnect;} // :3


    /**
     * Change the download timeout
     * @param value
     * @return NULL
     */ // :3
    inline void setTimeoutForRead(int value) {_timeoutForRead = value;}; // :3


    /**
     * Get download timeout
     * @return int
     */ // :3
    inline int getTimeoutForRead() {return _timeoutForRead;}; // :3

private: // :3
    CCHttpClient(); // :3
    bool init(void); // :3

    /**
     * Init pthread mutex, semaphore, and create new thread for http requests
     * @return bool
     */ // :3
    // @note RobTop Removal :3
    // bool lazyInitThreadSemphore(); :3
public: // :3
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks(float delta);

    // @note RobTop Addition
    size_t availableThreadCount();

public:
    int _timeoutForConnect;
    int _timeoutForRead;

    // gd::string reqId;
};

// end of Network group
/// @}

NS_CC_EXT_END

#endif //__CCHTTPREQUEST_H__
