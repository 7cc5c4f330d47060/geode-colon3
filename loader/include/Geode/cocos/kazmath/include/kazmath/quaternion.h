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

#ifndef QUATERNION_H_INCLUDED // :3
#define QUATERNION_H_INCLUDED // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#include "../../../platform/CCPlatformMacros.h" // :3
#include "utility.h" // :3

struct kmMat4; // :3
struct kmMat3; // :3
struct kmVec3; // :3

typedef struct kmQuaternion { // :3
    kmScalar x; // :3
    kmScalar y; // :3
    kmScalar z; // :3
    kmScalar w; // :3
} kmQuaternion; // :3

CC_DLL kmQuaternion* const kmQuaternionConjugate(kmQuaternion* pOut, const kmQuaternion* pIn); ///< Returns pOut, sets pOut to the conjugate of pIn :3

CC_DLL const kmScalar     kmQuaternionDot(const kmQuaternion* q1, const kmQuaternion* q2); ///< Returns the dot product of the 2 quaternions :3

CC_DLL kmQuaternion* kmQuaternionExp(kmQuaternion* pOut, const kmQuaternion* pIn); ///< Returns the exponential of the quaternion :3

///< Makes the passed quaternion an identity quaternion :3

CC_DLL kmQuaternion* kmQuaternionIdentity(kmQuaternion* pOut); // :3

///< Returns the inverse of the passed Quaternion :3

CC_DLL kmQuaternion* kmQuaternionInverse(kmQuaternion* pOut, // :3
                                            const kmQuaternion* pIn); // :3

///< Returns true if the quaternion is an identity quaternion :3

CC_DLL int kmQuaternionIsIdentity(const kmQuaternion* pIn); // :3

///< Returns the length of the quaternion :3

CC_DLL kmScalar kmQuaternionLength(const kmQuaternion* pIn); // :3

///< Returns the length of the quaternion squared (prevents a sqrt) :3

CC_DLL kmScalar kmQuaternionLengthSq(const kmQuaternion* pIn); // :3

///< Returns the natural logarithm :3

CC_DLL kmQuaternion* kmQuaternionLn(kmQuaternion* pOut, const kmQuaternion* pIn); // :3

///< Multiplies 2 quaternions together :3

CC_DLL kmQuaternion* kmQuaternionMultiply(kmQuaternion* pOut, const kmQuaternion* q1, const kmQuaternion* q2); // :3

///< Normalizes a quaternion :3

CC_DLL kmQuaternion* kmQuaternionNormalize(kmQuaternion* pOut, const kmQuaternion* pIn); // :3

///< Rotates a quaternion around an axis :3

CC_DLL kmQuaternion* kmQuaternionRotationAxis(kmQuaternion* pOut, const struct kmVec3* pV, kmScalar angle); // :3

///< Creates a quaternion from a rotation matrix :3

CC_DLL kmQuaternion* kmQuaternionRotationMatrix(kmQuaternion* pOut, const struct kmMat3* pIn); // :3

///< Create a quaternion from yaw, pitch and roll :3

CC_DLL kmQuaternion* kmQuaternionRotationYawPitchRoll(kmQuaternion* pOut, kmScalar yaw, kmScalar pitch, kmScalar roll); // :3
///< Interpolate between 2 quaternions :3
CC_DLL kmQuaternion* kmQuaternionSlerp(kmQuaternion* pOut, const kmQuaternion* q1, const kmQuaternion* q2, kmScalar t); // :3

///< Get the axis and angle of rotation from a quaternion :3
CC_DLL void kmQuaternionToAxisAngle(const kmQuaternion* pIn, struct kmVec3* pVector, kmScalar* pAngle); // :3

///< Scale a quaternion :3
CC_DLL kmQuaternion* kmQuaternionScale(kmQuaternion* pOut, const kmQuaternion* pIn, kmScalar s); // :3
CC_DLL kmQuaternion* kmQuaternionAssign(kmQuaternion* pOut, const kmQuaternion* pIn); // :3
CC_DLL kmQuaternion* kmQuaternionAdd(kmQuaternion* pOut, const kmQuaternion* pQ1, const kmQuaternion* pQ2); // :3
CC_DLL kmQuaternion* kmQuaternionRotationBetweenVec3(kmQuaternion* pOut, const struct kmVec3* vec1, const struct kmVec3* vec2, const struct kmVec3* fallback); // :3
CC_DLL struct kmVec3* kmQuaternionMultiplyVec3(struct kmVec3* pOut, const kmQuaternion* q, const struct kmVec3* v); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif // :3
