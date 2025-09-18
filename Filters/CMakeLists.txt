add_library(wiener_filter SHARED wiener_filter.c)

target_include_directories(wiener_filter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(wiener_filter PUBLIC m)

set_target_properties(wiener_filter PROPERTIES
    OUTPUT_NAME wiener_filter
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
)
