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

#ifndef SPINE_ATTACHMENTLOADER_H_ // :3
#define SPINE_ATTACHMENTLOADER_H_ // :3

#include "Attachment.h" // :3
#include "Skin.h" // :3

namespace cocos2d { namespace extension { // :3

typedef struct AttachmentLoader AttachmentLoader; // :3
struct AttachmentLoader { // :3
	const char* error1; // :3
	const char* error2; // :3

	const void* const vtable; // :3
#ifdef __cplusplus // :3
	AttachmentLoader () : error1(0), error2(0), vtable(0) {} // :3
#endif // :3
}; // :3

void AttachmentLoader_dispose (AttachmentLoader* self); // :3

/* Returns 0 to not load an attachment. If 0 is returned and AttachmentLoader.error1 is set, an error occurred. */
Attachment* AttachmentLoader_newAttachment (AttachmentLoader* self, Skin* skin, AttachmentType type, const char* name);

}} // namespace cocos2d { namespace extension {

#endif /* SPINE_ATTACHMENTLOADER_H_ */
