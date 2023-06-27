#include "lua_module.h"

typedef struct {
    lua_module_t* root;
} lua_module_tree_t;

bool lua_module_tree_search(lua_module_tree_t* tree, const char* lua_path) {
    if (!tree || !tree->root) {
        return false;
    }

    return lua_module_search(tree->root, lua_path);
}

lua_module_t* lua_module_(const lua_module* tree, lua_module_t* module, const char* lua_path) {
    char* code = lua_module_read(lua_path);

    if (!code) {
        return module;
    }

    const char* curr_pos = code;

    while ((curr_pos = strstr(curr_pos, require_keyword)) != NULL) {
        curr_pos += strlen(require_keyword);

        const char* opening_quote_pos = NULL;
        size_t quote_index;

        for (size_t i = 0; i < sizeof(opening_quotes) / sizeof(*opening_quotes); i++) {
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

        lua_module_t submodule = lua_module_create();

        size_t lua_module_path_len = closing_quote_pos - curr_pos;
        char* lua_module_path = malloc(lua_module_path_len + 1);
        strncpy(lua_module_path, curr_pos, lua_module_path_len);
        lua_module_path[lua_module_path_len] = '\0';

        if (!lua_module_tree_search(tree, lua_module_path)) {
            string_set_add(modules, lua_module_path);
            look_for_require_statements_in_lua_module(modules, lua_module_path);
        }

        free(lua_module_path);

        curr_pos = closing_quote_pos + strlen(closing_quotes[quote_index]);
    }

    free(code);
}
