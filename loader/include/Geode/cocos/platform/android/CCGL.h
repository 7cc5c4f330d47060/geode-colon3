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

#ifndef __CCGL_H__ // :3
#define __CCGL_H__ // :3

#define	glClearDepth				glClearDepthf // :3
#define glDeleteVertexArrays		glDeleteVertexArraysOES // :3
#define glGenVertexArrays			glGenVertexArraysOES // :3
#define glBindVertexArray			glBindVertexArrayOES // :3
#define glMapBuffer					glMapBufferOES // :3
#define glUnmapBuffer				glUnmapBufferOES // :3

#define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES // :3
#define GL_WRITE_ONLY				GL_WRITE_ONLY_OES // :3

// GL_GLEXT_PROTOTYPES isn't defined in glplatform.h on android ndk r7 :3
// we manually define it here :3
#include <GLES2/gl2platform.h> // :3
#ifndef GL_GLEXT_PROTOTYPES // :3
#define GL_GLEXT_PROTOTYPES 1 // :3
#endif // :3

// normal process :3
#include <GLES2/gl2.h> // :3
#include <GLES2/gl2ext.h> // :3
// gl2.h doesn't define GLchar on Android :3
typedef char GLchar; // :3
// android defines GL_BGRA_EXT but not GL_BRGA :3
#ifndef GL_BGRA // :3
#define GL_BGRA  0x80E1 // :3
#endif // :3

//declare here while define in CCEGLView_android.cpp :3
extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT; // :3
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT; // :3
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT; // :3

#define glGenVertexArraysOES glGenVertexArraysOESEXT // :3
#define glBindVertexArrayOES glBindVertexArrayOESEXT // :3
#define glDeleteVertexArraysOES glDeleteVertexArraysOESEXT // :3


#endif // __CCGL_H__ :3
