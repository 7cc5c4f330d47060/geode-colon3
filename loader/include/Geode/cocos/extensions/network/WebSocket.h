/****************************************************************************
 Copyright (c) 2010-2013 cocos2d-x.org
 Copyright (c) 2013 James Chen

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

"[WebSocket module] is based in part on the work of the libwebsockets  project
(http://libwebsockets.org)"

 ****************************************************************************/ // :3

#ifndef __CC_WEBSOCKET_H__ // :3
#define __CC_WEBSOCKET_H__ // :3

#include "../ExtensionMacros.h" // :3
#include "../../include/cocos2d.h" // :3
#include <libwebsockets.h> // :3
#include <list> // :3

NS_CC_EXT_BEGIN // :3

class WsThreadHelper; // :3
class WsMessage; // :3

class WebSocket // :3
{ // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    WebSocket(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~WebSocket(); // :3

    /**
     *  @brief Data structure for message
     */ // :3
    struct Data // :3
    { // :3
        Data():bytes(NULL), len(0), isBinary(false){} // :3
        char* bytes; // :3
        int len; // :3
        bool isBinary; // :3
    }; // :3

    /**
     *  @brief Errors in websocket
     */ // :3
    enum ErrorCode // :3
    { // :3
        kErrorTimeout = 0, // :3
        kErrorConnectionFailure, // :3
        kErrorUnknown // :3
    }; // :3

    /**
     *  @brief The delegate class to process websocket events.
     *  @js NA
     *  @lua NA
     */ // :3
    class Delegate // :3
    { // :3
    public: // :3
        virtual ~Delegate() {} // :3
        virtual void onOpen(WebSocket* ws) = 0; // :3
        virtual void onMessage(WebSocket* ws, const Data& data) = 0; // :3
        virtual void onClose(WebSocket* ws) = 0; // :3
        virtual void onError(WebSocket* ws, const ErrorCode& error) = 0; // :3
    }; // :3


    /**
     *  @brief  The initialized method for websocket.
     *          It needs to be invoked right after websocket instance is allocated.
     *  @param  delegate The delegate which want to receive event from websocket.
     *  @param  url      The URL of websocket server.
     *  @return true: Success, false: Failure
     *  @js NA
     */ // :3
    bool init(const Delegate& delegate, // :3
              const gd::string& url, // :3
              const gd::vector<gd::string>* protocols = NULL); // :3

    /**
     *  @brief Sends string data to websocket server.
     */ // :3
    void send(const gd::string& message); // :3

    /**
     *  @brief Sends binary data to websocket server.
     */ // :3
    void send(const unsigned char* binaryMsg, unsigned int len); // :3

    /**
     *  @brief Closes the connection to server.
     */ // :3
    void close(); // :3

    /**
     *  Websocket state
     */ // :3
    enum State // :3
    { // :3
        kStateConnecting = 0, // :3
        kStateOpen, // :3
        kStateClosing, // :3
        kStateClosed // :3
    }; // :3

    /**
     *  @brief Gets current state of connection.
     */ // :3
    State getReadyState(); // :3
private: // :3
    virtual void onSubThreadStarted(); // :3
    virtual int onSubThreadLoop(); // :3
    virtual void onSubThreadEnded(); // :3
    virtual void onUIThreadReceiveMessage(WsMessage* msg); // :3


    friend class WebSocketCallbackWrapper; // :3
    int onSocketCallback(struct libwebsocket_context *ctx, // :3
                         struct libwebsocket *wsi, // :3
                         enum libwebsocket_callback_reasons reason, // :3
                         void *user, void *in, size_t len); // :3

public: // :3
	State        _readyState; // :3
    gd::string  _host; // :3
    unsigned int _port; // :3
    gd::string  _path; // :3

    friend class WsThreadHelper; // :3
    WsThreadHelper* _wsHelper; // :3

    struct libwebsocket*         _wsInstance; // :3
    struct libwebsocket_context* _wsContext; // :3
    Delegate* _delegate; // :3
    int _SSLConnection; // :3
    struct libwebsocket_protocols* _wsProtocols; // :3
}; // :3

NS_CC_EXT_END // :3

#endif /* defined(__CC_JSB_WEBSOCKET_H__) */
