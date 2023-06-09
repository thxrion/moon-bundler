#include "config.h"
#include "errors.h"

void config_read_arguments(int argc, char* argv[]) {
    for (int i = 1; i < argc - 1; i++) {
        char* argument = argv[i];
        char* next_argument = argv[i + 1];

        if (!strcmp(argument, "--bundle-directory")) {
            config_bundle_directory = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--source-directory")) {
            config_source_directory = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--entry-point")) {
            config_entry_point = strdup(next_argument);
            continue;
        }

        if (!strcmp(argument, "--target")) {
            config_target = strdup(next_argument);
            continue;
        }
    }

    if (!config_bundle_directory) {
        config_bundle_directory = strdup("build");
    }

    if (!config_source_directory) {
        config_source_directory = strdup("test/general");
    }

    if (!config_entry_point) {
        config_entry_point = strdup("expected.lua");
    }
}

void config_free(void) {
    free(config_bundle_directory);
    free(config_source_directory);
    free(config_entry_point);
    free(config_target);
}