#ifndef ECL_ECL_H
#define ECL_ECL_H

#include "exported.h"
#include "parser.h"
#include "transpiler.h"

// Decodes an input into the given value representation.
EXPORTED ecl_value_t *ecl_decode(const char *const source);
// Decodes an input file into the given value representation.
EXPORTED ecl_value_t *ecl_decode_file(const char *const file_name);
// Transpiles the value into another format.
EXPORTED void ecl_transpile(const char *file_name, const ecl_value_t *const value, const ecl_format_t format);

#endif
