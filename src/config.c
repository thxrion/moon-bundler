#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "utils.h"
#include "config.h"

#define MAX_LINE_LENGTH 256

// args --bundle-directory "path"
// args --working-directory "path"
// args --entry-point "path"
// args --target "name"

config_t* config_new(void) {
    config_t* config = malloc(sizeof(config_t));

    config->bundle_directory = NULL;
    config->source_directory = NULL;
    config->entry_point = NULL;
    config->target = NULL;

    return config;
}

void config_put_default_values(config_t* config) {
    if (!config->source_directory) {
        config->source_directory = strdup(DEFAULT_SOURCE_DIRECTORY);
    }

    if (!config->bundle_directory) {
        config->bundle_directory = strdup(DEFAULT_BUNDLE_DIRECTORY);
    }

    if (!config->entry_point) {
        config->entry_point = strdup(DEFAULT_ENTRY_POINT);
    }

    if (!config->target) {
        config->target = strdup(DEFAULT_TARGET);
    }
}

void config_free(config_t* config) {
    free(config->bundle_directory);
    free(config->source_directory);
    free(config->entry_point);
    free(config->target);
    free(config);
}

char* config_file_find_entry(const char* path, const char* key) {
    if (!is_file_readable(path)) {
        return NULL;
    }

    FILE* file = fopen(path, "r");

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL) {
        char found_key[MAX_LINE_LENGTH];
        char found_value[MAX_LINE_LENGTH];

        if (sscanf(line, "%s = %s\n", found_key, found_value) == 2 && !strcmp(found_key, key)) {
            fclose(file);

            return strdup(found_value);
        }
    }

    fclose(file);

    return NULL;
}

void config_process_file(config_t* config) {
    config->target = config_file_find_entry(CONFIG_FILE_NAME, "target");
    config->entry_point = config_file_find_entry(CONFIG_FILE_NAME, "entry-point");
    config->source_directory = config_file_find_entry(CONFIG_FILE_NAME, "source-directory");
    config->bundle_directory = config_file_find_entry(CONFIG_FILE_NAME, "bundle-directory");
}
