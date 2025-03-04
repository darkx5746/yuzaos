/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include <windef.h>

//#define KMALLOC_DEBUG_LARGE_ALLOCATIONS

#define KMALLOC_SCRUB_BYTE 0xbb
#define KFREE_SCRUB_BYTE 0xaa

void kmalloc_init(size_t base, size_t heap_size);
void* kmalloc_impl(size_t);
void* kmalloc_eternal(size_t);
void* kmalloc_page_aligned(size_t);
void* kmalloc_aligned(size_t, size_t alignment);
void* krealloc(void*, size_t);
void*  kcalloc(size_t count, size_t size);
void kfree(void*);
void kfree_aligned(void*);

extern volatile size_t sum_alloc;
extern volatile size_t sum_free;
extern volatile size_t kmalloc_sum_eternal;
extern volatile size_t kmalloc_sum_page_aligned;
extern uint32 g_kmalloc_call_count;
extern uint32 g_kfree_call_count;
extern bool g_dump_kmalloc_stacks;

#ifdef __cplusplus
inline void* operator new(size_t, void* p) { return p; }
inline void* operator new[](size_t, void* p) { return p; }


#endif

inline void* kmalloc2(size_t size)
{
#ifdef KMALLOC_DEBUG_LARGE_ALLOCATIONS
    // Any kernel allocation >= 1M is 99.9% a bug.
    if (size >= 1048576)
        asm volatile("cli;hlt");
#endif
    return kmalloc_impl(size);
}

inline void* kmalloc(size_t size)
{
    void* ptr = kmalloc_impl(size + 4096 + sizeof(void*));
    size_t max_addr = (size_t)ptr + 4096;
    void* aligned_ptr = (void*)(max_addr - (max_addr % 4096));
    ((void**)aligned_ptr)[-1] = ptr;
    return aligned_ptr;

#ifdef KMALLOC_DEBUG_LARGE_ALLOCATIONS
    // Any kernel allocation >= 1M is 99.9% a bug.
    if (size >= 1048576)
        asm volatile("cli;hlt");
#endif
    return kmalloc_impl(size);
}

#ifdef __cplusplus
extern "C" {
#endif
inline void* malloc(size_t size)
{
    return kmalloc(size);
}
inline void free(void* p)
{
    kfree(p);
}

#ifdef __cplusplus
}
#endif