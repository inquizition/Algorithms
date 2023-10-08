set(CMAKE_C_FLAGS "-Og -g3")
add_library(LinearModel_test_lib LinearModel_tests.c)

target_link_libraries(LinearModel_test_lib PUBLIC ModelFunctions)

target_include_directories(LinearModel_test_lib PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models")
