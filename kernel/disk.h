#ifndef _KERNEL_DISK_H
#define _KERNEL_DISK_H

#include "helper.h"
#include "malloc.h"
// 建立一个4MB大小的文件模拟磁盘，按逻辑将其划分为1024块，每块大小4KB。
// 其中900块用于存放普通数据，124块用于存储兑换数据。存储管理需要支持

// 0x600000 - 0xa00000 是模拟的磁盘 
// 前16块用来存放目录信息


#define CHUNK_SIZE (1024*4)
#define CHUNK_NUMB_TO_ADDR(chunk_num) (DISK_START + CHUNK_SIZE*chunk_num)


enum FileType{
    FILE_TYPE,
    DIR_TYPE
};

struct chunks_info
{
    uint64_t start;
    uint64_t chunk_numb;
};

struct FileInfo
{
    void append(FileInfo*); //DIR_TYPE only;
    void list_dir(); //DIR_TYPE only
    FileType type_;
    char name_[16];
    uint64_t content_numb;
    FileInfo* content_[0x10]; // DIR_TYPE only
    FileInfo* where_; //当前文件/目录 在哪个文件夹中
    chunks_info disk_info_;
};


class Disk
{
public:
    Disk() = default;
    chunks_info alloc_chunk(uint64_t file_size);
    void free_chunk(chunks_info info);
    void list_freechunk();
private:
    char* disk_start_ = (char*)DISK_START;
    void next_chunk(){ disk_start_ + CHUNK_SIZE; }
    uint64_t read_idx(uint64_t index);
    void write_idx(uint64_t index,uint64_t value);
    uint64_t bitmap_[16] = {0};
};




#endif