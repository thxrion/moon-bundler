#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "io.h"
#include "config.h"


// args --bundle-directory "path"
// args --working-directory "path"
// args --entry-point "path"
// args --target "name"

void config_process_arguments(config_t* config, size_t argc, char* argv[]) {
    config->bundle_directory = NULL;
    config->source_directory = NULL;
    config->entry_point = NULL;
    config->target = NULL;

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
}
