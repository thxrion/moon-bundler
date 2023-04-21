#include <stdlib.h>
#include "io.h"

int main(int args_count, char** args) {
    char* file_content = file_read("");
    printf("%s", file_content);
    free(file_content);

    return EXIT_SUCCESS;
}