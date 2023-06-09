#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

char* file_read(const char* path);
char* read_source_file(const char* source_directory, const char* file_relative_path);
void file_write(const char* path, const char* string);