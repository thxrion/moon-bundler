#ifndef LUA_MODULE_LIST_H
#define LUA_MODULE_LIST_H

#include <stdlib.h>

#include "lua_module.h"

typedef struct {
    lua_module_t* elements;
    size_t size;
    size_t capacity;
} lua_module_list_t;

lua_module_list_t* lua_module_list_new(const size_t initial_capacity);
bool lua_module_list_contains(lua_module_list_t* list, char* lua_path);
void lua_module_list_resize(lua_module_list_t* list, const size_t new_capacity);
void lua_module_list_add(lua_module_list_t* list, lua_module_t module);
void lua_module_list_free(lua_module_list_t* list);
void lua_module_list_generate(lua_module_list_t* list, const char* source_directory, char* lua_path);

#endif
