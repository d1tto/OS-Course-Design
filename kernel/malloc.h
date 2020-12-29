#ifndef _KERNEL_MALLOC_H
#define _KERNEL_MALLOC_H
#include "helper.h"

#define CHUNK_ALIGN 0x10
#define ALIGN_SIZE(size) ((size+CHUNK_ALIGN-1)&(~CHUNK_ALIGN))

class MemAlloc;

extern MemAlloc Alloc;

struct ChunkHeader
{   
    uint64_t size;
    ChunkHeader* next;
};
using pChunkHeader = ChunkHeader*;

class MemAlloc
{
public:
    MemAlloc(char* top_chunk):top_chunk_(top_chunk){};
    void* alloc(uint64_t size);
    void free(void* ptr);
private:
    pChunkHeader find_freelist(uint64_t size);
    pChunkHeader free_list_ = nullptr;
    char* top_chunk_;
};

#endif