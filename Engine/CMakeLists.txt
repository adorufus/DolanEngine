# GameEngine library

# Collect all source files
file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
    "src/*.c"
)

file(GLOB_RECURSE ENGINE_HEADERS
    "include/*.h"
    "include/*.hpp"
)

# Create static library
add_library(GameEngine STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

# Find Lua
find_package(Lua 5.4 REQUIRED)

# Include directories
target_include_directories(GameEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LUA_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Vendor/sol2/include
)

# Link dependencies
target_link_libraries(GameEngine PUBLIC
    glfw
    glad
    glm
    stb_image
    spdlog
    entt
    yaml-cpp
    box2d
    imgui
    ${LUA_LIBRARIES}
)

# Platform-specific libraries
if(WIN32)
    target_link_libraries(GameEngine PUBLIC opengl32)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    find_library(OPENAL_LIBRARY OpenAL REQUIRED)
    target_link_libraries(GameEngine PUBLIC
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
        ${OPENGL_LIBRARY}
        ${OPENAL_LIBRARY}
    )
else()
    target_link_libraries(GameEngine PUBLIC GL)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(GameEngine PRIVATE /W4)
else()
    target_compile_options(GameEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

