#include "string_set.h"

void string_set_init(string_set_t* set, size_t capacity) {
    set->elements = malloc(capacity * sizeof(char*));
    set->size = 0;
    set->capacity = capacity;
}

bool string_set_contains(const string_set_t* set, const char* element) {
    for (size_t i = 0; i < set->size; i++) {
        if (!strcmp(set->elements[i], element)) {
            return true;
        }
    }
    return false;
}

bool string_set_add(string_set_t* set, const char* element) {
    if (string_set_contains(set, element)) {
        return false;
    }

    if (set->size >= set->capacity) {
        size_t new_capacity = set->capacity * 2;
        char** new_elements = realloc(set->elements, new_capacity * sizeof(char*));
        if (new_elements == NULL) {
            return false;
        }
        set->elements = new_elements;
        set->capacity = new_capacity;
    }

    set->elements[set->size] = strdup(element);
    set->size++;
    return true;
}

bool string_set_remove(string_set_t* set, const char* element) {
    size_t index = 0;
    bool found = false;

    for (size_t i = 0; i < set->size; i++) {
        if (strcmp(set->elements[i], element) == 0) {
            index = i;
            found = true;
            break;
        }
    }

    if (!found) {
        return false;
    }

    free(set->elements[index]);

    for (size_t i = index; i < set->size - 1; i++) {
        set->elements[i] = set->elements[i + 1];
    }

    set->size--;

    if (set->size < set->capacity / 4) {
        size_t new_capacity = set->capacity / 2;
        char** new_elements = realloc(set->elements, new_capacity * sizeof(char*));
        if (new_elements != NULL) {
            set->elements = new_elements;
            set->capacity = new_capacity;
        }
    }

    return true;
}

void string_set_print(const string_set_t* set) {
    printf("String Set:\n");
    for (size_t i = 0; i < set->size; i++) {
        printf("%s\n", set->elements[i]);
    }
}

void string_set_clear(string_set_t* set) {
    for (size_t i = 0; i < set->size; i++) {
        free(set->elements[i]);
    }
    free(set->elements);
    set->elements = NULL;

    set->size = 0;
    set->capacity = 0;
}
