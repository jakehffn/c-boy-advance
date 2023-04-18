target_sources(${PROJECT_NAME} PUBLIC
    main.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    video
)

add_subdirectory(cpu)
add_subdirectory(video)