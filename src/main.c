#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "io.h"
#include "template.h"
#include "errors.h"

#define BUFFER_SIZE 256

const char* DEFAULT_BUNDLE_DIRECTORY = "build";
const char* DEFAULT_SOURCE_DIRECTORY = "src";
const char* DEFAULT_ENTRY_POINT = "main";

static char* bundle_directory;
static char* source_directory;
static char* entry_point;
static char* target;


int main(int argc, char *argv[]) {
    char working_directory[BUFFER_SIZE];
    if (getcwd(working_directory, BUFFER_SIZE) == NULL) {    
        print_getcwd_errors();
        return EXIT_FAILURE;
    }

    printf("working directory: %s\n", working_directory);

    if (argc < 1) {
        // look for config
    }

    for (size_t i = 1; i < argc - 1; i++) {
        char* argument = argv[i];
        char* next_argument = argv[i + 1];

        if (!strcmp(argument, "--bundle-directory")) {
            bundle_directory = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--source-directory")) {
            source_directory = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--entry-point")) {
            entry_point = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--target")) {
            target = strdup(next_argument);
            continue;
        }
    }

    if (!bundle_directory) {
        bundle_directory = strdup(DEFAULT_BUNDLE_DIRECTORY);
    }

    if (!source_directory) {
        source_directory = strdup(DEFAULT_SOURCE_DIRECTORY);
    }

    if (!entry_point) {
        entry_point = strdup(DEFAULT_ENTRY_POINT);
    }

    if (!target) {
        print_target_not_found_error();

        free(bundle_directory);
        free(source_directory);
        free(entry_point);

        return EXIT_FAILURE;
    }

    printf("bundle directory: %s\n", bundle_directory);
    printf("source directory: %s\n", source_directory);
    printf("entry point: %s\n", entry_point);
    printf("target: %s\n", target);

    char absolute_path[BUFFER_SIZE];
    sprintf(absolute_path, "%s\\%s\\%s.lua", working_directory, source_directory, entry_point);

    char* file_content = file_read(absolute_path);
    if (!file_content) {
        free(bundle_directory);
        free(source_directory);
        free(entry_point);
        free(target);
        
        return EXIT_FAILURE;
    }

    printf("%s", file_content);
    file_write("", file_content);
    free(file_content);

    free(bundle_directory);
    free(source_directory);
    free(entry_point);
    free(target);

    return EXIT_SUCCESS;
}

// ./build/lua_bundler.exe --bundle-directory build --working-directory src --entry-point main.lua --target vehicle_manager.lua

// args --bundle-directory "path"
// args --working-directory "path"
// args --entry-point "path"
// args --target "name"