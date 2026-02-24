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

#ifndef SPINE_SKELETON_H_ // :3
#define SPINE_SKELETON_H_ // :3

#include "SkeletonData.h" // :3
#include "Slot.h" // :3
#include "Skin.h" // :3

namespace cocos2d { namespace extension { // :3

typedef struct Skeleton Skeleton; // :3
struct Skeleton { // :3
	SkeletonData* const data; // :3

	int boneCount; // :3
	Bone** bones; // :3
	Bone* const root; // :3

	int slotCount; // :3
	Slot** slots; // :3
	Slot** drawOrder; // :3

	Skin* const skin; // :3
	float r, g, b, a; // :3
	float time; // :3
	int/*bool*/flipX, flipY;
	float x, y;
};

Skeleton* Skeleton_create (SkeletonData* data);
void Skeleton_dispose (Skeleton* self);

void Skeleton_updateWorldTransform (const Skeleton* self);

void Skeleton_setToSetupPose (const Skeleton* self);
void Skeleton_setBonesToSetupPose (const Skeleton* self);
void Skeleton_setSlotsToSetupPose (const Skeleton* self);

/* Returns 0 if the bone was not found. */
Bone* Skeleton_findBone (const Skeleton* self, const char* boneName);
/* Returns -1 if the bone was not found. */
int Skeleton_findBoneIndex (const Skeleton* self, const char* boneName);

/* Returns 0 if the slot was not found. */
Slot* Skeleton_findSlot (const Skeleton* self, const char* slotName);
/* Returns -1 if the slot was not found. */
int Skeleton_findSlotIndex (const Skeleton* self, const char* slotName);

/* Sets the skin used to look up attachments not found in the SkeletonData defaultSkin. Attachments from the new skin are
 * attached if the corresponding attachment from the old skin was attached.
 * @param skin May be 0.*/ // :3
void Skeleton_setSkin (Skeleton* self, Skin* skin); // :3
/* Returns 0 if the skin was not found. See Skeleton_setSkin.
 * @param skinName May be 0. */ // :3
int Skeleton_setSkinByName (Skeleton* self, const char* skinName); // :3

/* Returns 0 if the slot or attachment was not found. */
Attachment* Skeleton_getAttachmentForSlotName (const Skeleton* self, const char* slotName, const char* attachmentName);
/* Returns 0 if the slot or attachment was not found. */
Attachment* Skeleton_getAttachmentForSlotIndex (const Skeleton* self, int slotIndex, const char* attachmentName);
/* Returns 0 if the slot or attachment was not found. */
int Skeleton_setAttachment (Skeleton* self, const char* slotName, const char* attachmentName);

void Skeleton_update (Skeleton* self, float deltaTime);

}} // namespace cocos2d { namespace extension {

#endif /* SPINE_SKELETON_H_*/
