#include <stdio.h>
#include <stdlib.h>

char* file_read(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* string = malloc(file_size + 1);
    fread(string, file_size, 1, file);
    fclose(file);

    string[file_size] = '\0';

    return string;
}

void file_write(const char* path, const char* string) {
    FILE* file = fopen(path, "w");
    fputs(string, file);
    fclose(file);
}