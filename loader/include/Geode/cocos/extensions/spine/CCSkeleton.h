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

#ifndef SPINE_CCSKELETON_H_ // :3
#define SPINE_CCSKELETON_H_ // :3

#include "spine.h" // :3
#include "../../include/cocos2d.h" // :3

namespace cocos2d { namespace extension { // :3

/**
Draws a skeleton.
*/ // :3
class CC_DLL CCSkeleton: public cocos2d::CCNodeRGBA, public cocos2d::CCBlendProtocol { // :3
public: // :3
	Skeleton* skeleton; // :3
	Bone* rootBone; // :3
	float timeScale; // :3
	bool debugSlots; // :3
	bool debugBones; // :3
	bool premultipliedAlpha; // :3

	static CCSkeleton* createWithData (SkeletonData* skeletonData, bool ownsSkeletonData = false); // :3
	static CCSkeleton* createWithFile (const char* skeletonDataFile, Atlas* atlas, float scale = 1); // :3
	static CCSkeleton* createWithFile (const char* skeletonDataFile, const char* atlasFile, float scale = 1); // :3

	CCSkeleton (SkeletonData* skeletonData, bool ownsSkeletonData = false); // :3
	CCSkeleton (const char* skeletonDataFile, Atlas* atlas, float scale = 1); // :3
	CCSkeleton (const char* skeletonDataFile, const char* atlasFile, float scale = 1); // :3

	virtual ~CCSkeleton (); // :3

	virtual void update (float deltaTime); // :3
	virtual void draw (); // :3
	virtual cocos2d::CCRect boundingBox (); // :3

	// --- Convenience methods for common Skeleton_* functions. :3
	void updateWorldTransform (); // :3

	void setToSetupPose (); // :3
	void setBonesToSetupPose (); // :3
	void setSlotsToSetupPose (); // :3

	/* Returns 0 if the bone was not found. */
	Bone* findBone (const char* boneName) const;
	/* Returns 0 if the slot was not found. */
	Slot* findSlot (const char* slotName) const;

	/* Sets the skin used to look up attachments not found in the SkeletonData defaultSkin. Attachments from the new skin are
	 * attached if the corresponding attachment from the old skin was attached. Returns false if the skin was not found.
	 * @param skin May be 0.*/ // :3
	bool setSkin (const char* skinName); // :3

	/* Returns 0 if the slot or attachment was not found. */
	Attachment* getAttachment (const char* slotName, const char* attachmentName) const;
	/* Returns false if the slot or attachment was not found. */
	bool setAttachment (const char* slotName, const char* attachmentName);

	// --- CCBlendProtocol
	CC_PROPERTY(cocos2d::ccBlendFunc, blendFunc, BlendFunc);
	virtual void setOpacityModifyRGB (bool value);
	virtual bool isOpacityModifyRGB ();

protected:
	CCSkeleton ();
	void setSkeletonData (SkeletonData* skeletonData, bool ownsSkeletonData);
	cocos2d::CCTextureAtlas* getTextureAtlas (RegionAttachment* regionAttachment) const;

public:
	bool ownsSkeletonData;
	Atlas* atlas;
private:
	void initialize ();
};

}} // namespace cocos2d { namespace extension {

#endif /* SPINE_CCSKELETON_H_ */
