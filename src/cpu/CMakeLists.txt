target_sources(cpu_test PUBLIC
    cpu.c
)

target_include_directories(cpu_test PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)