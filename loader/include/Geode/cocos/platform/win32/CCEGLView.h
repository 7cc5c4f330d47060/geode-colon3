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

#ifndef __CC_EGLVIEW_WIN32_H__ // :3
#define __CC_EGLVIEW_WIN32_H__ // :3

#include "CCStdC.h" // :3
#include "../CCCommon.h" // :3
#include "../../cocoa/CCGeometry.h" // :3
#include "../CCEGLViewProtocol.h" // :3

#include "../../robtop/glfw/glfw3.h" // :3

NS_CC_BEGIN // :3

typedef LRESULT (*CUSTOM_WND_PROC)(UINT message, WPARAM wParam, LPARAM lParam, BOOL* pProcessed); // :3

class CCEGL; // :3

// @note RobTop Addition: added CCObject inheritance :3
class CC_DLL CCEGLView : public CCEGLViewProtocol, public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
protected: // :3
    virtual ~CCEGLView(); // :3
public: // :3
    CCEGLView(); // :3

    CCEGLView(geode::ZeroConstructorType, size_t fill) : // :3
        CCEGLViewProtocol(geode::ZeroConstructor, fill), // :3
        CCObject(geode::ZeroConstructor, fill - sizeof(CCEGLViewProtocol)) {} // :3

    CCEGLView(geode::ZeroConstructorType) : // :3
        CCEGLViewProtocol(geode::ZeroConstructor, sizeof(CCEGLView)), // :3
        CCObject(geode::ZeroConstructor, sizeof(CCEGLView) - sizeof(CCEGLViewProtocol)) {} // :3

    CCEGLView(geode::CutoffConstructorType, size_t fill) : CCEGLView() {} // :3

    /* override functions */
    virtual bool isOpenGLReady();
    virtual void end();
    virtual void swapBuffers();
    virtual void setFrameSize(float width, float height);
	// Robtop Removal
    // virtual void setEditorFrameSize(float width, float height,HWND hWnd);
    virtual void setIMEKeyboardState(bool bOpen);
    void updateWindow(int width, int height);
    void pollEvents(void);

    void setMenuResource(LPCWSTR menu);
    void setWndProc(CUSTOM_WND_PROC proc);

protected:
    // Robtop Removal
    // virtual bool Create();
    void setupWindow(cocos2d::CCRect rect);
    // @note RobTop Addition
    bool initGlew();

public:
    // Robtop Removal
    // bool initGL();
    // Robtop Removal
    // void destroyGL();

    // Robtop Removal
    // virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void setHWnd(HWND hWnd);
    // win32 platform function
    // Robtop Removal
    // HWND getHWnd();
    // Robtop Removal
    // virtual void resize(int width, int height);
    // @note RobTop Addition
     void resizeWindow(int width, int height);

    /*
     * Set zoom factor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
     */ // :3
    void setFrameZoomFactor(float fZoomFactor); // :3
	float getFrameZoomFactor(); // :3
    // @note RobTop Addition: made non virtual :3
    void centerWindow(); // :3
    // @note RobTop Addition :3
    bool windowShouldClose(); // :3
    // @note RobTop Addition :3
    void showCursor(bool state); // :3

    typedef void (*LPFN_ACCELEROMETER_KEYHOOK)( UINT message,WPARAM wParam, LPARAM lParam ); // :3
    void setAccelerometerKeyHook( LPFN_ACCELEROMETER_KEYHOOK lpfnAccelerometerKeyHook ); // :3

    virtual void setViewPortInPoints(float x , float y , float w , float h); // :3
    virtual void setScissorInPoints(float x , float y , float w , float h); // :3

    // static function :3
    /**
    @brief    get the shared main open gl window
    */ // :3
    static CCEGLView* sharedOpenGLView(); // :3

    /**
     * @note Geode addition
     */ // :3
    static GEODE_DLL CCEGLView* get(); // :3

    /**
     * @note RobTop addition
     */ // :3
    static CCEGLView* create(const gd::string&); // :3

    static cocos2d::CCEGLView* createWithFullScreen(gd::string const&, bool); // :3
	static cocos2d::CCEGLView* createWithFullScreen(gd::string const&, bool, GLFWvidmode const&, GLFWmonitor*); // :3
	static cocos2d::CCEGLView* createWithRect(gd::string const&, cocos2d::CCRect, float); // :3

    /**
     * @note Geode addition
     */ // :3
    inline CCPoint getMousePosition() { return { m_fMouseX, m_fMouseY }; } // :3

    /**
     * @note RobTop addition
     */ // :3
    void toggleFullScreen(bool fullscreen, bool borderless, bool fix); // :3

    /**
     * @note RobTop addition
     */ // :3
    GLFWwindow* getWindow(void) const; // :3

    /**
     * @note RobTop addition
     */ // :3
    CCSize getDisplaySize(); // :3

	void capture(); // :3
	void checkErrorGL(char const*); // :3

	void enableRetina(bool); // :3

	bool getCursorLocked() const; // :3
	bool getGameplayActive() const; // :3
	bool getIsBorderless() const; // :3
	bool getIsFullscreen() const; // :3
	int getRetinaFactor() const; // :3
	bool getShouldHideCursor() const; // :3
	void iconify(); // :3

    bool initWithFullScreen(gd::string const&, bool); // :3
	bool initWithFullscreen(gd::string const&, bool, GLFWvidmode const&, GLFWmonitor*); // :3
	bool initWithRect(gd::string const&, cocos2d::CCRect, float); // :3

	bool isRetinaEnabled() const; // :3

	void onGLFWWindowCloseFunCallback(GLFWwindow*); // :3
	void releaseCapture(); // :3
	void showMessage(gd::string); // :3

	void toggleGameplayActive(bool); // :3
	void toggleLockCursor(bool); // :3
	void updateDesignSize(int, int); // :3
	void updateFrameSize(); // :3

    // @note RobTop Addition :3
    void pumpRawInput(); // :3
    // @note RobTop Addition :3
    void performSafeClipboardPaste(); // :3

public: // :3
    static CCEGLView* s_pEglView; // :3

    bool m_bCaptured; // :3
    // Robtop Removal :3
    // HWND m_hWnd; :3
    // Robtop Removal :3
    // HDC  m_hDC; :3
    // Robtop Removal :3
    // HGLRC m_hRC; :3
    // Robtop Removal :3
    // LPFN_ACCELEROMETER_KEYHOOK m_lpfnAccelerometerKeyHook; :3
    bool m_bSupportTouch; // :3
    // @note RobTop Addition :3
    bool m_bInRetinaMonitor; // :3
    // @note RobTop Addition :3
    bool m_bRetinaEnabled; // :3
    // @note RobTop Addition :3
    int m_nRetinaFactor; // :3
    // @note RobTop Addition :3
    bool m_bCursorHidden; // :3
    // @note may be before m_bCursorHidden :3
    int m_unkSize4; // :3
    // Robtop Removal :3
    // LPCWSTR m_menu; :3
    // Robtop Removal :3
    // CUSTOM_WND_PROC m_wndproc; :3
    float m_fFrameZoomFactor; // :3
    // @note RobTop Addition :3
    GLFWwindow* m_pMainWindow; // :3
    // @note RobTop Addition :3
    GLFWmonitor* m_pPrimaryMonitor; // :3
public: // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(CCSize, m_obWindowedSize, WindowedSize); // :3

    // @note RobTop Addition :3
    float m_fMouseX; // :3
    // @note RobTop Addition :3
    float m_fMouseY; // :3
    // @note RobTop Addition :3
    bool m_bIsFullscreen; // :3
    // @note RobTop Addition :3
    bool m_bIsBorderless; // :3
    // @note RobTop Addition :3
    bool m_bIsFix; // :3
    // @note RobTop Addition :3
    bool m_bShouldHideCursor; // :3
    // @note RobTop Addition :3
    bool m_bCursorLocked; // :3
    // @note RobTop Addition :3
    bool m_bShouldCallGLFinish; // :3

protected: // :3
    // @note RobTop Addition :3
    void onGLFWCharCallback(GLFWwindow* window, unsigned int entered); // :3
    // @note RobTop Addition :3
    void onGLFWCursorEnterFunCallback(GLFWwindow* window, int entered); // :3
    // @note RobTop Addition :3
    void onGLFWDeviceChangeFunCallback(GLFWwindow* window); // :3
    // @note RobTop Addition :3
    void onGLFWError(int code, const char* description); // :3
    // @note RobTop Addition :3
    void onGLFWframebuffersize(GLFWwindow* window, int width, int height); // :3
    // @note RobTop Addition :3
    void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y); // :3
    // @note RobTop Addition :3
    void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods); // :3
    // @note RobTop Addition :3
    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods); // :3
    // @note RobTop Addition :3
    void onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset); // :3
    // @note RobTop Addition :3
    void onGLFWWindowIconifyFunCallback(GLFWwindow* window, int iconified); // :3
    // @note RobTop Addition :3
    void onGLFWWindowPosCallback(GLFWwindow* window, int x, int y); // :3
    // @note RobTop Addition :3
    void onGLFWWindowSizeFunCallback(GLFWwindow* window, int width, int height); // :3
}; // :3

NS_CC_END // :3

#endif    // end of __CC_EGLVIEW_WIN32_H__ :3
