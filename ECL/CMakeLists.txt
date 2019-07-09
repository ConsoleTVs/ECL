# Create the shared library of ECL.
add_library (ECL SHARED src/lexer.c src/parser.c src/ecl.c src/transpiler.c)
set_target_properties(ECL PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION})
set_target_properties(ECL PROPERTIES PUBLIC_HEADER include/ecl.h)

# Let's set compiler-specific flags
if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    # G++
    target_compile_options(ECL PRIVATE -Wall -Wextra)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
    # MSVC
    target_compile_options(ECL PRIVATE /EHsc /MTd /W2 /c)
    # Set the DLLEXPORT variable to export symbols
    target_compile_definitions(ECL PRIVATE WIN_EXPORT)
endif()
