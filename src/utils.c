#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "utils.h"

void strreplace(char* lua_path, char old, char new) {
    char *ptr = strchr(lua_path, old);

    while (ptr != NULL) {
        *ptr = new;
        ptr = strchr(ptr + 1, old);
    }
}

// TODO: make sure this one is cross-platform
bool is_file_readable(const char* path) {
    if (access(path, F_OK)) {
        return false;
    }

    struct stat st;
    if (stat(path, &st) != 0) {
        return false;
    }

    if (S_ISREG(st.st_mode)) {
        return true;
    }

    return false;
}
