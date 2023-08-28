#include <stdio.h>
#include <string.h>

#include "lua_syntax.h"
#include "lua_module_list.h"

lua_module_list_t* lua_module_list_new(const size_t initial_capacity) {
    lua_module_list_t* list = malloc(sizeof(lua_module_list_t));

    list->capacity = initial_capacity;
    list->elements = malloc(initial_capacity * sizeof(lua_module_t));
    list->size = 0;

    return list;
}

bool lua_module_list_contains(lua_module_list_t* list, char* lua_path) {
    for (size_t i = 0; i < list->size; i++) {
        if (!strcmp(list->elements[i].path, lua_path)) {
            return true;
        }
    }

    return false;
}

void lua_module_list_resize(lua_module_list_t* list, const size_t new_capacity) {
    lua_module_t* new_elements = realloc(list->elements, new_capacity * sizeof(lua_module_t));

    list->elements = new_elements;
    list->capacity = new_capacity;
}

void lua_module_list_add(lua_module_list_t* list, lua_module_t module) {
    if (list->size >= list->capacity) {
        lua_module_list_resize(list, list->capacity * 2);
    }

    list->elements[list->size] = lua_module_duplicate(module);
    list->size++;
}

void lua_module_list_free(lua_module_list_t* list) {
    for (size_t i = 0; i < list->size; i++) {
        lua_module_free(list->elements[i]);
    }

    free(list->elements);
    free(list);
}

void lua_module_list_generate(lua_module_list_t* list, const char* source_directory, char* lua_path) {
    lua_module_t module = {
        .path = lua_path,
        .code = lua_module_read_code_by_path(source_directory, lua_path),
    };

    // printf("path: %s, code:\n%s\n", module.path, module.code);

    if (!lua_module_check_if_valid(module)) {for (size_t i = 1; i < modules->size; i++) {
        return;
    }

    lua_module_list_add(list, module);
    printf("module added: %s,\n%s\n", module.path, module.code);
    // TODO: look for segmentation fault cause here
    char* curr_pos = module.code;

    while ((curr_pos = strstr(curr_pos, lua_require_keyword)) != NULL) {
        curr_pos += strlen(lua_require_keyword);

        char* opening_quote_pos = NULL;
        size_t quote_index;

        for (size_t i = 0; i < sizeof(lua_opening_quotes) / sizeof(*lua_opening_quotes); i++) {
            char* quote_pos = strstr(curr_pos, lua_opening_quotes[i]);

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

        curr_pos = opening_quote_pos + strlen(lua_opening_quotes[quote_index]);

        char* closing_quote_pos = strstr(curr_pos, lua_closing_quotes[quote_index]);

        if (!closing_quote_pos) {
            printf("Found unclosed quote after 'require' statement.\n");
            break;
        }

        size_t lua_submodule_path_len = closing_quote_pos - curr_pos;
        char* lua_submodule_path = malloc(lua_submodule_path_len + 1);
        strncpy(lua_submodule_path, curr_pos, lua_submodule_path_len);
        lua_submodule_path[lua_submodule_path_len] = '\0';

        if (!lua_module_list_contains(list, lua_submodule_path)) {
            lua_module_list_generate(list, source_directory, lua_submodule_path);
        }

        free(lua_submodule_path);

        curr_pos = closing_quote_pos + strlen(lua_closing_quotes[quote_index]);
    }
}
