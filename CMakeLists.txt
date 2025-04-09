cmake_minimum_required(VERSION 3.10)

project(OpenGL VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES "src/**.cpp")
file(GLOB_RECURSE HEADERS "include/**.h")
file(GLOB_RECURSE SHADERS "shaders/*.vert" "shaders/*.frag")

add_executable(OpenGL ${SOURCES} ${HEADERS} ${SHADERS})

target_compile_definitions(OpenGL PUBLIC GL_SILENCE_DEPRECATION)
target_link_libraries(OpenGL ${OPENGL_LIBRARIES} GLEW::GLEW ${GLM_LIBRARIES} glfw)