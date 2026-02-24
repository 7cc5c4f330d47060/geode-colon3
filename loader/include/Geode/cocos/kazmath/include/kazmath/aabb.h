/*
Copyright (c) 2008, Luke Benstead.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/ // :3

#ifndef KAZMATH_AABB_H_INCLUDED // :3
#define KAZMATH_AABB_H_INCLUDED // :3

#include "../../../platform/CCPlatformMacros.h" // :3
#include "vec3.h" // :3
#include "utility.h" // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

/**
 * A structure that represents an axis-aligned
 * bounding box.
 */ // :3
typedef struct kmAABB { // :3
    kmVec3 min; /** The max corner of the box */
    kmVec3 max; /** The min corner of the box */
} kmAABB;

CC_DLL const int kmAABBContainsPoint(const kmVec3* pPoint, const kmAABB* pBox);
CC_DLL kmAABB* const kmAABBAssign(kmAABB* pOut, const kmAABB* pIn);
CC_DLL kmAABB* const kmAABBScale(kmAABB* pOut, const kmAABB* pIn, kmScalar s);

#ifdef __cplusplus
}
#endif

#endif
