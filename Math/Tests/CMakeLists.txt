#set(CMAKE_C_FLAGS "-Og -g3")
set(CMAKE_C_FLAGS "-O0")
add_library(Math_test_lib matrix_tests.c)

target_link_libraries(Math_test_lib PRIVATE m PUBLIC MathFunctions PRIVATE cunit)

target_include_directories(Math_test_lib PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/Math")
