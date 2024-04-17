cmake_minimum_required(VERSION 3.0.0)
project(Test VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(Test src/main.cpp)

target_link_libraries(Test 
            ${OPENGL_LIBRARIES}
            ${GLUT_LIBRARY})