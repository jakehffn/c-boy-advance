target_sources(test_cpu PUBLIC
    ARM7TDMI_instruction.test.cc
    ARM7TDMI_instruction.c
)

target_include_directories(test_cpu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)