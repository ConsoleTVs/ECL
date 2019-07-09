#ifndef ECL_TRANSPILER_H
#define ECL_TRANSPILER_H

#include "values.h"
#include <stdbool.h>

typedef enum {
    ECL_ECL,
    ECL_JSON,
    // ECL_TOML,
    // ECL_YAML
} ecl_format_t;

void ecl_transpile_to_json(FILE *dest, const ecl_value_t *const value, const uint16_t spaces, const bool initial_spaces);
void ecl_transpile_to_ecl(FILE *dest, const ecl_value_t *const value, const uint16_t spaces, const bool initial_spaces, const bool braces);

#endif
