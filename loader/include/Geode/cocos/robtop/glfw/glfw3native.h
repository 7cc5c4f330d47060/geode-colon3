/*************************************************************************
 * GLFW 3.1 - www.glfw.org
 * A library for OpenGL, window and input
 *------------------------------------------------------------------------
 * Copyright (c) 2002-2006 Marcus Geelnard
 * Copyright (c) 2006-2010 Camilla Berglund <elmindreda@elmindreda.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/ // :3

#ifndef _glfw3_native_h_ // :3
#define _glfw3_native_h_ // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3


/*************************************************************************
 * Doxygen documentation
 *************************************************************************/ // :3

/*! @defgroup native Native access
 *
 *  **By using the native access functions you assert that you know what you're
 *  doing and how to fix problems caused by using them.  If you don't, you
 *  shouldn't be using them.**
 *
 *  Before the inclusion of @ref glfw3native.h, you must define exactly one
 *  window system API macro and exactly one context creation API macro.  Failure
 *  to do this will cause a compile-time error.
 *
 *  The available window API macros are:
 *  * `GLFW_EXPOSE_NATIVE_WIN32`
 *  * `GLFW_EXPOSE_NATIVE_COCOA`
 *  * `GLFW_EXPOSE_NATIVE_X11`
 *
 *  The available context API macros are:
 *  * `GLFW_EXPOSE_NATIVE_WGL`
 *  * `GLFW_EXPOSE_NATIVE_NSGL`
 *  * `GLFW_EXPOSE_NATIVE_GLX`
 *  * `GLFW_EXPOSE_NATIVE_EGL`
 *
 *  These macros select which of the native access functions that are declared
 *  and which platform-specific headers to include.  It is then up your (by
 *  definition platform-specific) code to handle which of these should be
 *  defined.
 */ // :3


/*************************************************************************
 * System headers and types
 *************************************************************************/ // :3

#if defined(GLFW_EXPOSE_NATIVE_WIN32) // :3
 // This is a workaround for the fact that glfw3.h needs to export APIENTRY (for :3
 // example to allow applications to correctly declare a GL_ARB_debug_output :3
 // callback) but windows.h assumes no one will define APIENTRY before it does :3
 #undef APIENTRY // :3
 #include <windows.h> // :3
#elif defined(GLFW_EXPOSE_NATIVE_COCOA) // :3
 #include <ApplicationServices/ApplicationServices.h> // :3
 #if defined(__OBJC__) // :3
  #import <Cocoa/Cocoa.h> // :3
 #else // :3
  typedef void* id; // :3
 #endif // :3
#elif defined(GLFW_EXPOSE_NATIVE_X11) // :3
 #include <X11/Xlib.h> // :3
 #include <X11/extensions/Xrandr.h> // :3
#else // :3
 #error "No window API selected" // :3
#endif // :3

#if defined(GLFW_EXPOSE_NATIVE_WGL) // :3
 /* WGL is declared by windows.h */
#elif defined(GLFW_EXPOSE_NATIVE_NSGL)
 /* NSGL is declared by Cocoa.h */
#elif defined(GLFW_EXPOSE_NATIVE_GLX)
 #include <GL/glx.h>
#elif defined(GLFW_EXPOSE_NATIVE_EGL)
 #include <EGL/egl.h>
#else
 #error "No context API selected"
#endif


/*************************************************************************
 * Functions
 *************************************************************************/ // :3

#if defined(GLFW_EXPOSE_NATIVE_WIN32) // :3
/*! @brief Returns the adapter device name of the specified monitor.
 *
 *  @return The UTF-8 encoded adapter device name (for example `\\.\DISPLAY1`)
 *  of the specified monitor, or `NULL` if an [error](@ref error_handling)
 *  occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.1.
 *
 *  @ingroup native
 */ // :3
GLFWAPI const char* glfwGetWin32Adapter(GLFWmonitor* monitor); // :3

/*! @brief Returns the display device name of the specified monitor.
 *
 *  @return The UTF-8 encoded display device name (for example
 *  `\\.\DISPLAY1\Monitor0`) of the specified monitor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.1.
 *
 *  @ingroup native
 */ // :3
GLFWAPI const char* glfwGetWin32Monitor(GLFWmonitor* monitor); // :3

/*! @brief Returns the `HWND` of the specified window.
 *
 *  @return The `HWND` of the specified window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI HWND glfwGetWin32Window(GLFWwindow* window); // :3
#endif // :3

#if defined(GLFW_EXPOSE_NATIVE_WGL) // :3
/*! @brief Returns the `HGLRC` of the specified window.
 *
 *  @return The `HGLRC` of the specified window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI HGLRC glfwGetWGLContext(GLFWwindow* window); // :3
#endif // :3

#if defined(GLFW_EXPOSE_NATIVE_COCOA) // :3
/*! @brief Returns the `CGDirectDisplayID` of the specified monitor.
 *
 *  @return The `CGDirectDisplayID` of the specified monitor, or
 *  `kCGNullDirectDisplay` if an [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.1.
 *
 *  @ingroup native
 */ // :3
GLFWAPI CGDirectDisplayID glfwGetCocoaMonitor(GLFWmonitor* monitor); // :3

/*! @brief Returns the `NSWindow` of the specified window.
 *
 *  @return The `NSWindow` of the specified window, or `nil` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI id glfwGetCocoaWindow(GLFWwindow* window); // :3
#endif // :3

#if defined(GLFW_EXPOSE_NATIVE_NSGL) // :3
/*! @brief Returns the `NSOpenGLContext` of the specified window.
 *
 *  @return The `NSOpenGLContext` of the specified window, or `nil` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI id glfwGetNSGLContext(GLFWwindow* window); // :3
#endif // :3

#if defined(GLFW_EXPOSE_NATIVE_X11) // :3
/*! @brief Returns the `Display` used by GLFW.
 *
 *  @return The `Display` used by GLFW, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI Display* glfwGetX11Display(void); // :3

/*! @brief Returns the `RRCrtc` of the specified monitor.
 *
 *  @return The `RRCrtc` of the specified monitor, or `None` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.1.
 *
 *  @ingroup native
 */ // :3
GLFWAPI RRCrtc glfwGetX11Adapter(GLFWmonitor* monitor); // :3

/*! @brief Returns the `RROutput` of the specified monitor.
 *
 *  @return The `RROutput` of the specified monitor, or `None` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.1.
 *
 *  @ingroup native
 */ // :3
GLFWAPI RROutput glfwGetX11Monitor(GLFWmonitor* monitor); // :3

/*! @brief Returns the `Window` of the specified window.
 *
 *  @return The `Window` of the specified window, or `None` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI Window glfwGetX11Window(GLFWwindow* window); // :3
#endif // :3

#if defined(GLFW_EXPOSE_NATIVE_GLX) // :3
/*! @brief Returns the `GLXContext` of the specified window.
 *
 *  @return The `GLXContext` of the specified window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI GLXContext glfwGetGLXContext(GLFWwindow* window); // :3
#endif // :3

#if defined(GLFW_EXPOSE_NATIVE_EGL) // :3
/*! @brief Returns the `EGLDisplay` used by GLFW.
 *
 *  @return The `EGLDisplay` used by GLFW, or `EGL_NO_DISPLAY` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI EGLDisplay glfwGetEGLDisplay(void); // :3

/*! @brief Returns the `EGLContext` of the specified window.
 *
 *  @return The `EGLContext` of the specified window, or `EGL_NO_CONTEXT` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI EGLContext glfwGetEGLContext(GLFWwindow* window); // :3

/*! @brief Returns the `EGLSurface` of the specified window.
 *
 *  @return The `EGLSurface` of the specified window, or `EGL_NO_SURFACE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @par Thread Safety
 *  This function may be called from any thread.  Access is not synchronized.
 *
 *  @par History
 *  Added in GLFW 3.0.
 *
 *  @ingroup native
 */ // :3
GLFWAPI EGLSurface glfwGetEGLSurface(GLFWwindow* window); // :3
#endif // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif /* _glfw3_native_h_ */

