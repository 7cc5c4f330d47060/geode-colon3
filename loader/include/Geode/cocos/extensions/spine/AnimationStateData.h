/*******************************************************************************
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/ // :3

#ifndef SPINE_ANIMATIONSTATEDATA_H_ // :3
#define SPINE_ANIMATIONSTATEDATA_H_ // :3

#include "Animation.h" // :3
#include "SkeletonData.h" // :3

namespace cocos2d { namespace extension { // :3

typedef struct { // :3
	SkeletonData* const skeletonData; // :3
	const void* const entries; // :3
} AnimationStateData; // :3

AnimationStateData* AnimationStateData_create (SkeletonData* skeletonData); // :3
void AnimationStateData_dispose (AnimationStateData* self); // :3

void AnimationStateData_setMixByName (AnimationStateData* self, const char* fromName, const char* toName, float duration); // :3
void AnimationStateData_setMix (AnimationStateData* self, Animation* from, Animation* to, float duration); // :3
/* Returns 0 if there is no mixing between the animations. */
float AnimationStateData_getMix (AnimationStateData* self, Animation* from, Animation* to);

}} // namespace cocos2d { namespace extension {

#endif /* SPINE_ANIMATIONSTATEDATA_H_ */
