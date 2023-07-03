#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include "io.h"
#include "config.h"
#include "lua_syntax.h"
#include "lua_module_list.h"

#define INITIAL_MODULES_CAPACITY 256

char* lua_bundle_create(lua_module_list_t* modules) {
    char* modules_code[modules->size];
    size_t bundle_len = 0;

    for (size_t i = 1; i < modules->size; i++) {
        char* definition = lua_module_get_definition(modules->elements[i]);
        bundle_len += strlen(definition);
        modules_code[i - 1] = definition;
    }

    char* main_module_code = modules->elements[0].code;
    modules_code[modules->size - 1] = main_module_code;
    bundle_len += strlen(main_module_code);

    /*
    for (size_t i = 0; i < modules->size; i++) {
        printf("require('%s'):\n%s\n", modules->elements[i].path, modules->elements[i].code);
    }
    */

    char* bundle = malloc(bundle_len + 1);

    printf("bundle len (after malloc): %ld\n", bundle_len + 1);

    size_t next_string_segment_pos = 0;

    for (size_t i = 0; i < sizeof(modules_code) / sizeof(*modules_code); i++) {
        const char* module_code = modules_code[i];
        size_t module_code_len = strlen(module_code);
        strncpy(bundle + next_string_segment_pos, module_code, module_code_len);
        next_string_segment_pos += module_code_len;
    }

    strncpy(bundle + next_string_segment_pos, main_module_code, strlen(main_module_code));

    bundle[bundle_len] = '\0';

    printf("bundle size: %ld\n", bundle_len + 1);

    return bundle;
}

int main(int argc, char *argv[]) {
    config_t config;
    config_build(&config);

    // printf("arguments size: %d\n", argc);

    if (argc > 1) {
        config_process_arguments(&config, argc, argv);
    } else {
        config_process_file(&config);
    }

    config_put_default_values(&config);

    /*
    printf("source dir: %s\n", config.source_directory);
    printf("bundle dir: %s\n", config.bundle_directory);
    printf("target: %s\n", config.target);
    printf("entry point: %s\n", config.entry_point);
    */

    lua_module_list_t modules;
    lua_module_list_build(&modules, INITIAL_MODULES_CAPACITY);
    lua_module_list_generate(&modules, config.source_directory, config.entry_point);

    if (modules.size) {
        char* bundle_path = malloc(strlen(config.bundle_directory) + strlen("/") + strlen(config.target) + 1);
        sprintf(bundle_path, "%s/%s", config.bundle_directory, config.target);

        char* bundle = lua_bundle_create(&modules);

        file_write(bundle_path, bundle);
        free(bundle_path);
        free(bundle);
    }

    lua_module_list_free(&modules);
    config_free(&config);

    return EXIT_SUCCESS;
}
