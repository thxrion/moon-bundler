#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

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

void config_process_arguments(config_t* config, size_t argc, char* argv[]) {
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);

    for (size_t i = 1; i < argc - 1; i++) {
        char* argument = argv[i];
        char* next_argument = argv[i + 1];

        if (!strcmp(argument, "--source-directory")) {
            config->source_directory = malloc(strlen(cwd) + strlen("/") + strlen(next_argument) + 1);
            sprintf(config->source_directory, "%s/%s", cwd, next_argument);
            continue;
        }

        if (!strcmp(argument, "--bundle-directory")) {
            config->bundle_directory = malloc(strlen(cwd) + strlen("/") + strlen(next_argument) + 1);
            sprintf(config->bundle_directory, "%s/%s", cwd, next_argument);
            continue;
        }

        if (!strcmp(argument, "--entry-point")) {
            config->entry_point = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--target")) {
            config->target = strdup(next_argument);
            continue;
        }
    }
}

void config_put_default_values(config_t* config) {
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);

    if (!config->source_directory) {
        config->source_directory = malloc(strlen(cwd) + strlen("/") + strlen(DEFAULT_SOURCE_DIRECTORY) + 1);
        sprintf(config->source_directory, "%s/%s", cwd, DEFAULT_SOURCE_DIRECTORY);
    }

    if (!config->bundle_directory) {
        config->bundle_directory = malloc(strlen(cwd) + strlen("/") + strlen(DEFAULT_BUNDLE_DIRECTORY) + 1);
        sprintf(config->bundle_directory, "%s/%s", cwd, DEFAULT_BUNDLE_DIRECTORY);
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
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);

    config->target = config_file_find_entry(CONFIG_FILE_NAME, "target");
    config->entry_point = config_file_find_entry(CONFIG_FILE_NAME, "entry-point");

    char* relative_source_direcory = config_file_find_entry(CONFIG_FILE_NAME, "source-directory");

    if (relative_source_direcory) {
        config->source_directory = malloc(strlen(cwd) + strlen("/") + strlen(relative_source_direcory) + 1);
        sprintf(config->source_directory, "%s/%s", cwd, relative_source_direcory);
    }

    char* relative_bundle_direcory = config_file_find_entry(CONFIG_FILE_NAME, "bundle-directory");

    if (relative_bundle_direcory) {
        config->bundle_directory = malloc(strlen(cwd) + strlen("/") + strlen(relative_bundle_direcory) + 1);
        sprintf(config->bundle_directory, "%s/%s", cwd, relative_bundle_direcory);
    }
}
