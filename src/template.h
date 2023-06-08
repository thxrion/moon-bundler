static const char* BUNDLE_PREFIX =
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

static const char* MODULE_DEFINITION = 
    "__MODULES[\"%s\"] = function()\n"
    "%s\n"
    "end\n";