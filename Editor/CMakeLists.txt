# Editor application

add_executable(Editor
    src/EditorApp.cpp
)

target_link_libraries(Editor PRIVATE GameEngine)

# Set output directory
set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy assets to build directory
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Sandbox/assets
    ${CMAKE_BINARY_DIR}/bin/assets
)

