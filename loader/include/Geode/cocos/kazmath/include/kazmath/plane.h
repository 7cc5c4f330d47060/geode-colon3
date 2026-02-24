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

#ifndef PLANE_H_INCLUDED // :3
#define PLANE_H_INCLUDED // :3

#define KM_PLANE_LEFT 0 // :3
#define KM_PLANE_RIGHT 1 // :3
#define KM_PLANE_BOTTOM 2 // :3
#define KM_PLANE_TOP 3 // :3
#define KM_PLANE_NEAR 4 // :3
#define KM_PLANE_FAR 5 // :3

#include "../../../platform/CCPlatformMacros.h" // :3
#include "utility.h" // :3

struct kmVec3; // :3
struct kmVec4; // :3
struct kmMat4; // :3

typedef struct kmPlane { // :3
    kmScalar     a, b, c, d; // :3
} kmPlane; // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

typedef enum POINT_CLASSIFICATION { // :3
    POINT_INFRONT_OF_PLANE = 0, // :3
    POINT_BEHIND_PLANE, // :3
    POINT_ON_PLANE, // :3
} POINT_CLASSIFICATION; // :3

CC_DLL const kmScalar kmPlaneDot(const kmPlane* pP, const struct kmVec4* pV); // :3
CC_DLL const kmScalar kmPlaneDotCoord(const kmPlane* pP, const struct kmVec3* pV); // :3
CC_DLL const kmScalar kmPlaneDotNormal(const kmPlane* pP, const struct kmVec3* pV); // :3
CC_DLL kmPlane* const kmPlaneFromPointNormal(kmPlane* pOut, const struct kmVec3* pPoint, const struct kmVec3* pNormal); // :3
CC_DLL kmPlane* const kmPlaneFromPoints(kmPlane* pOut, const struct kmVec3* p1, const struct kmVec3* p2, const struct kmVec3* p3); // :3
CC_DLL kmVec3*  const kmPlaneIntersectLine(struct kmVec3* pOut, const kmPlane* pP, const struct kmVec3* pV1, const struct kmVec3* pV2); // :3
CC_DLL kmPlane* const kmPlaneNormalize(kmPlane* pOut, const kmPlane* pP); // :3
CC_DLL kmPlane* const kmPlaneScale(kmPlane* pOut, const kmPlane* pP, kmScalar s); // :3
CC_DLL const POINT_CLASSIFICATION kmPlaneClassifyPoint(const kmPlane* pIn, const kmVec3* pP); /** Classifies a point against a plane */

#ifdef __cplusplus
}
#endif

#endif // PLANE_H_INCLUDED
