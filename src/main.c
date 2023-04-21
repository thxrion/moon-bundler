#include <stdlib.h>
#include "io.h"

static unsigned int ITERATIONS_LIMIT = 64;
static const char* BUNDLE_PREFIX = "local function __require(f) return f() end";
static const char* MODULE_WRAPPER = "__call(function()\
    %s\
end)\
";

int main(int args_count, char** args) {
    char* file_content = file_read("");
    if (!file_content) {
        return EXIT_FAILURE;
    }

    printf("%s", file_content);

    file_write("", file_content);

    free(file_content);

    return EXIT_SUCCESS;
}