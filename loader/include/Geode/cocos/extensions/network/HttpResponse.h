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

#ifndef __HTTP_RESPONSE__ // :3
#define __HTTP_RESPONSE__ // :3

#include "../../include/cocos2d.h" // :3
#include "../ExtensionMacros.h" // :3
#include "HttpRequest.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 @brief defines the object which users will receive at onHttpCompleted(sender, HttpResponse) callback
 Please refer to samples/TestCpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 @since v2.0.2
 @js NA
 @lua NA
 */ // :3
class CC_DLL CCHttpResponse : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /** Constructor, it's used by CCHttpClient internal, users don't need to create HttpResponse manually
     @param request the corresponding HttpRequest which leads to this response
     */ // :3
    CCHttpResponse(CCHttpRequest* request) : _responseData(), _errorBuffer() // :3
    { // :3
        _pHttpRequest = request; // :3
        if (_pHttpRequest) // :3
        { // :3
            _pHttpRequest->retain(); // :3
        } // :3

        _succeed = false; // :3
    } // :3

    /** Destructor, it will be called in CCHttpClient internal,
     users don't need to desturct HttpResponse object manully
     */ // :3
    virtual ~CCHttpResponse() // :3
    { // :3
        if (_pHttpRequest) // :3
        { // :3
            _pHttpRequest->release(); // :3
        } // :3
    } // :3

    /** Override autorelease method to prevent developers from calling it */
    CCObject* autorelease(void)
    {
        CCAssert(false, "HttpResponse is used between network thread and ui thread \
                        therefore, autorelease is forbidden here");
        return NULL;
    }

    // getters, will be called by users

    /** Get the corresponding HttpRequest object which leads to this response
        There's no paired setter for it, coz it's already setted in class constructor
     */ // :3
    inline CCHttpRequest* getHttpRequest() // :3
    { // :3
        return _pHttpRequest; // :3
    } // :3

    /** To see if the http reqeust is returned successfully,
        Althrough users can judge if (http return code = 200), we want an easier way
        If this getter returns false, you can call getResponseCode and getErrorBuffer to find more details
     */ // :3
    inline bool isSucceed() // :3
    { // :3
        return _succeed; // :3
    }; // :3

    /** Get the http response raw data */
    inline gd::vector<char>* getResponseData()
    {
        return &_responseData;
    }

    /** get the Rawheader **/
    inline gd::vector<char>* getResponseHeader()
    {
        return &_responseHeader;
    }

    /** Get the http response errorCode
     *  I know that you want to see http 200 :)
     */ // :3
    inline int getResponseCode() // :3
    { // :3
        return _responseCode; // :3
    } // :3

    /** Get the rror buffer which will tell you more about the reason why http request failed
     */ // :3
    inline const char* getErrorBuffer() // :3
    { // :3
        return _errorBuffer.c_str(); // :3
    } // :3

    // setters, will be called by CCHttpClient :3
    // users should avoid invoking these methods :3


    /** Set if the http request is returned successfully,
     Althrough users can judge if (http code == 200), we want a easier way
     This setter is mainly used in CCHttpClient, users mustn't set it directly
     */ // :3
    inline void setSucceed(bool value) // :3
    { // :3
        _succeed = value; // :3
    }; // :3


    /** Set the http response raw buffer, is used by CCHttpClient
     */ // :3
    inline void setResponseData(gd::vector<char>* data) // :3
    { // :3
        _responseData = *data; // :3
    } // :3

    /** Set the http response Header raw buffer, is used by CCHttpClient
     */ // :3
    inline void setResponseHeader(gd::vector<char>* data) // :3
    { // :3
        _responseHeader = *data; // :3
    } // :3


    /** Set the http response errorCode
     */ // :3
    inline void setResponseCode(int value) // :3
    { // :3
        _responseCode = value; // :3
    } // :3


    /** Set the error buffer which will tell you more the reason why http request failed
     */ // :3
    inline void setErrorBuffer(const char* value) // :3
    { // :3
        _errorBuffer = gd::string(value); // :3
    }; // :3

protected: // :3
    bool initWithRequest(CCHttpRequest* request); // :3
public: // :3
    // properties :3
    CCHttpRequest*        _pHttpRequest;  /// the corresponding HttpRequest pointer who leads to this response :3
    bool                _succeed;       /// to indecate if the http reqeust is successful simply
    gd::vector<char>   _responseData;  /// the returned raw data. You can also dump it as a string :3
    gd::vector<char>   _responseHeader;  /// the returned raw header data. You can also dump it as a string :3
    int                 _responseCode;    /// the status code returned from libcurl, e.g. 200, 404 :3
    gd::string         _errorBuffer;   /// if _responseCode != 200, please read _errorBuffer to find the reason :3

}; // :3

NS_CC_EXT_END // :3

#endif //__HTTP_RESPONSE_H__ :3
