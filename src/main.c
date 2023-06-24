#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#include "lua_syntax.h"
#include "io.h"
#include "string_set.h"

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

string_set_t modules;

void config_read_arguments(size_t argc, char* argv[]) {
    for (size_t i = 1; i < argc - 1; i++) {
        char* argument = argv[i];
        char* next_argument = argv[i + 1];

        if (!strcmp(argument, "--bundle-directory")) {
            config_bundle_directory = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--source-directory")) {
            config_source_directory = malloc(PATH_MAX);
            getcwd(config_source_directory, PATH_MAX);
            sprintf(config_source_directory, "%s/%s", config_source_directory, next_argument);
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

void replace_char_in_string(char* string, char old_char, char new_char) {
    char *ptr = strchr(string, old_char);

    while (ptr != NULL) {
        *ptr = new_char;
        ptr = strchr(ptr + 1, old_char);
    }
}

char* get_source_file_absolute_path(const char* file_relative_path, const char* modifier) {
    char* source_file_path = malloc(strlen(config_source_directory) + strlen("/") + strlen(file_relative_path) + strlen(modifier) + 1);
    sprintf(source_file_path, "%s/%s%s", config_source_directory, file_relative_path, modifier);
    return source_file_path;
}

char* read_module_code(const char* lua_path) {
    char* file_relative_path = strdup(lua_path);
    replace_char_in_string(file_relative_path, '.', '/');

    char* module_as_folder_absolute_path = get_source_file_absolute_path(file_relative_path, "/init.lua");
    char* module_as_file_absolute_path = get_source_file_absolute_path(file_relative_path, ".lua");

    char* module_code = NULL;

    if (!access(module_as_folder_absolute_path, F_OK)) {
        module_code = file_read(module_as_folder_absolute_path);
    }

    if (!module_code && !access(module_as_file_absolute_path, F_OK)) {
        module_code = file_read(module_as_file_absolute_path);
    }

    free(module_as_folder_absolute_path);
    free(module_as_file_absolute_path);
    free(file_relative_path);

    return module_code;
}

void look_for_require_statements_in_lua_module(string_set_t* modules, const char* path) {
    char* lua_code = read_module_code(path);

    if (!lua_code) {
        return;
    }

    const char* curr_pos = lua_code;
    while ((curr_pos = strstr(curr_pos, require_keyword)) != NULL) {
        curr_pos += strlen(require_keyword);

        const char* opening_quote_pos = NULL;
        size_t quote_index;
        for (size_t i = 0; i < 3; i++) {
            const char* quote_pos = strstr(curr_pos, opening_quotes[i]);

            if (!quote_pos) {
                continue;
            }

            if (!opening_quote_pos || quote_pos < opening_quote_pos) {
                opening_quote_pos = quote_pos;
                quote_index = i;
            }
        }

        if (!opening_quote_pos) {
            break;
        }

        curr_pos = opening_quote_pos + strlen(opening_quotes[quote_index]);

        const char* closing_quote_pos = strstr(curr_pos, closing_quotes[quote_index]);
        if (!closing_quote_pos) {
            printf("Found unclosed quote after 'require' statement.\n");
            break;
        }

        size_t lua_module_path_len = closing_quote_pos - curr_pos;
        char* lua_module_path = malloc(lua_module_path_len + 1);
        strncpy(lua_module_path, curr_pos, lua_module_path_len);
        lua_module_path[lua_module_path_len] = '\0';

        if (!string_set_contains(modules, lua_module_path)) {
            string_set_add(modules, lua_module_path);
            look_for_require_statements_in_lua_module(modules, lua_module_path);
        }

        free(lua_module_path);

        curr_pos = closing_quote_pos + strlen(closing_quotes[quote_index]);
    }

    free(lua_code);
}

char* get_module_definition(const char* lua_path) {
    char* module_code = read_module_code(lua_path);

    if (!module_code) {
        return NULL;
    }

    size_t module_definition_len = strlen(MODULE_DEFINITION) + strlen(lua_path) + strlen(module_code) - 2 * sizeof("%s") + 1;
    char* module_definition_code = malloc(module_definition_len);
    sprintf(module_definition_code, MODULE_DEFINITION, lua_path, module_code);

    return module_definition_code;
}

int main(int argc, char *argv[]) {
    config_read_arguments(argc, argv);
    config_init();

    /*if (!config_target) {
        config_free();
        return EXIT_FAILURE;
    }*/

    string_set_init(&modules, INITIAL_MODULES_CAPACITY);

    look_for_require_statements_in_lua_module(&modules, config_entry_point);
    string_set_add(&modules, config_entry_point);

    string_set_print(&modules);

    char* modules_code[modules.size + 1];
    size_t bundle_len = strlen(BUNDLE_PREFIX);

    for (size_t i = 0; i < modules.size; i++) {
        modules_code[i] = get_module_definition(modules.elements[i]);

        if (modules_code[i]) {
            bundle_len += strlen(modules_code[i]);
        }
    }

    char* bundle = malloc(bundle_len + 1);

    strncpy(bundle, BUNDLE_PREFIX, strlen(BUNDLE_PREFIX));

    for (size_t i = 0, next_string_segment = strlen(BUNDLE_PREFIX); i < modules.size; i++) {
        const char* module_code = modules_code[i];
        if (!module_code) {
            continue;
        }

        int module_code_len = strlen(module_code);

        strncpy(bundle + next_string_segment, module_code, module_code_len);
        next_string_segment += module_code_len;
    }

    bundle[bundle_len] = '\0';

    printf("%s\n", bundle);

    file_write("bin/target.lua", bundle);

    free(bundle);

    string_set_clear(&modules);

    config_free();

    return EXIT_SUCCESS;
}
