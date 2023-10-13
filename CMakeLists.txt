cmake_minimum_required(VERSION 3.20)                                                                                                   
# set the project name and version
project(Algorithms VERSION 1.0)

# specify the C standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "-Og -g3")

# add subdirectories
add_subdirectory(Bayesian/VAE/Models)
add_subdirectory(Bayesian/VAE/Tests)
add_subdirectory(utils/Tests)
add_subdirectory(utils/Matrix)
add_subdirectory(utils/cunit/CUnit)
add_subdirectory(test_folder)

# add the executable
add_executable(main main.c)

# Enable testing and add tests
enable_testing()
add_test(Matrix_test 		test_folder/MatrixTest)
add_test(LinearModel_test 	test_folder/LinearModelTest)

# link libraries
target_link_libraries(main PUBLIC MatrixFunctions PUBLIC ModelFunctions m)
 
# add the binary tree to the search path for main include files
target_include_directories(main PUBLIC 
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/utils/Matrix"
	 "${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models")

# add the binary tree to the search path for test include files
target_include_directories(MatrixTest PUBLIC 
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/test_folder")

target_include_directories(LinearModelTest PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}/test_folder")
