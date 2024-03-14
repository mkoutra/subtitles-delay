project(subtitle_delay)

# Add source files
set(SOURCE_FILES
    sub_delay.c
    main.c
)

# Create executable
add_executable(subtitle_delay ${SOURCE_FILES})

# Include header files directory
target_include_directories(subtitle_delay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set the output directory for the executable to 
# be the directory outside the build directory
set_target_properties(subtitle_delay PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
