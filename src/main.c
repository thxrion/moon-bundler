#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include "utils.h"
#include "config.h"
#include "lua_syntax.h"
#include "lua_module_list.h"

#define INITIAL_MODULES_CAPACITY 256

int main(int argc, char *argv[]) {
    printf("LOG START\n");

    config_t* config = config_new();
    config_process_file(config);

    char cwd[256];
    getcwd(cwd, 256);
    printf("workin dir: %s\n", cwd);
    // config_put_default_values(config);
    printf("source dir: %s\n", config->source_directory);
    printf("bundle dir: %s\n", config->bundle_directory);
    printf("target: %s\n", config->target);
    printf("entry point: %s\n\n", config->entry_point);

    if (access(config->bundle_directory, F_OK)) {
        makedir(config->bundle_directory);
    }

    lua_module_list_t* modules = lua_module_list_new(INITIAL_MODULES_CAPACITY);
    lua_module_list_generate(modules, config->source_directory, config->entry_point);

    if (modules->size) {
        char* bundle_path = malloc(strlen(config->bundle_directory) + strlen("/") + strlen(config->target) + 1);
        sprintf(bundle_path, "%s/%s", config->bundle_directory, config->target);

        FILE* file = fopen(bundle_path, "w");

        fprintf(file, "%s", lua_bundle_prefix);

        for (size_t i = 1; i < modules->size; i++) {
            lua_module_write(file, modules->elements[i]);
            // printf("some module:\n%s\n", modules->elements[i].code);
        }

        fprintf(file, "%s", modules->elements[0].code);

        fclose(file);

        free(bundle_path);
    }

    lua_module_list_free(modules);
    config_free(config);

    return EXIT_SUCCESS;
}
