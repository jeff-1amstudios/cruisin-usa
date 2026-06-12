cmake_minimum_required(VERSION 3.20)

project(cruisin_usa_port C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(CRUSN_TRACE "Enable native trace logging" ON)

find_package(SDL2 CONFIG QUIET)
if(NOT SDL2_FOUND)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

file(GLOB_RECURSE CRUSN_SOURCES CONFIGURE_DEPENDS
  src/*.c
)

add_executable(crusn ${CRUSN_SOURCES})

target_include_directories(crusn PRIVATE src src/core src/platform src/game)

if(TARGET SDL2::SDL2)
  target_link_libraries(crusn PRIVATE SDL2::SDL2)
else()
  target_include_directories(crusn PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(crusn PRIVATE ${SDL2_LINK_LIBRARIES})
endif()

if(CRUSN_TRACE)
  target_compile_definitions(crusn PRIVATE CRUSN_TRACE=1)
endif()
