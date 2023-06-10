#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char** elements;
    size_t size;
    size_t capacity;
} string_set_t;

void string_set_init(string_set_t* set, size_t capacity);
bool string_set_contains(const string_set_t* set, const char* element);
bool string_set_add(string_set_t* set, const char* element);
bool string_set_remove(string_set_t* set, const char* element);
void string_set_print(const string_set_t* set);
void string_set_clear(string_set_t* set);
void string_set_destroy(string_set_t* set);