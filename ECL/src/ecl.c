#include "../include/ecl.h"

// In case of mingw use the ANSI stdio to avoid warnings and possible errors.
#define __USE_MINGW_ANSI_STDIO 1

ecl_value_t *ecl_decode(const char *const source)
{
    // Create the tokens vector.
    ecl_token_list_t tokens = ECL_VECTOR_CREATE(20);
    // Scan the source code.
    ecl_scan(&tokens, source);
    // Parse the source code.
    return ecl_parse(&tokens);
}

ecl_value_t *ecl_decode_file(const char *const file_name)
{
    // Open the file in read mode.
    FILE *file = fopen(file_name, "rb");
    // Check if there was an error.
    if (file == NULL) {
        fprintf(stderr, "Error: Can't open file '%s'.", file_name);
        exit(EXIT_FAILURE);
    }
    // Get the file length
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    // Create the string for the file contents.
    char *buffer = malloc(sizeof(char) * (length + 1));
    buffer[length] = '\0';
    // Set the contents of the string.
    fread(buffer, sizeof(char), length, file);
    // Close the file.
    fclose(file);
    // Decode the input.
    ecl_value_t *value = ecl_decode(buffer);
    // Free the allocated string space.
    free(buffer);
    return value;
}

void ecl_transpile(const char *file_name, const ecl_value_t *const value, const ecl_format_t format)
{
    // Open the file in write mode.
    FILE *file = fopen(file_name, "wb");
    // Check if there was an error.
    if (file == NULL) {
        fprintf(stderr, "Error: Can't open file '%s'.", file_name);
        exit(EXIT_FAILURE);
    }
    // Chose the format to transpile to.
    switch (format) {
        case ECL_ECL: { ecl_transpile_to_ecl(file, value, 0, true, false); break; }
        case ECL_JSON: { ecl_transpile_to_json(file, value, 0, true); break; }
        // case ECL_TOML: { break; }
        // case ECL_YAML: { break; }
        default: {
            fprintf(stderr, "Error: Invalid format to transpile to.");
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
}

#undef __USE_MINGW_ANSI_STDIO
