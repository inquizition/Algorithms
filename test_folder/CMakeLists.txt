add_executable(MatrixTest Matrix_test.c)

target_link_libraries(MatrixTest PUBLIC MatrixFunctions PRIVATE cunit m)

target_include_directories(MatrixTest PUBLIC 
            "${PROJECT_BINARY_DIR}"
            "${PROJECT_SOURCE_DIR}/utils/Matrix"
            "${PROJECT_SOURCE_DIR}/utils/cunit/CUnit")
