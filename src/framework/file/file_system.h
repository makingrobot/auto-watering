#if CONFIG_USE_FS==1

#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include <FS.h>

class FileSystem {
public:
    FileSystem(fs::FS& fs) : fs_(fs) { }
    ~FileSystem() { }

    void ListDir(const char *dirname, uint8_t levels);
    void CreateDir(const char *path);
    void RemoveDir(const char *path);
    bool OpenFile(const char *path, File* file);
    void WriteFile(const char *path, const char *message);
    void AppendFile(const char *path, const char *message);
    void RenameFile(const char *path1, const char *path2);
    void DeleteFile(const char *path); 

private:
    fs::FS& fs_;
    
};

#endif //_FILE_SYSTEM_H

#endif //CONFIG_USE_FS