cmake_minimum_required(VERSION 3.20)

project(cruisin_usa_port C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(CRUSN_TRACE "Enable native trace logging" ON)

find_package(SDL2 CONFIG QUIET)
if(NOT SDL2_FOUND)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

set(CRUSN_SOURCES
  src/core/cpu.c
  src/core/config.c
  src/core/machine.c
  src/core/memory.c
  src/core/trace.c
  src/core/validator.c
  src/game/attrdrne.c
  src/game/attracta.c
  src/game/audits.c
  src/game/backgrnd.c
  src/game/bonus.c
  src/game/bssstart.c
  src/game/chopper.c
  src/game/cksum.c
  src/game/cmos.c
  src/game/cointab.c
  src/game/colla.c
  src/game/coin.c
  src/game/commdrne.c
  src/game/commq.c
  src/game/comm.c
  src/game/comp.c
  src/game/cornobj.c
  src/game/cusa.c
  src/game/date.c
  src/game/delta.c
  src/game/diag.c
  src/game/dirq.c
  src/game/drones.c
  src/game/effects.c
  src/game/error.c
  src/game/flame.c
  src/game/font1a.c
  src/game/font.c
  src/game/galil.c
  src/game/heads.c
  src/game/hottub.c
  src/game/hpmath.c
  src/game/hstdp.c
  src/game/hud.c
  src/game/infin.c
  src/game/intro.c
  src/game/invtab30.c
  src/game/leg.c
  src/game/line.c
  src/game/map.c
  src/game/math.c
  src/game/memtest.c
  src/game/motion.c
  src/game/mproc.c
  src/game/obj.c
  src/game/objects.c
  src/game/overlay.c
  src/game/pall.c
  src/game/plyr.c
  src/game/racer.c
  src/game/rho.c
  src/game/routs.c
  src/game/rroad.c
  src/game/roadblck.c
  src/game/roadkill.c
  src/game/setups.c
  src/game/sigma.c
  src/game/smoke.c
  src/game/sndtaba.c
  src/game/snd.c
  src/game/texttab.c
  src/game/text.c
  src/game/totala.c
  src/game/tracksel.c
  src/game/trans.c
  src/game/util.c
  src/game/wave.c
  src/game/wheel.c
  src/platform/sdl_main.c
  src/platform/sdl_video.c
)

add_executable(crusn)
target_sources(crusn PRIVATE ${CRUSN_SOURCES})

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

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(crusn PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
    $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
  )
  target_link_options(crusn PRIVATE $<$<CONFIG:Debug>:-fsanitize=address>)
endif()

if(APPLE)
  target_link_options(crusn PRIVATE "LINKER:SHELL:-map ${CMAKE_BINARY_DIR}/port.map")
else()
  target_link_options(crusn PRIVATE "LINKER:-Map=${CMAKE_BINARY_DIR}/port.map")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  target_compile_options(crusn PRIVATE -Werror=literal-conversion)
endif()
