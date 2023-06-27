const char* BUNDLE_PREFIX =
    "__MODULES = {}\n"
    "\n"
    "local __require = require\n"
    "function require(path)\n"
    "\tlocal module = __MODULES[path]\n"
    "\tif module then\n"
    "\t\treturn module()\n"
    "\tend\n"
    "\n"
    "\treturn __require(path)\n"
    "end\n";

const char* lua_module_template =
    "__MODULES[\"%s\"] = function()\n"
    "%s\n"
    "end\n";

const char* require_keyword = "require";
const char* opening_quotes[] = { "'", "\"", "[[" };
const char* closing_quotes[] = { "'", "\"", "]]" };

const char* lua_require_path_suffixes[] = { "/init.lua", ".lua" };

void lua_require_path_format(char* lua_path, char old_char, char new_char) {
    char *ptr = strchr(lua_path, old_char);

    while (ptr != NULL) {
        *ptr = new_char;
        ptr = strchr(ptr + 1, old_char);
    }
}
