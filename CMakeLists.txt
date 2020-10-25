cmake_minimum_required(VERSION 3.0)
project(c-final-project C)

set(CMAKE_C_STANDARD 11)

add_executable(${CMAKE_PROJECT_NAME} main.c physics.c physics.h utils.c utils.h render.c render.h constants.h)

if (WIN32)
        include_directories(${SFML_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/include")
        add_library(Lib STATIC IMPORTED)
        set_property(TARGET Lib PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/csfml-graphics-2.dll")
        set_property(TARGET Lib PROPERTY IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/lib/gcc/csfml-graphics.a")
        target_link_libraries(${CMAKE_PROJECT_NAME} Lib m)
else()
    find_package(SFML 2.5 REQUIRED system window graphics network audio)
    target_link_libraries(${CMAKE_PROJECT_NAME} csfml-graphics csfml-system m)
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