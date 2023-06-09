#include "io.h"
#include "errors.h"

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

char* get_source_file_path(const char* source_directory, const char* file_name) {
    char working_directory[PATH_MAX];
    if (!getcwd(working_directory, PATH_MAX)) {    
        print_getcwd_errors();
        return NULL;
    }

    char* source_file_path = malloc(strlen(working_directory) + 1 + strlen(source_directory) + 1 + strlen(file_name) + 1);
    sprintf(source_file_path, "%s/%s/%s", working_directory, source_directory, file_name);

    return source_file_path;
}

char* read_source_file(const char* source_directory, const char* file_relative_path) {
    char* source_file_path = get_source_file_path(source_directory, file_relative_path);
    if (!source_file_path) {
        return NULL;
    }

    char* file_content = file_read(source_file_path);
    free(source_file_path);
    
    return file_content;
}

void file_write(const char* path, const char* string) {
    FILE* file = fopen(path, "w");
    fputs(string, file);
    fclose(file);
}