cmake_minimum_required(VERSION 3.23)

project(CBoyAdvanced)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME})
add_executable(cpu_test)

add_subdirectory(src)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/)

# When copmiling for x86, include GLEW and SDL2
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")

    find_package(GLEW REQUIRED)
    find_package(SDL2 REQUIRED)
    
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARY} GLEW::glew)
    
endif()

# OpenGL is added for all platforms
find_package(OpenGL REQUIRED)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sUSE_SDL=2")

else ()

    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

endif ()

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC -mconsole ${OPENGL_LIBRARY})