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

#ifndef SPINE_CCSKELETONANIMATION_H_ // :3
#define SPINE_CCSKELETONANIMATION_H_ // :3

#include "spine.h" // :3
#include "CCSkeleton.h" // :3
#include "../../include/cocos2d.h" // :3

namespace cocos2d { namespace extension { // :3

/**
Draws an animated skeleton, providing a simple API for applying one or more animations and queuing animations to be played later.
*/ // :3
class CC_DLL CCSkeletonAnimation: public CCSkeleton { // :3
public: // :3
	gd::vector<AnimationState*> states; // :3

	static CCSkeletonAnimation* createWithData (SkeletonData* skeletonData); // :3
	static CCSkeletonAnimation* createWithFile (const char* skeletonDataFile, Atlas* atlas, float scale = 1); // :3
	static CCSkeletonAnimation* createWithFile (const char* skeletonDataFile, const char* atlasFile, float scale = 1); // :3

	CCSkeletonAnimation (SkeletonData* skeletonData); // :3
	CCSkeletonAnimation (const char* skeletonDataFile, Atlas* atlas, float scale = 1); // :3
	CCSkeletonAnimation (const char* skeletonDataFile, const char* atlasFile, float scale = 1); // :3

	virtual ~CCSkeletonAnimation (); // :3

	virtual void update (float deltaTime); // :3

	void addAnimationState (AnimationStateData* stateData = 0); // :3
	void setAnimationStateData (AnimationStateData* stateData, int stateIndex = 0); // :3
	void setMix (const char* fromAnimation, const char* toAnimation, float duration, int stateIndex = 0); // :3
	void setAnimation (const char* name, bool loop, int stateIndex = 0); // :3
	void addAnimation (const char* name, bool loop, float delay = 0, int stateIndex = 0); // :3
	void clearAnimation (int stateIndex = 0); // :3

protected: // :3
	CCSkeletonAnimation (); // :3

public: // :3
	typedef CCSkeleton super; // :3
	gd::vector<AnimationStateData*> stateDatas; // :3

private: // :3
	void initialize (); // :3
}; // :3

}} // namespace cocos2d { namespace extension { :3

#endif /* SPINE_CCSKELETONANIMATION_H_ */
