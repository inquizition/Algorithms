#set(CMAKE_C_FLAGS "-Og -g3")
set(CMAKE_C_FLAGS "-O0")
add_library(ModelFunctions SHARED model_api.c LinearModel.c model.c NonLinearModel.c )

if(POLICY CMP0079)
	cmake_policy(SET CMP0079 NEW)
endif()

target_link_libraries(ModelFunctions PUBLIC MatrixFunctions)

target_include_directories(ModelFunctions PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/utils/Matrix")
