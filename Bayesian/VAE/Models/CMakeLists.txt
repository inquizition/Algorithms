 set(CMAKE_C_FLAGS "-Og -g3")
add_library(ModelFunctions LinearModel.c NonLinearModel.c)

if(POLICY CMP0079)
		cmake_policy(SET CMP0079 NEW)
endif()
target_sources(ModelFunctions PRIVATE
		"${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models/LinearModel.c"
		"${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models/LinearModel.h"
		"${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models/NonLinearModel.c"
		"${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models/NonLinearModel.h")
target_include_directories(ModelFunctions PUBLIC
						"${PROJECT_BINARY_DIR}"
						"${PROJECT_SOURCE_DIR}/utils/Matrix")
