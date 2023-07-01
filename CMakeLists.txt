cmake_minimum_required(VERSION 3.23)

project(CBoyAdvance)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_cpu)
target_link_libraries(
    test_cpu
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_cpu)

add_executable(${PROJECT_NAME})

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