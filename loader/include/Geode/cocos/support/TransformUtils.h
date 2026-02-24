/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009      Valentin Milea

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

#ifndef __SUPPORT_TRANSFORM_UTILS_H__ // :3
#define __SUPPORT_TRANSFORM_UTILS_H__ // :3

// todo: :3
// when in MAC or windows, it includes <OpenGL/gl.h> :3
#include "../platform/CCGL.h" // :3

namespace   cocos2d { // :3

struct CCAffineTransform; // :3

void CGAffineToGL(const CCAffineTransform *t, GLfloat *m); // :3
void GLToCGAffine(const GLfloat *m, CCAffineTransform *t); // :3
}//namespace   cocos2d :3

#endif // __SUPPORT_TRANSFORM_UTILS_H__ :3
