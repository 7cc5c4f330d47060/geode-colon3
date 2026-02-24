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

#ifndef C_STACK_H_INCLUDED // :3
#define C_STACK_H_INCLUDED // :3

#include "../mat4.h" // :3

typedef struct km_mat4_stack { // :3
    int capacity; //The total item capacity :3
    int item_count; //The number of items :3
    kmMat4* top; // :3
    kmMat4* stack; // :3
} km_mat4_stack; // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

void km_mat4_stack_initialize(km_mat4_stack* stack); // :3
void km_mat4_stack_push(km_mat4_stack* stack, const kmMat4* item); // :3
void km_mat4_stack_pop(km_mat4_stack* stack, kmMat4* pOut); // :3
void km_mat4_stack_release(km_mat4_stack* stack); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif // C_STACK_H_INCLUDED :3
