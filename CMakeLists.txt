cmake_minimum_required(VERSION 3.20)                                                                                                   
                            
 # set the project name and version
 project(Algorithms VERSION 1.0)
  
 # configure a header file to pass some of the CMake settings
 # to the source code       
  
 # TODO 8: Use list() and APPEND to create a list of optional libraries
 # called  EXTRA_LIBS and a list of optional include directories called
 # EXTRA_INCLUDES. Add the MathFunctions library and source directory to
 # the appropriate lists.   
  
 # Only call add_subdirectory and only add MathFunctions specific values
 # to EXTRA_LIBS and EXTRA_INCLUDES if USE_MYMATH is true.
  
 # TODO 2: Use add_subdirectory() to add MathFunctions to this project
 add_subdirectory(Bayesian/VAE/Models)
 add_subdirectory(utils/Matrix)
 # add the executable
 add_executable(main main.c)

 # TESTS
 add_executable(MatrixTest utils/test_folder/Matrix_test.c)
 target_link_libraries(MatrixTest MatrixFunctions)
 add_test(Matrix_test2 MatrixTest) 
 
 
 # TODO 9: Use EXTRA_LIBS instead of the MathFunctions specific values
 # in target_link_libraries.
  
 # TODO 3: Use target_link_libraries to link the library to our executable
 target_link_libraries(LinearModelFunctions PUBLIC MatrixFunctions)
 target_link_libraries(main PUBLIC MatrixFunctions PUBLIC LinearModelFunctions)
  
 # TODO 4: Add MathFunctions to Tutorial's target_include_directories()
 # Hint: ${PROJECT_SOURCE_DIR} is a path to the project source. AKA This folder!
 target_include_directories(main PUBLIC 
         "${PROJECT_BINARY_DIR}"
         "${PROJECT_SOURCE_DIR}/utils/Matrix"
 		 "${PROJECT_SOURCE_DIR}/Bayesian/VAE/Models")
  
 # TODO 10: Use EXTRA_INCLUDES instead of the MathFunctions specific values
 # in target_include_directories.
  
 # add the binary tree to the search path for include files
 # so that we will find TutorialConfig.h
 
