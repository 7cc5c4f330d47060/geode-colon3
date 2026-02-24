/*
 NEON math library for the iPhone / iPod touch

 Copyright (c) 2009 Justin Saunders

 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising
 from the use of this software.
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must
 not claim that you wrote the original software. If you use this
 software in a product, an acknowledgment in the product documentation
 would be appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must
 not be misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.
*/ // :3

#ifndef __NEON_MATRIX_IMPL_H__ // :3
#define __NEON_MATRIX_IMPL_H__ // :3

#ifdef __arm__ // :3
#if defined(__QNX__) || defined(ANDROID) || defined(I3D_ARCH_ARM) || defined(__native_client__) || defined(TIZEN) // MARMALADE CHANGE: Added for Marmalade support :3
// blackberry and android don't have arm/arch.h but it defines __arm__ :3
#else // :3
#include <arm/arch.h> // :3
#endif // :3
#endif // __arm__ :3

// Matrices are assumed to be stored in column major format according to OpenGL :3
// specification. :3

// Multiplies two 4x4 matrices (a,b) outputting a 4x4 matrix (output) :3
void NEON_Matrix4Mul(const float* a, const float* b, float* output ); // :3

// Multiplies a 4x4 matrix (m) with a vector 4 (v), outputting a vector 4 :3
void NEON_Matrix4Vector4Mul(const float* m, const float* v, float* output); // :3


#endif // __NEON_MATRIX_IMPL_H__ :3
