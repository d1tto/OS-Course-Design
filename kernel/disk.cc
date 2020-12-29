#include "disk.h"
#include "syscall.h"

uint64_t
Disk::read_idx(uint64_t index)
{
    if(index >= 1024)
        return 1;
    uint32_t bitmap_idx = index / 64;
    uint32_t inner_idx = index % 64;

    uint64_t target = bitmap_[bitmap_idx];
    target = (target >> inner_idx);
    target = target & 1;
    return target;
}

void 
Disk::write_idx(uint64_t index,uint64_t value)
{
    if(index >= 1024)
        return;
    uint32_t bitmap_idx = index / 64;
    uint32_t inner_idx = index % 64;

    bitmap_[bitmap_idx] |= (value << inner_idx);

}

void
Disk::list_freechunk()
{
    for(int i = 0x10; i < 1024 ; i++){
        if(!read_idx(i)){
            write_idx(i,1);
            sys_write((uint64_t)"1",1,0);
        }
    }
    for(int i = 0x10; i < 1024 ; i++){
        if(!read_idx(i)){
            sys_write((uint64_t)"2",1,0);
        }
    }
}

chunks_info 
Disk::alloc_chunk(uint64_t file_size)
{
    uint64_t chunk_numb = ((file_size+CHUNK_SIZE-1)&(~CHUNK_SIZE))/CHUNK_SIZE;
    for(int i = 0x10 ; i < 1024 ; i++){
        if(!read_idx(i)){
            bool is_ok = true;
            for(int j = i ; j < i+chunk_numb ; j++){
                if(read_idx(j)){
                    is_ok = false;
                    break;
                }
            }
            if(is_ok)
                return {i,chunk_numb};
        }
    }
    return {0,0};
}

void 
Disk::free_chunk(chunks_info info)
{
    for(int i = info.start ; i < info.start+info.chunk_numb ; i++)
        write_idx(i,0);
}


void 
FileInfo::list_dir()
{
    for(int i = 0 ; i < content_numb ; i++){
        auto p = content_[i];
        write_str(p->name_,strlen(p->name_));
        if(p->type_==DIR_TYPE)
            write_str("(dir)",5);
        write_str("     ",5);
    }
    write_str("\n",1);
}

void 
FileInfo::append(FileInfo* p)
{
    if(content_numb == 0x10-1)
        return;
    content_[content_numb] = p;
    content_numb++;
    //puts(content_[content_numb]->name_);
}