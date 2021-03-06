cmake_minimum_required(VERSION 3.0)
project(c-final-project C)

set(CMAKE_C_STANDARD 11)

add_executable(${CMAKE_PROJECT_NAME} main.c physics.c physics.h utils.c utils.h render.c render.h constants.h ui.c ui.h)

if (WIN32)
    message("Windows is not supported...")
    return()
else()
    find_package(SFML 2.5 REQUIRED system window graphics network audio)
    target_link_libraries(${CMAKE_PROJECT_NAME} csfml-graphics csfml-system csfml-window m)
endif()

# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    add_custom_command(
            TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/resource"
    )
    message("Copying resources directory to ${CMAKE_CURRENT_BINARY_DIR}/resources")
endif()