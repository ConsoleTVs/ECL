#ifndef ECL_PARSER_H
#define ECL_PARSER_H

#include "lexer.h"
#include "values.h"

// Prases a single value.
ecl_value_t *ecl_parse_value();
// Parses the input source code.
ecl_value_t *ecl_parse(const ecl_token_list_t *const tokens);
// Prints the value to the STDOUT. In case the value contains inner values, those are also printed.
void ecl_print_value(const ecl_value_t *const value);

#endif
