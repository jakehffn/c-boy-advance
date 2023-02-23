cmake_minimum_required(VERSION 3.23)

project(AGE)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME})

add_subdirectory(src)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/)

# When copmiling for x86, include GLEW and SDL2
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")

    set(GLEW_VERBOSE true)
    set(GLEW_ROOT "C:/mingw_dev_lib/glew-2.1.0")
    find_package(GLEW REQUIRED)
    message(STATUS GLEW_LIBRARIES=${GLEW_LIBRARIES})
    
    set(SDL2_PATH "C:/mingw_dev_lib/SDL2")
    find_package(SDL2 REQUIRED)
    
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARY} GLEW::glew)
    
endif()

# OpenGL is added for all platforms
find_package(OpenGL REQUIRED)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sUSE_SDL=2")
    # set(USE_FLAGS "-o build/web/debug/age.js -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sUSE_SDL=2")
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sUSE_SDL=2")
    # set(SDL2_LIBRARIES "-sUSE_SDL=2")
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    # set(CMAKE_EXECUTABLE_SUFFIX .wasm)
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC -mconsole ${OPENGL_LIBRARY})