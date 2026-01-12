# Third-party libraries

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# GLAD
add_library(glad STATIC
    glad/src/glad.c
)
target_include_directories(glad PUBLIC
    glad/include
)

# GLM (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

# stb_image (header-only, we'll compile in Engine)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb)

# spdlog (header-only mode)
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE spdlog/include)

# EnTT (header-only ECS)
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE entt/src)

# yaml-cpp
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
add_subdirectory(yaml-cpp)

# Box2D
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(box2d)

