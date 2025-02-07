add_executable(MatrixTest Matrix_test_suite.c)
add_executable(MathTest Math_test_suite.c)
add_executable(LinearModelTest LinearModel_test_suite.c)
add_executable(NonLinearModelTest NonLinearModel_test_suite.c)
add_executable(TrainedModelTest TrainedModel_test_suite.c)

target_link_libraries(MatrixTest PUBLIC MatrixFunctions PUBLIC Matrix_test_lib PRIVATE cunit)
target_link_libraries(MathTest PUBLIC MathFunctions PUBLIC Math_test_lib PRIVATE cunit)
target_link_libraries(LinearModelTest PUBLIC ModelFunctions PUBLIC LinearModel_test_lib PRIVATE cunit)
target_link_libraries(NonLinearModelTest PUBLIC ModelFunctions PUBLIC NonLinearModel_test_lib PRIVATE cunit)
target_link_libraries(TrainedModelTest PUBLIC ModelFunctions PUBLIC TrainedModel_test_lib PRIVATE cunit)

target_include_directories(MatrixTest PUBLIC 
            "${PROJECT_BINARY_DIR}"
            "${PROJECT_SOURCE_DIR}/utils/Matrix"
            "${PROJECT_SOURCE_DIR}/utils/Tests"
            "${PROJECT_SOURCE_DIR}/utils/cunit/CUnit")

target_include_directories(MathTest PUBLIC 
            "${PROJECT_BINARY_DIR}"
            "${PROJECT_SOURCE_DIR}/Math"
            "${PROJECT_SOURCE_DIR}/Math/Tests"
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

target_include_directories(TrainedModelTest PUBLIC 
            "${PROJECT_BINARY_DIR}"
	    "${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models"
	    "${PROJECT_SOURCE_DIR}/Bayesian/VAE/Tests"
            "${PROJECT_SOURCE_DIR}/utils/cunit/CUnit")
