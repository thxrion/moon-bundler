#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#ifdef _WIN32
    #include <direct.h>
    #define makedir(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #define makedir(path) mkdir(path, 0777)
#endif

void strreplace(char* lua_path, char old, char new);
bool is_file_readable(const char* path);

#endif
