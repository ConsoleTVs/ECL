#ifndef ECL_VALUES_H
#define ECL_VALUES_H

#include "vector.h"
#include <stdint.h>
#include <stdbool.h>

// Defines the value types found in ECL.
// ECL_VALUE_INT: ..., -2, -1, 0, 1, 2, ...
// ECL_VALUE_FLOAT: ..., -2.0, -1.0, 0.0, 1.0, 2.0, ...
// ECL_VALUE_BOOL: ?y, ?n
// ECL_VALUE_STRING: "sample string"
// ECL_VALUE_LIST: [10 20 30 40 50]
// ECL_VALUE_DEFS: {one 1 two 2 three 3}
typedef enum {
    ECL_VALUE_INT,
    ECL_VALUE_FLOAT,
    ECL_VALUE_BOOL,
    ECL_VALUE_STRING,
    ECL_VALUE_LIST,
    ECL_VALUE_DEFS,
} ecl_value_kind_t;

// Forward declaration;
typedef struct ecl_value_t ecl_value_t;

/* Data types of ECL. */
// INT
typedef int64_t ecl_value_int_t;
// FLOAT
typedef double ecl_value_float_t;
// BOOL
typedef bool ecl_value_bool_t;
// STRING
typedef char * ecl_value_string_t;
// LIST
ECL_VECTOR_CREATE_FINAL_TYPE(ecl_value_t *, ecl_value_list_t);
// DEFINITION (NOT A VALUE!!)
typedef struct {
    char *name;
    ecl_value_t *value;
} ecl_value_def_t;
// DEFINITIONS
ECL_VECTOR_CREATE_FINAL_TYPE(ecl_value_def_t, ecl_value_defs_t);

// The generic value implementation.
typedef struct ecl_value_t {
    // The type of the value.
    ecl_value_kind_t kind;
    // The real value.
    union {
        // Integer value.
        ecl_value_int_t i_val;
        // Float value.
        ecl_value_float_t f_val;
        // Boolean value.
        ecl_value_bool_t b_val;
        // String value.
        ecl_value_string_t s_val;
        // List value.
        ecl_value_list_t l_val;
        // Definitions value.
        ecl_value_defs_t d_val;
    };
} ecl_value_t;

// Create the vector for ecl_value_t.
ECL_VECTOR_CREATE_TYPE(ecl_value_t *, ecl_value_vector_t);
// Create the vector for ecl_value_def_t.
ECL_VECTOR_CREATE_TYPE(ecl_value_def_t, ecl_def_vector_t);

#endif
