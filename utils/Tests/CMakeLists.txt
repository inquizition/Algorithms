set(CMAKE_C_FLAGS "-Og -g3")
add_library(Matrix_test_lib Matrix_tests.c)

target_link_libraries(Matrix_test_lib PUBLIC MatrixFunctions)

target_include_directories(Matrix_test_lib PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/utils/Matrix")
