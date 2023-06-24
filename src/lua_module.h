#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct lua_module_t {
    int value;
    struct lua_module_t** submodules;
    size_t submodules_length;
} lua_module_t;

lua_module_t* lua_module_create(int value) {
    lua_module_t* new_node = malloc(sizeof(lua_module_t));

    new_node->value = value;
    new_node->submodules = NULL;
    new_node->submodules_length = 0;

    return new_node;
}

void lua_module_add_submodule(lua_module_t* module, lua_module_t* submodule) {
    if (module == NULL || submodule == NULL) {
        return;
    }

    module->submodules_length++;
    module->submodules = realloc(module->submodules, module->submodules_length * sizeof(lua_module_t*));
    module->submodules[module->submodules_length - 1] = submodule;
}

bool lua_module_search(lua_module_t* node, int target) {
    if (node == NULL) {
        return false;
    }

    if (node->value == target) {
        return true;
    }

    for (int i = 0; i < node->submodules_length; i++) {
        if (search_node(node->submodules[i], target)) {
            return true;
        }
    }

    return false;
}

void lua_module_free(lua_module_t* node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < node->submodules_length; i++) {
        free_tree_node(node->submodules[i]);
    }

    free(node->submodules);
    free(node);
}
