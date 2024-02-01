#include <string.h>

#include "lua_syntax.h"
#include "utils.h"

const char* lua_module_template =
    "package.preload[\"%s\"] = (function()\n"
    "%s\n"
    "end)\n\n";

const char* lua_require_keyword = "require";
const char* lua_opening_quotes[] = { "'", "\"", "[[" };
const char* lua_closing_quotes[] = { "'", "\"", "]]" };

const char* lua_require_path_suffixes[] = { "", ".lua", "/init.lua" };

void lua_require_path_format(char* lua_path) {
    return strreplace(lua_path, '.', '/');
}
