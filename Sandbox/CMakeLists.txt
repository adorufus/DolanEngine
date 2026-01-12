# Sandbox application

add_executable(Sandbox
    src/main.cpp
)

target_link_libraries(Sandbox PRIVATE
    GameEngine
)

# Copy assets to build directory
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

