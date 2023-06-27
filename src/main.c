#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "io.h"
#include "lua_syntax.h"
#include "lua_module_list.h"

#define INITIAL_MODULES_CAPACITY 256

// args --bundle-directory "path"
// args --working-directory "path"
// args --entry-point "path"
// args --target "name"

// ./bin/lua_bundler.exe --bundle-directory build --working-directory src --entry-point main.lua --target vehicle_manager.lua

static char* config_bundle_directory;
static char* config_source_directory;
static char* config_entry_point;
static char* config_target;

void config_read_arguments(size_t argc, char* argv[]) {
    for (size_t i = 1; i < argc - 1; i++) {
        char* argument = argv[i];
        char* next_argument = argv[i + 1];

        if (!strcmp(argument, "--bundle-directory")) {
            config_bundle_directory = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--source-directory")) {
            char cwd[PATH_MAX];
            getcwd(cwd, PATH_MAX);

            config_source_directory = malloc(strlen(cwd) + strlen("/") + strlen(next_argument) + 1);
            sprintf(config_source_directory, "%s/%s", cwd, next_argument);

            continue;
        }

        if (!strcmp(argument, "--entry-point")) {
            config_entry_point = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--target")) {
            config_target = strdup(next_argument);
            continue;
        }
    }
}

void config_init(void) {
    if (!config_bundle_directory) {
        config_bundle_directory = strdup("build");
    }

    if (!config_source_directory) {
        config_source_directory = malloc(PATH_MAX);
        getcwd(config_source_directory, PATH_MAX);
        strcat(config_source_directory, "/test/deep-import/project");
    }

    if (!config_entry_point) {
        config_entry_point = strdup("main");
    }
}

void config_free(void) {
    free(config_bundle_directory);
    free(config_source_directory);
    free(config_entry_point);
    free(config_target);
}

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
        printf("code %ld: %s\n", i, module_code);
        size_t module_code_len = strlen(module_code);
        strncpy(bundle + next_string_segment_pos, module_code, module_code_len);
        next_string_segment_pos += module_code_len;
    }

    strncpy(bundle + next_string_segment_pos, main_module_code, strlen(main_module_code));

    bundle[bundle_len] = '\0';

    return bundle;
}

int main(int argc, char *argv[]) {
    config_read_arguments(argc, argv);
    config_init();

    /*if (!config_target) {
        config_free();
        return EXIT_FAILURE;
    }*/

    lua_module_list_t modules;
    lua_module_list_build(&modules, INITIAL_MODULES_CAPACITY);
    lua_module_list_generate(&modules, config_source_directory, config_entry_point);

    char* bundle = lua_bundle_create(&modules);
    file_write("bin/target.lua", bundle);
    free(bundle);

    lua_module_list_free(&modules);
    config_free();

    return EXIT_SUCCESS;
}
