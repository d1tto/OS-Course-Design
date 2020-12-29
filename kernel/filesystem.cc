#include "filesystem.h"

Filesystem file_system;

Filesystem::Filesystem():disk_alloc_((char*)DISK_START)
{
    
    //FileInfo这个结构体在磁盘前0x10个块上分配
    auto p = (FileInfo*)disk_alloc_.alloc(sizeof(FileInfo));
    p->where_ = nullptr;
    p->type_ = DIR_TYPE;
    p->content_numb = 0;
    strcpy(p->name_,"/");
    root_dir_ = p;
    current_dir = root_dir_;
}


bool
Filesystem::rm_file(const char* file_name)
{
    for(int i = 0 ; i < current_dir->content_numb ; i++){
        auto &p = current_dir->content_[i];
        if(!strcmp(p->name_,file_name)){
            for(int j = i ; j < current_dir->content_numb-1 ; j++){
                current_dir->content_[j] = current_dir->content_[j+1];
            }
            current_dir->content_[current_dir->content_numb-1] = nullptr;
            current_dir->content_numb--;
            return true;
        }
    }
    return false;
}

const char* 
Filesystem::current_dir_name()
{
    return current_dir->name_;
}

bool 
Filesystem::change_dir(const char* name)
{
    if(!strcmp(name,"..")){
        auto parent = current_dir->where_;
        if(!parent)
            return false;
        current_dir = parent;
        return true;
    }
    auto p = is_exsit(name);
    if(!p)
        return false;
    if(p->type_ == DIR_TYPE){
        current_dir = p;
        return true;
    }
    return false;
}

bool
Filesystem::create_file(const char* name,uint64_t file_size)
{
    if(is_exsit(name))
        return false;
    //FileInfo这个结构体在磁盘前0x10个块上分配
    auto p = (FileInfo*)disk_alloc_.alloc(sizeof(FileInfo));
    //char buf[0x10]={0};
    //puts(digitToAlpha((uint64_t)p,buf,16));
    p->type_ = FILE_TYPE;
    strcpy(p->name_,name);
    p->disk_info_ = disk_.alloc_chunk(file_size);
    p->where_ = current_dir;
    dir_append(p);
    return true;
}
bool 
Filesystem::create_dir(const char* name)
{
    if(is_exsit(name))
        return false;
    //FileInfo这个结构体在磁盘前0x10个块上分配
    auto p = (FileInfo*)disk_alloc_.alloc(sizeof(FileInfo));
    p->type_ = DIR_TYPE;
    p->content_numb = 0;
    p->where_ = current_dir;
    strcpy(p->name_,name);
    dir_append(p);
    return true;
}

#define _OK 1
#define _NOT_EXSIT 0
#define _IS_DIR 2

int64_t 
Filesystem::write_file(const char* file_name,const char*content)
{
    auto p = is_exsit(file_name);
    if(!p)
        return _NOT_EXSIT;
    if(p->type_ == DIR_TYPE)
        return _IS_DIR;
    
    char* target_addr = (char*)CHUNK_NUMB_TO_ADDR(p->disk_info_.start);
    strcpy(target_addr,content);
    return _OK;
}

int64_t 
Filesystem::read_file(const char* file_name,char* output)
{
    auto p = is_exsit(file_name);
    if(!p)
        return _NOT_EXSIT;
    if(p->type_ == DIR_TYPE)
        return _IS_DIR;
    
    char* target_addr = (char*)CHUNK_NUMB_TO_ADDR(p->disk_info_.start);
    strcpy(output,target_addr);
    return _OK;
}

FileInfo*
Filesystem::is_exsit(const char* name)
{
    for(int i = 0 ; i < current_dir->content_numb ; i++){
        auto p = current_dir->content_[i];
        if(!strcmp(p->name_,name))
            return p;
    }
    return nullptr;
}

bool 
Filesystem::is_dir(const char* name)
{
    for(int i = 0 ; i < current_dir->content_numb ; i++){
        auto p = current_dir->content_[i];
        if(p->type_ == DIR_TYPE)
            return true;
    }
    return false;
}

