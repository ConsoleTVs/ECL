#include "../include/transpiler.h"
#include <string.h>
#include <stdlib.h>

#define PRINT_SPACES(dest, n) for (uint16_t i = 0; i < n; ++i) \
    fprintf(dest, " ")

void ecl_transpile_to_json(FILE *dest, const ecl_value_t *const value, const uint16_t spaces, const bool initial_spaces)
{
    if (initial_spaces) PRINT_SPACES(dest, spaces);
    switch (value->kind) {
        case ECL_VALUE_INT: { fprintf(dest, "%lld", value->i_val); break; }
        case ECL_VALUE_FLOAT: { fprintf(dest, "%f", value->f_val); break; }
        case ECL_VALUE_BOOL: { fprintf(dest, "%s", value->b_val ? "true" : "false"); break; }
        case ECL_VALUE_STRING: { fprintf(dest, "\"%s\"", value->s_val); break; }
        case ECL_VALUE_LIST: {
            if (value->l_val.length == 0) {
                fprintf(dest, "[]");
                break;
            }
            fprintf(dest, "[\n");
            for (size_t i = 0; i < value->l_val.length; ++i) {
                ecl_transpile_to_json(dest, value->l_val.data[i], spaces + 2, true);
                if (i != (value->l_val.length - 1))
                    fprintf(dest, ",\n");
                else
                    fprintf(dest, "\n");
            }
            PRINT_SPACES(dest, spaces);
            fprintf(dest, "]");
            break;
        }
        case ECL_VALUE_DEFS: {
            if (value->d_val.length == 0) {
                fprintf(dest, "{}");
                break;
            }
            fprintf(dest, "{\n");
            for (size_t i = 0; i < value->d_val.length; ++i) {
                PRINT_SPACES(dest, spaces + 2);
                fprintf(dest, "\"%s\": ", value->d_val.data[i].name);
                ecl_transpile_to_json(dest, value->d_val.data[i].value, spaces + 2, false);
                if (i != (value->d_val.length - 1))
                    fprintf(dest, ",\n");
                else
                    fprintf(dest, "\n");
            }
            PRINT_SPACES(dest, spaces);
            fprintf(dest, "}");
            break;
        }
        default: {
            fprintf(stderr, "Error: The kind number '%u' is not known.\n", value->kind);
            exit(EXIT_FAILURE);
        }
    }
}

void ecl_transpile_to_ecl(FILE *dest, const ecl_value_t *const value, const uint16_t spaces, const bool initial_spaces, const bool braces)
{
    if (initial_spaces) PRINT_SPACES(dest, spaces);
    switch (value->kind) {
        case ECL_VALUE_INT: { fprintf(dest, "%lld", value->i_val); break; }
        case ECL_VALUE_FLOAT: { fprintf(dest, "%f", value->f_val); break; }
        case ECL_VALUE_BOOL: { fprintf(dest, "%s", value->b_val ? "y" : "n"); break; }
        case ECL_VALUE_STRING: { fprintf(dest, "\"%s\"", value->s_val); break; }
        case ECL_VALUE_LIST: {
            if (value->l_val.length == 0) {
                fprintf(dest, "[]");
                break;
            }
            fprintf(dest, "[\n");
            for (size_t i = 0; i < value->l_val.length; ++i) {
                ecl_transpile_to_ecl(dest, value->l_val.data[i], spaces + 2, true, true);
                fprintf(dest, "\n");
            }
            PRINT_SPACES(dest, spaces);
            fprintf(dest, "]");
            break;
        }
        case ECL_VALUE_DEFS: {
            if (braces) {
                if (value->d_val.length == 0) {
                    fprintf(dest, "{}");
                    break;
                }
                fprintf(dest, "{\n");
                for (size_t i = 0; i < value->d_val.length; ++i) {
                    PRINT_SPACES(dest, spaces + 2);
                    fprintf(dest, "%s", value->d_val.data[i].name);
                    fprintf(dest, value->d_val.data[i].value->kind == ECL_VALUE_BOOL ? "? " : " ");
                    ecl_transpile_to_ecl(dest, value->d_val.data[i].value, spaces + 2, false, true);
                    fprintf(dest, "\n");
                }
                PRINT_SPACES(dest, spaces);
                fprintf(dest, "}");
            } else {
                for (size_t i = 0; i < value->d_val.length; ++i) {
                    PRINT_SPACES(dest, spaces);
                    fprintf(dest, "%s", value->d_val.data[i].name);
                    fprintf(dest, value->d_val.data[i].value->kind == ECL_VALUE_BOOL ? "? " : " ");
                    ecl_transpile_to_ecl(dest, value->d_val.data[i].value, spaces, false, true);
                    fprintf(dest, "\n");
                }
            }
            break;
        }
        default: {
            fprintf(stderr, "Error: The kind number '%u' is not known.\n", value->kind);
            exit(EXIT_FAILURE);
        }
    }
}

#undef PRINT_SPACES
