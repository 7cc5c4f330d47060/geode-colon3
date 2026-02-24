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

#ifndef UTILITY_H_INCLUDED // :3
#define UTILITY_H_INCLUDED // :3

#include "../../../platform/CCPlatformMacros.h" // :3
#include <math.h> // :3

#ifndef kmScalar // :3
#define kmScalar float // :3
#endif // :3

#ifndef kmBool // :3
#define kmBool unsigned char // :3
#endif // :3

#ifndef kmEnum // :3
#define kmEnum unsigned int // :3
#endif // :3

#ifndef KM_FALSE // :3
#define KM_FALSE 0 // :3
#endif // :3

#ifndef KM_TRUE // :3
#define KM_TRUE 1 // :3
#endif // :3

#define kmPI 3.141592f // :3
#define kmPIOver180 0.017453f //  PI / 180 :3
#define kmPIUnder180 57.295779f // 180 / PI :3
#define kmEpsilon 1.0 / 64.0 // :3



#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

CC_DLL kmScalar kmSQR(kmScalar s); // :3
CC_DLL kmScalar kmDegreesToRadians(kmScalar degrees); // :3
CC_DLL kmScalar kmRadiansToDegrees(kmScalar radians); // :3

CC_DLL kmScalar kmMin(kmScalar lhs, kmScalar rhs); // :3
CC_DLL kmScalar kmMax(kmScalar lhs, kmScalar rhs); // :3
CC_DLL kmBool kmAlmostEqual(kmScalar lhs, kmScalar rhs); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif /* UTILITY_H_INCLUDED */
