#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "template.h"

const char* DEFAULT_BUNDLE_DIRECTORY = "build";
const char* DEFAULT_WORKING_DIRECTORY = "src";
const char* DEFAULT_ENTRY_POINT = "main";

static char* bundle_directory;
static char* working_directory;
static char* entry_point;
static char* target;

int main(int argc, char *argv[]) {
    printf("args count: %d\n", argc - 1);

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

        if (!strcmp(argument, "--working-directory")) {
            working_directory = strdup(next_argument);
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

    if (!working_directory) {
        working_directory = strdup(DEFAULT_WORKING_DIRECTORY);
    }

    if (!entry_point) {
        entry_point = strdup(DEFAULT_ENTRY_POINT);
    }

    if (!target) {
        printf("Target not specified, please use --target <name>\n");

        free(bundle_directory);
        free(working_directory);
        free(entry_point);

        return EXIT_FAILURE;
    }

    printf("bundle directory: %s\n", bundle_directory);
    printf("working directory: %s\n", working_directory);
    printf("entry point: %s\n", entry_point);
    printf("target: %s\n", target);

    /* char* file_content = file_read("");
    if (!file_content) {
        return EXIT_FAILURE;
    }

    printf("%s", file_content);
    file_write("", file_content);
    free(file_content);
    */

    free(bundle_directory);
    free(working_directory);
    free(entry_point);
    free(target);

    return EXIT_SUCCESS;
}

// ./build/lua_bundler.exe --bundle-directory build --working-directory src --entry-point main.lua --target vehicle_manager.lua

// args --bundle-directory "path"
// args --working-directory "path"
// args --entry-point "path"
// args --target "name"