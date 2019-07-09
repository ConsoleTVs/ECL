#ifndef ECL_VECTOR_H
#define ECL_VECTOR_H

#include <stdio.h>

#define ECL_VECTOR_CREATE_TYPE(data_type, type_name) typedef struct { \
    data_type *data; \
    size_t length; \
    size_t capacity; \
    const size_t block; \
} type_name;

#define ECL_VECTOR_CREATE_FINAL_TYPE(data_type, type_name) typedef struct { \
    data_type *data; \
    size_t length; \
} type_name;

#define ECL_VECTOR_TO_FINAL(vector) { \
    .data = (vector)->data, \
    .length = (vector)->length \
}

#define ECL_VECTOR_TO_FINAL_ON(vector, variable) { \
    (variable)->data = (vector)->data; \
    (variable)->length = (vector)->length; \
}

#define ECL_VECTOR_REALLOC(vector, capacity) { \
    if ((vector)->data == NULL && ((vector)->capacity == 0 && capacity > 0)) { \
        do (vector)->capacity += (vector)->block; \
        while ((vector)->capacity < capacity); \
        (vector)->data = malloc(sizeof(*(vector)->data) * (vector)->capacity); \
    } else if ((vector)->capacity < capacity) { \
        do (vector)->capacity += (vector)->block; \
        while ((vector)->capacity < capacity); \
        (vector)->data = realloc((vector)->data, sizeof(*(vector)->data) * (vector)->capacity); \
    } else if ((vector)->capacity > capacity) { \
        while ((vector)->capacity > capacity + (vector)->block) \
            (vector)->capacity -= (vector)->block; \
        (vector)->data = realloc((vector)->data, sizeof(*(vector)->data) * (vector)->capacity); \
    } \
}

// Creates a new vector.
#define ECL_VECTOR_CREATE(block_size) { \
    .data = NULL, \
    .length = 0, \
    .capacity = 0, \
    .block = block_size \
}

// Pushes an element into the vector.
#define ECL_VECTOR_PUSH(vector, element) { \
    size_t capacity = ++(vector)->length; \
    ECL_VECTOR_REALLOC(vector, capacity); \
    memcpy((vector)->data + ((vector)->length - 1), &element, sizeof(element)); \
}

// Pops an element from the vector and returns it.
#define ECL_VECTOR_POP(vector, element) { \
    size_t capacity = --(vector)->length; \
    ECL_VECTOR_REALLOC(vector, capacity); \
}

// Prints the vector information in the stdout.
#define ECL_VECTOR_INFO(vector) { \
    printf("{length: %zu, capacity: %zu, block: %zu}\n", (vector)->length, (vector)->capacity, (vector)->block); \
}

// Deletes the vector's allocated data and the vector itself.
#define ECL_VECTOR_DELETE(vector) { \
    free((vector)->data); \
}

#endif
