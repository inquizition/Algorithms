add_library(LinearModelFunctions LinearModel.c)

if(POLICY CMP0079)
		cmake_policy(SET CMP0079 NEW)
endif()
target_include_directories(LinearModelFunctions PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/utils/Matrix")
