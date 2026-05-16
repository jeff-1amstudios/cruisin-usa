cmake_minimum_required(VERSION 3.16)
project(cusa C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

find_program(SDL2_CONFIG_EXECUTABLE sdl2-config REQUIRED)
execute_process(
    COMMAND ${SDL2_CONFIG_EXECUTABLE} --cflags
    OUTPUT_VARIABLE SDL2_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${SDL2_CONFIG_EXECUTABLE} --libs
    OUTPUT_VARIABLE SDL2_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

separate_arguments(SDL2_CFLAGS_LIST NATIVE_COMMAND ${SDL2_CFLAGS})
separate_arguments(SDL2_LIBS_LIST NATIVE_COMMAND ${SDL2_LIBS})

# Include all module translation units in src/ (top-level only).
# We intentionally exclude src/scaffold/** to avoid duplicate placeholder symbols.
file(GLOB MODULE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(cusa ${MODULE_SOURCES})

target_compile_options(cusa PRIVATE ${SDL2_CFLAGS_LIST})
target_link_libraries(cusa PRIVATE ${SDL2_LIBS_LIST})

target_compile_definitions(cusa PRIVATE
    SCREEN_WIDTH=512
    SCREEN_HEIGHT=400
)
