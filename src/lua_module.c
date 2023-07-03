#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    char module_relative_path[strlen(lua_path) + 1];
    strcpy(module_relative_path, lua_path);
    lua_require_path_format(module_relative_path);

    for (size_t i = 0; i < sizeof(lua_require_path_suffixes) / sizeof(*lua_require_path_suffixes); i++) {
        const char* suffix = lua_require_path_suffixes[i];

        char module_path[strlen(source_directory) + strlen("/") + strlen(module_relative_path) + strlen(suffix) + 1];
        sprintf(module_path, "%s/%s%s", source_directory, module_relative_path, suffix);

        // TODO: add directory check, make sure this is cross platform
        if (access(module_path, F_OK)) {
            continue;
        }

        FILE* file = fopen(module_path, "r");

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* code = malloc(file_size + 1);
        fread(code, file_size, 1, file);
        fclose(file);

        code[file_size] = '\0';

        return code;
    }

    return NULL;
}

int lua_module_write(FILE* file, lua_module_t module) {
    return fprintf(file, lua_module_template, module.path, module.code);
}
