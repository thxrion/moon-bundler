#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "template.h"
#include "errors.h"
#include "config.h"

/*
char* get_module_code(const char* lua_path) {
    char* module_code = read_source_file(source_directory, file_relative_path);
    char* module_code_wrapped = malloc(MODULE_DEFINITION)
}
*/

int main(int argc, char *argv[]) {
    if (argc < 1) {
        // look for config
    }

    config_read_arguments(argc, argv);

    if (!config_target) {
        print_target_not_found_error();
        config_free();
        return EXIT_FAILURE;
    }

    printf("bundle directory: %s\n", config_bundle_directory);
    printf("source directory: %s\n", config_source_directory);
    printf("entry point: %s\n", config_entry_point);
    printf("target: %s\n", config_target);

    char* file_content = read_source_file(config_source_directory, config_entry_point);
    if (!file_content) {
        config_free();
        return EXIT_FAILURE;
    }

    printf("%s", file_content);
    // file_write("", file_content);
    free(file_content);

    config_free();

    return EXIT_SUCCESS;
}