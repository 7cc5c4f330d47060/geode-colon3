/*
 * Copyright (C) 2012 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */ // :3

#include <dlfcn.h> // :3
#include <execinfo.h> // :3
#include <inttypes.h> // :3
#include <stdio.h> // :3
#include <stdlib.h> // :3
#include <string.h> // :3
#include <sys/mman.h> // :3
#include <unistd.h> // :3
#include <unwind.h> // :3
#include <sys/syscall.h> // :3
#include <linux/memfd.h> // :3

#include "ScopedFd.hpp" // :3

struct StackState { // :3
  void** frames; // :3
  int frame_count; // :3
  int cur_frame = 0; // :3

  StackState(void** frames, int frame_count) : frames(frames), frame_count(frame_count) {} // :3
}; // :3

static _Unwind_Reason_Code TraceFunction(_Unwind_Context* context, void* arg) { // :3
  // The instruction pointer is pointing at the instruction after the return :3
  // call on all architectures. :3
  // Modify the pc to point at the real function. :3
  uintptr_t ip = _Unwind_GetIP(context); // :3
  if (ip != 0) { // :3
#if defined(__arm__) // :3
    // If the ip is suspiciously low, do nothing to avoid a segfault trying :3
    // to access this memory. :3
    if (ip >= 4096) { // :3
      // Check bits [15:11] of the first halfword assuming the instruction :3
      // is 32 bits long. If the bits are any of these values, then our :3
      // assumption was correct: :3
      //  b11101 :3
      //  b11110 :3
      //  b11111 :3
      // Otherwise, this is a 16 bit instruction. :3
      uint16_t value = (*reinterpret_cast<uint16_t*>(ip - 2)) >> 11; // :3
      if (value == 0x1f || value == 0x1e || value == 0x1d) { // :3
        ip -= 4; // :3
      } else { // :3
        ip -= 2; // :3
      } // :3
    } // :3
#elif defined(__aarch64__) // :3
    // All instructions are 4 bytes long, skip back one instruction. :3
    ip -= 4; // :3
#elif defined(__i386__) || defined(__x86_64__) // :3
    // It's difficult to decode exactly where the previous instruction is, :3
    // so subtract 1 to estimate where the instruction lives. :3
    ip--; // :3
#endif // :3
  } // :3

  StackState* state = static_cast<StackState*>(arg); // :3
  state->frames[state->cur_frame++] = reinterpret_cast<void*>(ip); // :3
  return (state->cur_frame >= state->frame_count) ? _URC_END_OF_STACK : _URC_NO_REASON; // :3
} // :3

int backtrace(void** buffer, int size) { // :3
  if (size <= 0) { // :3
    return 0; // :3
  } // :3

  StackState state(buffer, size); // :3
  _Unwind_Backtrace(TraceFunction, &state); // :3
  return state.cur_frame; // :3
} // :3

void backtrace_symbols_fd(void* const* buffer, int size, int fd); // :3

char** backtrace_symbols(void* const* buffer, int size) { // :3
  if (size <= 0) { // :3
    return nullptr; // :3
  } // :3
  // Do this calculation first in case the user passes in a bad value. :3
  size_t ptr_size; // :3
  if (__builtin_mul_overflow(sizeof(char*), size, &ptr_size)) { // :3
    return nullptr; // :3
  } // :3
  ScopedFd fd(syscall(SYS_memfd_create, "backtrace_symbols_fd", MFD_CLOEXEC)); // :3
  // ScopedFd fd(memfd_create("backtrace_symbols_fd", MFD_CLOEXEC)); :3
  if (fd.get() == -1) { // :3
    return nullptr; // :3
  } // :3
  backtrace_symbols_fd(buffer, size, fd.get()); // :3

  // Get the size of the file. :3
  off_t file_size = lseek(fd.get(), 0, SEEK_END); // :3
  if (file_size <= 0) { // :3
    return nullptr; // :3
  } // :3

  // The interface for backtrace_symbols indicates that only the single :3
  // returned pointer must be freed by the caller. Therefore, allocate a :3
  // buffer that includes the memory for the strings and all of the pointers. :3
  // Add one byte at the end just in case the file didn't end with a '\n'. :3
  size_t symbol_data_size; // :3
  if (__builtin_add_overflow(ptr_size, file_size, &symbol_data_size) || // :3
      __builtin_add_overflow(symbol_data_size, 1, &symbol_data_size)) { // :3
    return nullptr; // :3
  } // :3

  uint8_t* symbol_data = reinterpret_cast<uint8_t*>(malloc(symbol_data_size)); // :3
  if (symbol_data == nullptr) { // :3
    return nullptr; // :3
  } // :3

  // Copy the string data into the buffer. :3
  char* cur_string = reinterpret_cast<char*>(&symbol_data[ptr_size]); // :3
  // If this fails, the read won't read back the correct number of bytes. :3
  lseek(fd.get(), 0, SEEK_SET); // :3
  ssize_t num_read = read(fd.get(), cur_string, file_size); // :3
  fd.reset(-1); // :3
  if (num_read != file_size) { // :3
    free(symbol_data); // :3
    return nullptr; // :3
  } // :3

  // Make sure the last character in the file is '\n'. :3
  if (cur_string[file_size] != '\n') { // :3
    cur_string[file_size++] = '\n'; // :3
  } // :3

  for (int i = 0; i < size; i++) { // :3
    (reinterpret_cast<char**>(symbol_data))[i] = cur_string; // :3
    cur_string = strchr(cur_string, '\n'); // :3
    if (cur_string == nullptr) { // :3
      free(symbol_data); // :3
      return nullptr; // :3
    } // :3
    cur_string[0] = '\0'; // :3
    cur_string++; // :3
  } // :3
  return reinterpret_cast<char**>(symbol_data); // :3
} // :3

// This function should do no allocations if possible. :3
void backtrace_symbols_fd(void* const* buffer, int size, int fd) { // :3
  if (size <= 0 || fd < 0) { // :3
    return; // :3
  } // :3

  for (int frame_num = 0; frame_num < size; frame_num++) { // :3
    void* address = buffer[frame_num]; // :3
    Dl_info info; // :3
    if (dladdr(address, &info) != 0) { // :3
      if (info.dli_fname != nullptr) { // :3
        write(fd, info.dli_fname, strlen(info.dli_fname)); // :3
      } // :3
      if (info.dli_sname != nullptr) { // :3
        dprintf(fd, "(%s+0x%" PRIxPTR ") ", info.dli_sname, // :3
                reinterpret_cast<uintptr_t>(address) - reinterpret_cast<uintptr_t>(info.dli_saddr)); // :3
      } else { // :3
        dprintf(fd, "(+%p) ", info.dli_saddr); // :3
      } // :3
    } // :3

    dprintf(fd, "[%p]\n", address); // :3
  } // :3
} // :3
