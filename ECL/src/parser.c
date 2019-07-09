#include "../include/parser.h"
#include "../include/transpiler.h"
#include <stdbool.h>
#include <string.h>

#define PRINT_SPACES(n) for (uint16_t i = 0; i < n; ++i) \
    printf(" ")
#define CHECK(against) current->kind == (against)
#define IS_AT_END() CHECK(ECL_TOKEN_EOF)
#define NEXT() current++
#define PREVIOUS() (current - 1)
#define GET_STRING(token, string) malloc(sizeof(char) * ((token)->length + 1)); \
    memcpy(string, (token)->start, sizeof(char) * ((token)->length + 1)); \
    (string)[(token)->length] = '\0';
#define GET_STRING_OF_STRING(token, string) malloc(sizeof(char) * ((token)->length - 1)); \
    memcpy(string, (token)->start + 1, sizeof(char) * ((token)->length - 1)); \
    (string)[(token)->length - 2] = '\0';

// Stores the current token beeing parsed.
static ecl_token_t *current;

static ecl_value_string_t previous_to_string()
{
    char *string = GET_STRING_OF_STRING(PREVIOUS(), string);
    return string;
}

static ecl_value_int_t previous_to_int()
{
    char *string = GET_STRING(PREVIOUS(), string);
    ecl_value_int_t result = atol(string);
    free(string);
    return result;
}

static ecl_value_float_t previous_to_float()
{
    char *string = GET_STRING(PREVIOUS(), string);
    ecl_value_int_t result = atof(string);
    free(string);
    return result;
}

static ecl_value_t *make_value(const ecl_value_kind_t kind)
{
    ecl_value_t *value = malloc(sizeof(ecl_value_t));
    value->kind = kind;
    switch (value->kind) {
        case ECL_VALUE_INT: { value->i_val = previous_to_int(); break; }
        case ECL_VALUE_FLOAT: { value->f_val = previous_to_float(); break; }
        case ECL_VALUE_STRING: { value->s_val = previous_to_string(); break; }
        default: {
            fprintf(stderr, "Error: Invalid value creation. Internal error, please contact Erik Campobadal <soc@erik.cat>.");
            exit(EXIT_FAILURE);
        }
    }
    return value;
}

static ecl_value_t *make_bool(const ecl_value_bool_t boolean)
{
    ecl_value_t *value = malloc(sizeof(ecl_value_t));
    value->kind = ECL_VALUE_BOOL;
    value->b_val = boolean;
    return value;
}

static ecl_token_t *token_consume(const ecl_token_kind_t kind, const char *const error_message)
{
    if (CHECK(kind))
        return NEXT();
    fprintf(stderr, "Error: %s\n", error_message);
    exit(EXIT_FAILURE);
}

static bool token_match(const ecl_token_kind_t kind)
{
    if (CHECK(kind)) {
        if (kind != ECL_TOKEN_EOF)
            NEXT();
        return true;
    }
    return false;
}

static ecl_value_t *parse_list()
{
    // Create the list vector.
    ecl_value_vector_t list = ECL_VECTOR_CREATE(5);
    // Check if it's an empty list.
    if (token_match(ECL_TOKEN_RIGHT_SQUARE))
        goto ecl_parse_list_create;
    for (;;) {
        if (IS_AT_END()) {
            fprintf(stderr, "Error: Unfinished list. Expected ']' after the last list element or after the initial '['.");
            exit(EXIT_FAILURE);
        }
        // Get the value of the list element.
        ecl_value_t *value = ecl_parse_value();
        // Push it to the list
        ECL_VECTOR_PUSH(&list, value);
        // Stop if the end of the list is found.
        if (token_match(ECL_TOKEN_RIGHT_SQUARE))
            break;
    }
    // Create the value of the definitions.
    ecl_parse_list_create:
    ecl_value_t *value = malloc(sizeof(ecl_value_t));
    // Assign it's kind.
    value->kind = ECL_VALUE_LIST;
    // Create the list value.
    ECL_VECTOR_TO_FINAL_ON(&list, &value->l_val);
    return value;
}

static ecl_value_t *parse_definitions(const bool initial)
{
    // Create the definitions vector.
    ecl_def_vector_t definitions = ECL_VECTOR_CREATE(5);
    // Check if the definitions are empty.
    if (token_match(ECL_TOKEN_RIGHT_BRACE) || (IS_AT_END() && initial))
        goto ecl_parse_definitions_create;
    for (;;) {
        if (IS_AT_END()) {
            fprintf(stderr, "Error: Unfinished definitions. Expected '}' after the last list element or after the initial '{'.");
            exit(EXIT_FAILURE);
        }
        // The evaluations of the initialization list expressions are indeterminately sequenced with respect to
        // one another and thus the order in which any side effects occur is unspecified. That's why we have to
        // manually create the definition.
        ecl_value_def_t definition;
        // Get the identifier.
        char *err1 = initial ? "Expected an identifier." : "Expected an identifier after the '{'.";
        ecl_token_t *identifier = token_consume(ECL_TOKEN_IDENTIFER, err1);
        // Create the string representation of it and append it to the definition.
        definition.name = GET_STRING(identifier, definition.name);
        // Get the value and append it to the definition.
        definition.value = ecl_parse_value();
        // Push it to the definition vector.
        ECL_VECTOR_PUSH(&definitions, definition);
        // Stop if the end of the definitions is found.
        if (token_match(ECL_TOKEN_RIGHT_BRACE) || (IS_AT_END() && initial))
            break;
    }
    // Create the value of the definitions.
    ecl_parse_definitions_create:
    ecl_value_t *value = malloc(sizeof(ecl_value_t));
    // Assign it's kind.
    value->kind = ECL_VALUE_DEFS;
    // Create the definitions value.
    ECL_VECTOR_TO_FINAL_ON(&definitions, &value->d_val);
    return value;
}

ecl_value_t *ecl_parse_value()
{
    if (token_match(ECL_TOKEN_INT)) // Integer
        return make_value(ECL_VALUE_INT);
    if (token_match(ECL_TOKEN_FLOAT)) // Float
        return make_value(ECL_VALUE_FLOAT);
    if (token_match(ECL_TOKEN_TRUE)) // Boolean (true)
        return make_bool(true);
    if (token_match(ECL_TOKEN_FALSE)) // Boolean (false)
        return make_bool(false);
    if (token_match(ECL_TOKEN_STRING)) // String
        return make_value(ECL_VALUE_STRING);
    if (token_match(ECL_TOKEN_LEFT_SQUARE)) // List
        return parse_list();
    if (token_match(ECL_TOKEN_LEFT_BRACE)) // Definitions
        return parse_definitions(false);
}

ecl_value_t *ecl_parse(const ecl_token_list_t *const tokens)
{
    if (tokens->length == 0) {
        fprintf(stderr, "Error: No input tokens to parse. Perhaps an empty file?");
        exit(EXIT_FAILURE);
    }
    current = tokens->data; // current = &tokens->data[0];
    ecl_value_t *result = parse_definitions(true);
    ecl_print_value(result);
    return result;
}

void ecl_print_value(const ecl_value_t *const value)
{
    ecl_transpile_to_json(stdout, value, 0, true);
    printf("\n");
}

#undef PRINT_SPACES
#undef CHECK
#undef IS_AT_END
#undef NEXT
#undef PREVIOUS
#undef GET_STRING
#undef GET_STRING_OF_STRING
