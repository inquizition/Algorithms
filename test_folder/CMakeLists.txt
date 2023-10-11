add_executable(MatrixTest Matrix_test_suite.c)
add_executable(LinearModelTest LinearModel_test_suite.c)
add_executable(NonLinearModelTest NonLinearModel_test_suite.c)

target_link_libraries(MatrixTest PUBLIC MatrixFunctions PUBLIC Matrix_test_lib PRIVATE cunit m)
target_link_libraries(LinearModelTest PUBLIC ModelFunctions PUBLIC LinearModel_test_lib PRIVATE cunit m)
target_link_libraries(NonLinearModelTest PUBLIC ModelFunctions PUBLIC NonLinearModel_test_lib PRIVATE cunit m)

target_include_directories(MatrixTest PUBLIC 
            "${PROJECT_BINARY_DIR}"
            "${PROJECT_SOURCE_DIR}/utils/Matrix"
            "${PROJECT_SOURCE_DIR}/utils/Tests"
            "${PROJECT_SOURCE_DIR}/utils/cunit/CUnit")

target_include_directories(LinearModelTest PUBLIC 
            "${PROJECT_BINARY_DIR}"
	    "${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models"
	    "${PROJECT_SOURCE_DIR}/Bayesian/VAE/Tests"
            "${PROJECT_SOURCE_DIR}/utils/cunit/CUnit")
         
target_include_directories(NonLinearModelTest PUBLIC 
            "${PROJECT_BINARY_DIR}"
	    "${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models"
	    "${PROJECT_SOURCE_DIR}/Bayesian/VAE/Tests"
            "${PROJECT_SOURCE_DIR}/utils/cunit/CUnit")
