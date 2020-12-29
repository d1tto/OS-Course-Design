#ifndef _KERNEL_FILESYSTEM_H
#define _KERNEL_FILESYSTEM_H

#include "disk.h"
#include "helper.h"
#include "malloc.h"

class Filesystem;
extern Filesystem file_system;

class Filesystem
{
public:
    Filesystem();
    void list_dir(){current_dir->list_dir();}
    bool change_dir(const char* name);
    bool create_file(const char* name,uint64_t file_size);
    bool create_dir(const char* name);
    int64_t write_file(const char* file_name,const char*content);
    int64_t read_file(const char* file_name,char* output);
    bool rm_file(const char* file_name);
    const char* current_dir_name();
private:
    void dir_append(FileInfo* p){current_dir->append(p);}
    FileInfo* is_exsit(const char*);
    bool is_dir(const char*);
    FileInfo* current_dir;
    FileInfo* root_dir_;
    Disk disk_;
    MemAlloc disk_alloc_;
};

#endif