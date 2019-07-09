#include "../include/lexer.h"
// In case of mingw use the ANSI stdio to avoid warnings and possible errors.
#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>

#define ADD_TOKEN(token) (column += add_token(tokens, token, &start, &current, line, column))
#define SKIP() ++current; start = current
#define IS_AT_END() (*current == '\0')
#define IS_DIGIT(character) ((character) >= '0' && (character) <= '9')
#define IS_ALPHA(character) (((character) >= 'a' && (character) <= 'z') || ((character) >= 'A' && (character) <= 'Z') || (character) == '_')
#define IS_ALPHANUM(character) (IS_ALPHA(character) || IS_DIGIT(character))

// Defines the tokens found in ECL.
static char *token_text[] = {
    "EOF",
    "LEFT_SQUARE",
    "RIGHT_SQUARE",
    "LEFT_BRACE",
    "RIGHT_BRACE",
    "IDENTIFER",
    "INT",
    "FLOAT",
    "TRUE",
    "FALSE",
    "STRING",
};

static void print_tokens(const ecl_token_list_t *const tokens)
{
    for (size_t i = 0; i < tokens->length; i++) {
        printf("%20s (%.*s)\n", token_text[tokens->data[i].kind], tokens->data[i].length, tokens->data[i].start);
    }
}

static uint16_t add_token(
    ecl_token_list_t *const tokens,
    const ecl_token_kind_t kind,
    const char **const start,
    const char *const *const current,
    const uint32_t line,
    const uint16_t column
) {
    ecl_token_t token = {
        .kind = kind,
        .start = *start,
        .length = *current - *start,
        .line = line,
        .column = column
    };
    ECL_VECTOR_PUSH(tokens, token);
    *start = (char *) (**current == ' ' ? *current + 1 : *current);
    return token.length;
}

void ecl_scan(ecl_token_list_t *const tokens, const char *const source)
{
    // The start char.
    char *start = (char *) source;
    // The current char.
    char *current = start;
    // The current line.
    uint32_t line = 1;
    // The current column.
    uint16_t column = 1;
    // Start scanning.
    while (!IS_AT_END()) {
        char c = *current++;
        switch (c) {
            case ' ': { start = current; ++column; break; }
            case '\r': { printf("OH HI R\n"); }
            case '\t': { ++column; break; }
            case '\n': { start = current; ++line; column = 1; break; }
            case '[': { ADD_TOKEN(ECL_TOKEN_LEFT_SQUARE); break; }
            case ']': { ADD_TOKEN(ECL_TOKEN_RIGHT_SQUARE); break; }
            case '{': { ADD_TOKEN(ECL_TOKEN_LEFT_BRACE); break; }
            case '}': { ADD_TOKEN(ECL_TOKEN_RIGHT_BRACE); break; }
            case '?': {
                while (*current == ' ') { ++current; ++column; }
                switch (*current++) {
                    case 'y': { ADD_TOKEN(ECL_TOKEN_TRUE); break; }
                    case 'n': { ADD_TOKEN(ECL_TOKEN_FALSE); break; }
                    default: {
                        fprintf(stderr, "Error: Invalid boolean value. Expected 'y' or 'n' after '?' at line %u, column %hu\n", line, column);
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            }
            case '#': {
                while (*current != '\n' && !IS_AT_END())
                    SKIP();
                if (!IS_AT_END()) {
                    SKIP();
                    ++line;
                }
                break;
            }
            case '"': {
                // Store the start of the string in case of unterminated string.
                uint32_t initial_line = line;
                uint16_t initial_column = column;
                // Get all the string
                while (*current != '"' && !IS_AT_END()) {
                    if (*current == '\n') ++line;
                    ++current;
                }
                if (IS_AT_END()) {
                    fprintf(stderr, "Error: Unterminated string starting at line %u, column %hu\n", initial_line, initial_column);
                    exit(EXIT_FAILURE);
                }
                ++current; // The last '"'.
                ADD_TOKEN(ECL_TOKEN_STRING);
                break;
            }
            default: {
                if (IS_DIGIT(c)) {
                    // It's a digit, (possible integer or float).
                    // Add all the possible digits.
                    while (IS_DIGIT(*current))
                        ++current;
                    // Check if it's a floating point.
                    if (*current == '.' && IS_DIGIT(*(current + 1))) {
                        ++current; // The '.'.
                        // Add all the possible decimal digits.
                        while (IS_DIGIT(*current))
                            ++current;
                        ADD_TOKEN(ECL_TOKEN_FLOAT);
                        break;
                    }
                    ADD_TOKEN(ECL_TOKEN_INT);
                    break;
                } else if (IS_ALPHA(c)) {
                    // It's an alpha char (possible identifier).
                    // Get all the identifier
                    while (IS_ALPHANUM(*current))
                        ++current;
                    ADD_TOKEN(ECL_TOKEN_IDENTIFER);
                    break;
                }
                fprintf(stderr, "Error: Unknown token '%c' (%u) at line %u, column %hu\n", c, c, line, column);
                exit(EXIT_FAILURE);
            }
        }
    }
    ADD_TOKEN(ECL_TOKEN_EOF);
    print_tokens(tokens);
    printf("\n");
}

#undef ADD_TOKEN
#undef SKIP
#undef IS_AT_END
#undef IS_DIGIT
#undef IS_ALPHA
#undef IS_ALPHANUM

#undef __USE_MINGW_ANSI_STDIO
