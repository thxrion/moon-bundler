#include <stdio.h>
#include <stdlib.h>

char* file_read(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* string = malloc(file_size + 1);
    fread(string, file_size, 1, file);
    fclose(file);

    string[file_size] = '\0';

    return string;
}

int main(int args_count, char** args) {
    char* file_content = file_read("");
    printf("%s", file_content);
    free(file_content);

    return EXIT_SUCCESS;
}