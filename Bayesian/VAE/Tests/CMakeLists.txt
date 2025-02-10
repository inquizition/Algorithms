#set(CMAKE_C_FLAGS "-Og -g3")
set(CMAKE_C_FLAGS "-O0")
add_library(LinearModel_test_lib LinearModel_tests.c)
add_library(NonLinearModel_test_lib NonLinearModel_tests.c)
add_library(TrainedModel_test_lib TrainedModel_tests.c)

target_link_libraries(LinearModel_test_lib PUBLIC ModelFunctions PRIVATE cunit m)

target_link_libraries(NonLinearModel_test_lib PUBLIC ModelFunctions PRIVATE cunit m)

target_link_libraries(TrainedModel_test_lib PUBLIC ModelFunctions PRIVATE cunit m)

target_include_directories(LinearModel_test_lib PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models")

target_include_directories(NonLinearModel_test_lib PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models")

target_include_directories(TrainedModel_test_lib PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models")
