#include "ECL/include/ecl.h"

int main(int argc, char *argv[])
{
    // Get the value of the configuration.
    // ecl_value_t *value = ecl_decode("name \"app\" version 1 list [10 {one 1 two 2 three 3} 30] config {name \"erik\" age 22}");
    ecl_value_t *value = ecl_decode_file("C:\\ECLc\\sample.ecl");
    // Transpile it to JSON.
    ecl_transpile("out.json", value, ECL_JSON);
    // Transpile it to ECL (clear input).
    ecl_transpile("out.ecl", value, ECL_ECL);
}
