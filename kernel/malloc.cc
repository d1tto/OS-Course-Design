#include "malloc.h"

pChunkHeader
MemAlloc::find_freelist(uint64_t size)
{
    if(!free_list_)
        return nullptr;
    pChunkHeader cur = free_list_;
    pChunkHeader prev = cur;
    while(cur){
        if(cur->size >= size){
            if(cur == free_list_)
                free_list_ = free_list_->next;
            else
                prev->next = cur->next;
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }
    return nullptr;
}

void*
MemAlloc::alloc(uint64_t size)
{
    size += 0x10; // ChunkHeader
    size = ALIGN_SIZE(size);
    pChunkHeader p = find_freelist(size);
    if(p){
        puts("fuck");
        return (void*)(p+1);
    }
    p = (pChunkHeader)top_chunk_;

    p->next = nullptr;
    p->size = size;
    top_chunk_+=size;
    return (void*)(p+1);
}

void
MemAlloc::free(void* userdata)
{
    pChunkHeader p = ((pChunkHeader)userdata)-1;
    if(!free_list_){
        free_list_ = p;
        return;
    }
    p->next = free_list_;
    free_list_ = p;
}