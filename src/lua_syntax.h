#ifndef LUA_SYNTAX_H
#define LUA_SYNTAX_H

extern const char* lua_module_template;
extern const char* lua_require_keyword;
extern const char* lua_opening_quotes[3];
extern const char* lua_closing_quotes[3];
extern const char* lua_require_path_suffixes[3];

void lua_require_path_format(char* lua_path);

#endif
