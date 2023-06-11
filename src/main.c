#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#include "lua_syntax.h"
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
        strcat(config_source_directory, "/test/general/project");
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

void file_write(const char* path, const char* string) {
    FILE* file = fopen(path, "w");
    fputs(string, file);
    fclose(file);
}

char* file_read(const char* path) {
    FILE* file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* string = malloc(file_size + 1);
    fread(string, file_size, 1, file);
    fclose(file);

    string[file_size] = '\0';

    return string;
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

    free(file_relative_path);

    if (!access(module_as_folder_absolute_path, F_OK)) {
        char* module_code = file_read(module_as_folder_absolute_path);
        free(module_as_folder_absolute_path);
        free(module_as_file_absolute_path);

        return module_code;
    }

    if (!access(module_as_file_absolute_path, F_OK)) {
        char* module_code = file_read(module_as_file_absolute_path);
        free(module_as_folder_absolute_path);
        free(module_as_file_absolute_path);

        return module_code;
    }

    return NULL;
}

char* get_module_definition(const char* lua_path, const char* module_code) {
    size_t module_definition_len = strlen(MODULE_DEFINITION) + strlen(lua_path) + strlen(module_code) - 2 * sizeof("%s") + 1;
    char* module_definition_code = malloc(module_definition_len);
    sprintf(module_definition_code, MODULE_DEFINITION, lua_path, module_code);

    return module_definition_code;
}

void look_for_require_statements_in_lua_code(const char* lua_code) {
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
            return;
        }

        curr_pos = opening_quote_pos + strlen(opening_quotes[quote_index]);

        const char* closing_quote_pos = strstr(curr_pos, closing_quotes[quote_index]);
        if (!closing_quote_pos) {
            printf("Found unclosed quote after 'require' statement.\n");
            return;
        }

        size_t lua_modules_path_len = closing_quote_pos - curr_pos;
        char* lua_modules_path = malloc(lua_modules_path_len + 1);
        strncpy(lua_modules_path, curr_pos, lua_modules_path_len);
        lua_modules_path[lua_modules_path_len] = '\0';

        string_set_add(&modules, lua_modules_path);

        curr_pos = closing_quote_pos + strlen(closing_quotes[quote_index]);
    }
}

void process_lua_files_and_shit(const char* lua_code, size_t initial_size) {
    look_for_require_statements_in_lua_code(lua_code);

    for (size_t i = initial_size; i < modules.size; i++) {
        const char* lua_path = modules.elements[i];
        char* module_code = read_module_code(lua_path);
        char* module_definition = get_module_definition(lua_path, module_code);
        free(module_code);
        printf("%s\n", module_definition);
        free(module_definition);
    }

    if (modules.size == initial_size) {
        return;
    }

    process_lua_files_and_shit(lua_code, modules.size);
}

int main(int argc, char *argv[]) {
    config_read_arguments(argc, argv);
    config_init();

    printf("%s\n", config_source_directory);

    /*if (!config_target) {
        config_free();
        return EXIT_FAILURE;
    }*/

    char* main_module_path = get_source_file_absolute_path(config_entry_point, ".lua");

    if (access(main_module_path, F_OK)) {
        free(main_module_path);
        config_free();
        return EXIT_FAILURE;
    }

    char* main_module_code = file_read(main_module_path);

    printf("%s\n", main_module_code);

    
    string_set_init(&modules, INITIAL_MODULES_CAPACITY);

    process_lua_files_and_shit(main_module_code, 0);
    string_set_print(&modules);
    string_set_clear(&modules);

    free(main_module_path);
    free(main_module_code);
    config_free();

    return EXIT_SUCCESS;
}