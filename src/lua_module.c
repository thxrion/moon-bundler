#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "io.h"
#include "lua_module.h"
#include "lua_syntax.h"

lua_module_t lua_module_duplicate(lua_module_t module) {
    lua_module_t duplicate = {
        .path = strdup(module.path),
        .code = strdup(module.code),
    };

    return duplicate;
}

bool lua_module_check_if_valid(const lua_module_t module) {
    return module.path && module.code;
}

void lua_module_free(lua_module_t module) {
    free(module.path);
    free(module.code);
}

char* lua_module_read_code_by_path(const char* source_directory, const char* lua_path) {
    char* module_relative_path = strdup(lua_path);
    lua_require_path_format(module_relative_path);

    for (size_t i = 0; i < sizeof(lua_require_path_suffixes) / sizeof(*lua_require_path_suffixes); i++) {
        const char* suffix = lua_require_path_suffixes[i];

        char* module_path = malloc(strlen(source_directory) + strlen("/") + strlen(module_relative_path) + strlen(suffix) + 1);
        sprintf(module_path, "%s/%s%s", source_directory, module_relative_path, suffix);

        if (access(module_path, F_OK)) {
            free(module_path);
            continue;
        }

        char* code = file_read(module_path);

        free(module_relative_path);
        free(module_path);

        return code;
    }

    free(module_relative_path);

    return NULL;
}

char* lua_module_get_definition(lua_module_t module) {
    char* definition = malloc(strlen(lua_module_template) + strlen(module.path) + strlen(module.code) - 2 * sizeof("%s") + 1);
    sprintf(definition, lua_module_template, module.path, module.code);
    return definition;
}
