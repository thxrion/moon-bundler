#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "config.h"
#include "lua_syntax.h"
#include "lua_module_list.h"

#define INITIAL_MODULES_CAPACITY 256

char* lua_bundle_create(lua_module_list_t* modules) {
    char* modules_code[modules->size];
    size_t bundle_len = strlen(lua_bundle_prefix);

    for (size_t i = 1; i < modules->size; i++) {
        char* definition = lua_module_get_definition(modules->elements[i]);
        bundle_len += strlen(definition);
        modules_code[i - 1] = definition;
    }

    char* main_module_code = modules->elements[0].code;
    modules_code[modules->size - 1] = main_module_code;
    bundle_len += strlen(main_module_code);

    char* bundle = malloc(bundle_len + 1);

    strncpy(bundle, lua_bundle_prefix, strlen(lua_bundle_prefix));

    size_t next_string_segment_pos = strlen(lua_bundle_prefix);

    for (size_t i = 0; i < sizeof(modules_code) / sizeof(*modules_code); i++) {
        const char* module_code = modules_code[i];
        size_t module_code_len = strlen(module_code);
        strncpy(bundle + next_string_segment_pos, module_code, module_code_len);
        next_string_segment_pos += module_code_len;
    }

    strncpy(bundle + next_string_segment_pos, main_module_code, strlen(main_module_code));

    bundle[bundle_len] = '\0';

    return bundle;
}

int main(int argc, char *argv[]) {
    config_t config;
    config_process_arguments(&config, argc, argv);

    lua_module_list_t modules;
    lua_module_list_build(&modules, INITIAL_MODULES_CAPACITY);
    lua_module_list_generate(&modules, config.source_directory, config.entry_point);

    if (modules.size) {
        char* bundle = lua_bundle_create(&modules);
        file_write("bin/target.lua", bundle);
        free(bundle);
    }

    lua_module_list_free(&modules);

    config_free(&config);

    return EXIT_SUCCESS;
}
