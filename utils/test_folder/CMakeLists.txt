add_test(MatrixTest Matrix_test.c) 

if(POLICY CMP0079)
		cmake_policy(SET CMP0079 NEW)
endif()
target_include_directories(MatrixTest PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/utils/Matrix")
