#ifndef LUA_MODULE_H
#define LUA_MODULE_H

#include <stdbool.h>

typedef struct {
    char* path;
    char* code;
} lua_module_t;

lua_module_t lua_module_duplicate(lua_module_t module);
bool lua_module_check_if_valid(const lua_module_t module);
void lua_module_free(lua_module_t module);
void lua_module_print(const lua_module_t module);
char* lua_module_read_code_by_path(const char* source_directory, const char* lua_path);
char* lua_module_get_definition(lua_module_t module);

#endif
