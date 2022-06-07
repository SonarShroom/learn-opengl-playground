cmake_policy(SET CMP0048 NEW)

project(LearnOpenGLPlayground
	VERSION 0.0.1
	LANGUAGES C CXX
)
cmake_minimum_required(VERSION 3.11)

set(LearnOpenGLPlayground_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# Dependencies
set(DEPS_DIR ${CMAKE_CURRENT_LIST_DIR}/deps)
set(GLFW_DIR ${DEPS_DIR}/glfw)
set(GLAD_DIR ${DEPS_DIR}/glad)


## GLFW
# Required as per https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_DIR})

## GLAD
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

## Main target
add_executable(${PROJECT_NAME} ${LearnOpenGLPlayground_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME}
	glad
	glfw
)