cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0076 NEW)

project(LearnOpenGLPlayground
	VERSION 0.0.8
	LANGUAGES C CXX
)
cmake_minimum_required(VERSION 3.11)

set(LearnOpenGLPlayground_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# Dependencies
set(DEPS_DIR ${CMAKE_CURRENT_LIST_DIR}/deps)
set(GLFW_DIR ${DEPS_DIR}/glfw)
set(GLAD_DIR ${DEPS_DIR}/glad)
set(STB_DIR ${DEPS_DIR}/stb)
set(GLM_DIR ${DEPS_DIR}/glm)

## GLFW
# Required as per https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_DIR})

## GLAD
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

## STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE
	${STB_DIR}
)

## GLM
add_subdirectory(${GLM_DIR})

## Main target
add_executable(${PROJECT_NAME}
	${LearnOpenGLPlayground_SOURCE_DIR}/main.cpp
	${LearnOpenGLPlayground_SOURCE_DIR}/graphics/private/Shader.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC ${LearnOpenGLPlayground_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}
	glad
	glfw
	stb
	glm
)

install(
	FILES
	shaders/vertShader.vert
	shaders/fragShader.frag
	DESTINATION
	${CMAKE_CURRENT_BINARY_DIR}/shaders
)

install(
	FILES
	images/container.jpg
	images/awesomeface.png
	DESTINATION
	${CMAKE_CURRENT_BINARY_DIR}/images
)