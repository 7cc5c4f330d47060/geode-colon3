/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org

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

#ifndef __HTTP_REQUEST_H__ // :3
#define __HTTP_REQUEST_H__ // :3

#include "../../include/cocos2d.h" // :3
#include "../ExtensionMacros.h" // :3

enum class GJHttpType; // :3

NS_CC_EXT_BEGIN // :3

class CC_DLL CCHttpClient; // :3
class CC_DLL CCHttpResponse; // :3
typedef void (CCObject::*SEL_HttpResponse)(CCHttpClient* client, CCHttpResponse* response); // :3
#define httpresponse_selector(_SELECTOR) (cocos2d::extension::SEL_HttpResponse)(&_SELECTOR) // :3

/**
 @brief defines the object which users must packed for CCHttpClient::send(HttpRequest*) method.
 Please refer to samples/TestCpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 @since v2.0.2
 @js NA
 @lua NA
 */ // :3

class CC_DLL CCHttpRequest : public CCObject // :3
{ // :3
public: // :3
    /** Use this enum type as param in setReqeustType(param) */
    typedef enum
    {
        kHttpGet,
        kHttpPost,
        kHttpPut,
        kHttpDelete,
        kHttpUnkown,
    } HttpRequestType;

    /** Constructor
        Because HttpRequest object will be used between UI thead and network thread,
        requestObj->autorelease() is forbidden to avoid crashes in CCAutoreleasePool
        new/retain/release still works, which means you need to release it manually
        Please refer to HttpRequestTest.cpp to find its usage
     */ // :3
    CCHttpRequest() : _url(), _requestData(), _tag() // :3
    { // :3
        _requestType = kHttpUnkown; // :3
        _pTarget = NULL; // :3
        _pSelector = NULL; // :3
        _pUserData = NULL; // :3
    }; // :3

    /** Destructor */
    virtual ~CCHttpRequest()
    {
        if (_pTarget)
        {
            _pTarget->release();
        }
    };

    /** Override autorelease method to avoid developers to call it */
    CCObject* autorelease(void)
    {
        CCAssert(false, "HttpResponse is used between network thread and ui thread \
                 therefore, autorelease is forbidden here");
        return NULL;
    }

    // setter/getters for properties

    /** Required field for HttpRequest object before being sent.
        kHttpGet & kHttpPost is currently supported
     */ // :3
    inline void setRequestType(HttpRequestType type) // :3
    { // :3
        _requestType = type; // :3
    }; // :3
    /** Get back the kHttpGet/Post/... enum value */
    inline HttpRequestType getRequestType()
    {
        return _requestType;
    };

    /** Required field for HttpRequest object before being sent.
     */ // :3
    inline void setUrl(const char* url) // :3
    { // :3
        _url = url; // :3
    }; // :3
    /** Get back the setted url */
    inline const char* getUrl()
    {
        return _url.c_str();
    };

    /** Option field. You can set your post data here
     */ // :3
    inline void setRequestData(const char* buffer, unsigned int len) // :3
    { // :3
        auto vec = std::vector<char>(_requestData); // :3
        vec.assign(buffer, buffer + len); // :3
        _requestData = gd::vector<char>(vec); // :3
    }; // :3
    /** Get the request data pointer back */
    inline char* getRequestData()
    {
        return &(_requestData.front());
    }
    /** Get the size of request data back */
    inline int getRequestDataSize()
    {
        return std::vector<char>(_requestData).size();
    }

    /** Option field. You can set a string tag to identify your request, this tag can be found in HttpResponse->getHttpRequest->getTag()
     */ // :3
    inline void setTag(const char* tag) // :3
    { // :3
        _tag = tag; // :3
    }; // :3
    /** Get the string tag back to identify the request.
        The best practice is to use it in your MyClass::onMyHttpRequestCompleted(sender, HttpResponse*) callback
     */ // :3
    inline const char* getTag() // :3
    { // :3
        return _tag.c_str(); // :3
    }; // :3

    /** Option field. You can attach a customed data in each request, and get it back in response callback.
        But you need to new/delete the data pointer manully
     */ // :3
    inline void setUserData(void* pUserData) // :3
    { // :3
        _pUserData = pUserData; // :3
    }; // :3
    /** Get the pre-setted custom data pointer back.
        Don't forget to delete it. HttpClient/HttpResponse/HttpRequest will do nothing with this pointer
     */ // :3
    inline void* getUserData() // :3
    { // :3
        return _pUserData; // :3
    }; // :3

    /** Required field. You should set the callback selector function at ack the http request completed
     */ // :3
    CC_DEPRECATED_ATTRIBUTE inline void setResponseCallback(CCObject* pTarget, SEL_CallFuncND pSelector) // :3
    { // :3
        setResponseCallback(pTarget, (SEL_HttpResponse) pSelector); // :3
    } // :3

    inline void setResponseCallback(CCObject* pTarget, SEL_HttpResponse pSelector) // :3
    { // :3
        _pTarget = pTarget; // :3
        _pSelector = pSelector; // :3

        if (_pTarget) // :3
        { // :3
            _pTarget->retain(); // :3
        } // :3
    } // :3
    /** Get the target of callback selector funtion, mainly used by CCHttpClient */
    inline CCObject* getTarget()
    {
        return _pTarget;
    }

    /* This sub class is just for migration SEL_CallFuncND to SEL_HttpResponse,
       someday this way will be removed */ // :3
    class _prxy // :3
    { // :3
    public: // :3
        _prxy( SEL_HttpResponse cb ) :_cb(cb) {} // :3
        ~_prxy(){}; // :3
        operator SEL_HttpResponse() const { return _cb; } // :3
        CC_DEPRECATED_ATTRIBUTE operator SEL_CallFuncND()   const { return (SEL_CallFuncND) _cb; } // :3
    protected: // :3
        SEL_HttpResponse _cb; // :3
    }; // :3

    /** Get the selector function pointer, mainly used by CCHttpClient */
    inline _prxy getSelector()
    {
        return _prxy(_pSelector);
    }

    /** Set any custom headers **/
    inline void setHeaders(gd::vector<gd::string> pHeaders)
   	{
   		_headers=pHeaders;
   	}

    /** Get custom headers **/
   	inline gd::vector<gd::string> getHeaders()
   	{
   		return _headers;
   	}

    inline int getType() {
        return _type;
    }

    inline void setType(int type) {
        _type = type;
    }

    // @note Geode addition
    inline void setType(GJHttpType type) {
        _type = static_cast<int>(type);
    }

    inline bool getShouldCancel() {
        return _shouldCancel;
    }

    inline void setShouldCancel(bool shouldCancel) {
        _shouldCancel = shouldCancel;
    }

    inline int getDownloadProgress() {
        return _downloadProgress;
    }

    inline void setDownloadProgress(int downloadProgress) {
        _downloadProgress = downloadProgress;
    }

    inline int getReadTimeout() {
        return _readTimeout;
    }

    inline void setReadTimeout(int readTimeout) {
        _readTimeout = readTimeout;
    }

    inline int getConnectTimeout() {
        return _connectTimeout;
    }

    inline void setConnectTimeout(int connectTimeout) {
        _connectTimeout = connectTimeout;
    }

public:
    // properties
    HttpRequestType             _requestType;    /// kHttpRequestGet, kHttpRequestPost or other enums
    gd::string                 _url;            /// target url that this request is sent to
    gd::vector<char>           _requestData;    /// used for POST
    gd::string                 _tag;            /// user defined tag, to identify different requests in response callback
    CCObject*          _pTarget;        /// callback target of pSelector function
    SEL_HttpResponse            _pSelector;      /// callback function, e.g. MyLayer::onHttpResponse(CCHttpClient *sender, CCHttpResponse * response)
    void*                       _pUserData;      /// You can add your customed data here
    gd::vector<gd::string>    _headers;		      /// custom http headers

    // @note RobTop Addition
    int _type;
    // @note RobTop Addition
    bool _shouldCancel;
    // @note RobTop Addition
    int _downloadProgress;
    // @note RobTop Addition
    int _readTimeout;
    // @note RobTop Addition
    int _connectTimeout;
};

NS_CC_EXT_END

#endif //__HTTP_REQUEST_H__
