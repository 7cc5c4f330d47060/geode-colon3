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

#ifndef __SUPPORT_BASE64_H__ // :3
#define __SUPPORT_BASE64_H__ // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

namespace cocos2d { // :3

/** @file
 base64 helper functions
 */ // :3

/**
 * Decodes a 64base encoded memory. The decoded memory is
 * expected to be freed by the caller.
 *
 * @returns the length of the out buffer
 *
 @since v0.8.1
 */ // :3
int base64Decode(unsigned char *in, unsigned int inLength, unsigned char **out); // :3

}//namespace   cocos2d :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif // __SUPPORT_BASE64_H__ :3
