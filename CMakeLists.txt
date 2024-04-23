cmake_minimum_required(VERSION 3.27)
project(OpenGL)
set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.4 REQUIRED)
add_library(GLAD ${PROJECT_SOURCE_DIR}/include/glad/src/glad.c)
target_include_directories(GLAD PUBLIC ${PROJECT_SOURCE_DIR}/include/glad/include)
set_target_properties(GLAD PROPERTIES LINKER_LANGUAGE C)
include_directories(
        ${glfw3_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(OpenGL src/Main.cpp
        src/OpenGLContext.cpp
        include/OpenGLContext.hpp)
target_link_libraries(OpenGL glfw GLAD)