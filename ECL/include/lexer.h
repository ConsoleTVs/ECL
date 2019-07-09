#ifndef ECL_LEXER_H
#define ECL_LEXER_H

#include "vector.h"
#include <stdint.h>
#include <stdlib.h>

// Defines the tokens found in ECL.
typedef enum {
    ECL_TOKEN_EOF,
    ECL_TOKEN_LEFT_SQUARE,
    ECL_TOKEN_RIGHT_SQUARE,
    ECL_TOKEN_LEFT_BRACE,
    ECL_TOKEN_RIGHT_BRACE,
    ECL_TOKEN_IDENTIFER,
    ECL_TOKEN_INT,
    ECL_TOKEN_FLOAT,
    ECL_TOKEN_TRUE,
    ECL_TOKEN_FALSE,
    ECL_TOKEN_STRING,
} ecl_token_kind_t;

typedef struct {
    // The kind of token.
    const ecl_token_kind_t kind;
    // The start of the token.
    const char *start;
    // The token length.
    const uint16_t length;
    // The token line.
    const uint32_t line;
    // The token column.
    const uint16_t column;
} ecl_token_t;

// Create the vector for ecl_token_list_t.
ECL_VECTOR_CREATE_TYPE(ecl_token_t, ecl_token_list_t);

void ecl_scan(ecl_token_list_t *const tokens, const char *const source);

#endif
