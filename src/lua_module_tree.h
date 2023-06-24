#include "lua_module.h"

typedef struct {
    lua_module_t* root;
} lua_module_tree_t;

bool lua_module_tree_search(lua_module_tree_t* tree, int target) {
    if (!tree || !tree->root) {
        return false;
    }

    return lua_module_search(tree->root, target);
}
