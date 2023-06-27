#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct lua_module_t {
    char* path;
    char* code;
    struct lua_module_t** submodules;
    size_t submodules_length;
} lua_module_t;

lua_module_t* lua_module_create() {
    lua_module_t* lua_module = malloc(sizeof(lua_module_t));

    lua_module->path = NULL;
    lua_module->code = NULL;
    lua_module->submodules = NULL;
    lua_module->submodules_length = 0;

    return lua_module;
}

void lua_module_add_submodule(lua_module_t* module, lua_module_t* submodule) {
    if (module == NULL || submodule == NULL) {
        return;
    }

    module->submodules_length++;
    module->submodules = realloc(module->submodules, module->submodules_length * sizeof(lua_module_t*));
    module->submodules[module->submodules_length - 1] = submodule;
}

bool lua_module_search(lua_module_t* module, const char* lua_path) {
    if (module == NULL) {
        return false;
    }

    if (!strcmp(module->path, lua_path)) {
        return true;
    }

    for (size_t i = 0; i < module->submodules_length; i++) {
        if (lua_module_search(module->submodules[i], lua_path)) {
            return true;
        }
    }

    return false;
}

void lua_module_free(lua_module_t* module) {
    if (module == NULL) {
        return;
    }

    for (size_t i = 0; i < module->submodules_length; i++) {
        lua_module_free(module->submodules[i]);
    }

    free(module->submodules);
    free(module->path);
    free(module->code);
    free(module);
}

char* lua_module_read_code_by_path(const char* lua_path) {
    char* file_relative_path = strdup(lua_path);
    lua_require_path_format(file_relative_path, '.', '/');

    char* module_code = NULL;

    for (size_t i = 0; i < sizeof(lua_require_path_suffixes) / sizeof(*lua_require_path_suffixes); i++) {
        const char* suffix = lua_require_path_suffixes[i]

        size_t path_len = strlen(config_source_directory) + strlen("/") + strlen(file_relative_path) + strlen(suffix) + 1;

        char* module_path = malloc(path_len);

        snprintf(module_path, path_len, "%s/%s%s", config_source_directory, file_relative_path, suffix);

        if (access(module_path, F_OK) && (module_code = file_read(module_path)) != NULL) {
            free(file_relative_path);
            free(module_path);

            size_t module_definition_len = strlen(lua_module_template) + strlen(lua_path) + strlen(module_code) - 2 * sizeof("%s") + 1;

            char* module_code_wrapped = malloc(module_definition_len);
            sprintf(module_code_wrapped, lua_module_template, lua_path, module_code);
            free(module_code);

            return module_code_wrapped;
        }

        free(module_path);
    }

    free(file_relative_path);
    return NULL;
}
