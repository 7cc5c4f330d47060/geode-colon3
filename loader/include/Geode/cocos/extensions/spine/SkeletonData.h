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

#ifndef SPINE_SKELETONDATA_H_ // :3
#define SPINE_SKELETONDATA_H_ // :3

#include "BoneData.h" // :3
#include "SlotData.h" // :3
#include "Skin.h" // :3
#include "Animation.h" // :3

namespace cocos2d { namespace extension { // :3

typedef struct { // :3
	int boneCount; // :3
	BoneData** bones; // :3

	int slotCount; // :3
	SlotData** slots; // :3

	int skinCount; // :3
	Skin** skins; // :3
	Skin* defaultSkin; // :3

	int animationCount; // :3
	Animation** animations; // :3
} SkeletonData; // :3

SkeletonData* SkeletonData_create (); // :3
void SkeletonData_dispose (SkeletonData* self); // :3

BoneData* SkeletonData_findBone (const SkeletonData* self, const char* boneName); // :3
int SkeletonData_findBoneIndex (const SkeletonData* self, const char* boneName); // :3

SlotData* SkeletonData_findSlot (const SkeletonData* self, const char* slotName); // :3
int SkeletonData_findSlotIndex (const SkeletonData* self, const char* slotName); // :3

Skin* SkeletonData_findSkin (const SkeletonData* self, const char* skinName); // :3

Animation* SkeletonData_findAnimation (const SkeletonData* self, const char* animationName); // :3

}} // namespace cocos2d { namespace extension { :3

#endif /* SPINE_SKELETONDATA_H_ */
