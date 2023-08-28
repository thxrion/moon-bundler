#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>

#define DEFAULT_BUNDLE_DIRECTORY "bundle"
#define DEFAULT_SOURCE_DIRECTORY "src"
#define DEFAULT_ENTRY_POINT "init"
#define DEFAULT_TARGET "script.lua"

#define CONFIG_FILE_NAME "bundle.cfg"

typedef struct {
    char* bundle_directory;
    char* source_directory;
    char* entry_point;
    char* target;
} config_t;

config_t* config_new(void);
void config_process_arguments(config_t* config, size_t argc, char* argv[]);
void config_put_default_values(config_t* config);
void config_free(config_t* config);
void config_process_file(config_t* config);

#endif
