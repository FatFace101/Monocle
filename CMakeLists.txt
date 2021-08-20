
cmake_minimum_required(VERSION 3.1)
project(Monocle)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" CACHE PATH "" FORCE)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin" CACHE PATH "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLM_STATIC_LIBRARY_ENABLE ON CACHE BOOL "" FORCE)


link_directories("${PROJECT_BINARY_DIR}/lib")

set(ALL_INCLUDES "${PROJECT_SOURCE_DIR}/glm-0.9.8" "${PROJECT_SOURCE_DIR}/glfw-3.3.2/include" "${PROJECT_SOURCE_DIR}/glad/include" "${PROJECT_SOURCE_DIR}/include")

add_subdirectory("glm-0.9.8")
add_subdirectory("glad")
add_subdirectory("glfw-3.3.2")
add_subdirectory("lib")
add_subdirectory("projects")

