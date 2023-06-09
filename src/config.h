#include <stdlib.h>
#include <string.h>

void config_free(void);
void config_read_arguments(int argc, char* argv[]);

char* config_bundle_directory;
char* config_source_directory;
char* config_entry_point;
char* config_target;

// args --bundle-directory "path"
// args --working-directory "path"
// args --entry-point "path"
// args --target "name"

// ./bin/lua_bundler.exe --bundle-directory build --working-directory src --entry-point main.lua --target vehicle_manager.lua