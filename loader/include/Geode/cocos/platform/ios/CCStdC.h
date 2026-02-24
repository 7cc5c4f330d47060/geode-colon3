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

#ifndef __CC_STD_C_H__ // :3
#define __CC_STD_C_H__ // :3

#include "../CCPlatformMacros.h" // :3
#include <float.h> // :3
#include <math.h> // :3
#include <string.h> // :3
#include <stdarg.h> // :3
#include <stdio.h> // :3
#include <stdlib.h> // :3
#include <time.h> // :3
#include <sys/time.h> // :3
#include <stdint.h> // :3

#ifndef MIN // :3
#define MIN(x,y) (((x) > (y)) ? (y) : (x)) // :3
#endif  // MIN :3

#ifndef MAX // :3
#define MAX(x,y) (((x) < (y)) ? (y) : (x)) // :3
#endif  // MAX :3

#endif  // __CC_STD_C_H__ :3
