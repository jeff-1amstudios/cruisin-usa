cmake_minimum_required(VERSION 3.20)

option(CRUSN_MACOS_BUNDLE "Build a macOS application bundle" OFF)
if(APPLE AND CRUSN_MACOS_BUNDLE AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version")
endif()

project(cruisin_usa_port LANGUAGES C CXX)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(CRUSN_TRACE "Enable native trace logging" ON)
option(C3X_USE_HOST_FLOAT "Use native host floating point for translated C3x float operations" OFF)
option(CRUSN_ENABLE_ASAN "Enable AddressSanitizer in Debug builds" OFF)

set(CRUSN_ROM_URL "https://archive.org/download/crusnusa/crusnusa.zip" CACHE STRING "ROM download URL (empty disables automatic download)")
if(NOT EMSCRIPTEN)
  find_package(SDL2 CONFIG QUIET)
  if(NOT SDL2_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
  endif()
  find_package(CURL 7.68 REQUIRED)
  if(APPLE)
    # Only native_download.c uses curl. FindCURL can expose the SDK's entire
    # usr/include directory, which breaks libc++'s include_next header lookup
    # when explicitly added to C++ compilation.
    get_target_property(CRUSN_CURL_INCLUDE_DIRS CURL::libcurl INTERFACE_INCLUDE_DIRECTORIES)
    if(CRUSN_CURL_INCLUDE_DIRS)
      set_property(TARGET CURL::libcurl PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        "$<$<COMPILE_LANGUAGE:C>:${CRUSN_CURL_INCLUDE_DIRS}>")
    endif()
  endif()
endif()

set(CRUSN_SOURCES

  src/portable/rom.c
  src/portable/startup.c
  src/portable/startup_ui.c
  src/portable/input.c
  src/portable/output.c
  src/portable/machine.c
  src/portable/memory.c
  src/portable/romreader.c
  src/portable/trace.c
  src/portable/validator.c
  src/game/attrdrne.c
  src/game/attracta.c
  src/game/audits.c
  src/game/backgrnd.c
  src/game/bonus.c
  src/game/bssstart.c
  src/portable/c3x_float.c
  src/portable/dcs_board.cpp
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
  src/game/math_cusa.c
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
  src/platform/sdl_audio.c
  src/platform/sdl_video.c
)

add_library(miniz STATIC
  lib/miniz/miniz.c
  lib/miniz/miniz_tdef.c
  lib/miniz/miniz_tinfl.c
  lib/miniz/miniz_zip.c
)
include(GenerateExportHeader)
generate_export_header(miniz)
target_compile_definitions(miniz PUBLIC MINIZ_STATIC_DEFINE)
target_compile_definitions(miniz PRIVATE
  $<$<PLATFORM_ID:Linux>:_POSIX_C_SOURCE=200809L>
)
target_include_directories(miniz PUBLIC lib/miniz ${CMAKE_CURRENT_BINARY_DIR})

add_library(adsp2100 STATIC
  lib/adsp2100/adsp2100.cpp
)
target_include_directories(adsp2100 PUBLIC lib)
target_compile_definitions(adsp2100 PRIVATE
  LSB_FIRST
  INLINE=inline
  UINT32=uint32_t
  HAS_ADSP2101=1
  HAS_ADSP2105=1
)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(adsp2100 PRIVATE $<$<CONFIG:Debug>:-O2>)
endif()

add_executable(crusn)
target_sources(crusn PRIVATE ${CRUSN_SOURCES})
target_link_libraries(crusn PRIVATE miniz adsp2100)
target_compile_definitions(crusn PRIVATE CRUISN_PORTABLE=1 CRUSN_ROM_URL="${CRUSN_ROM_URL}")
if(EMSCRIPTEN)
  target_sources(crusn PRIVATE src/platform/web_download.c)
  target_compile_options(crusn PRIVATE -sUSE_SDL=2 -fexceptions)
  target_link_options(crusn PRIVATE
    -sUSE_SDL=2 -sFETCH=1 -sALLOW_MEMORY_GROWTH=1
    -sINITIAL_MEMORY=134217728 -sMAXIMUM_MEMORY=536870912
    -sSTACK_SIZE=8388608 -sFORCE_FILESYSTEM=1 -lidbfs.js -fexceptions
    "-sEXPORTED_RUNTIME_METHODS=['FS','callMain']"
    "--shell-file=${CMAKE_SOURCE_DIR}/src/platform/web/shell.html"
  )
  set_target_properties(crusn PROPERTIES SUFFIX ".html")
  set_property(TARGET crusn APPEND PROPERTY LINK_DEPENDS "${CMAKE_SOURCE_DIR}/src/platform/web/shell.html")
else()
  target_sources(crusn PRIVATE src/platform/native_download.c)
  target_link_libraries(crusn PRIVATE CURL::libcurl)
  add_executable(startup-probe EXCLUDE_FROM_ALL
    tools/startup_probe.c src/portable/startup.c src/portable/rom.c
    src/portable/dcs_board.cpp src/platform/native_download.c)
  target_include_directories(startup-probe PRIVATE src src/portable src/game)
  target_link_libraries(startup-probe PRIVATE miniz adsp2100 CURL::libcurl)
endif()

# Compile the same translated core for deterministic native/Wasm comparisons.
set(CRUSN_PROBE_SOURCES ${CRUSN_SOURCES})
list(FILTER CRUSN_PROBE_SOURCES EXCLUDE REGEX "^src/platform/|^src/portable/(input|startup|startup_ui)\\.c$")
add_executable(game-probe EXCLUDE_FROM_ALL tools/game_probe.c ${CRUSN_PROBE_SOURCES})
target_include_directories(game-probe PRIVATE src src/portable src/game)
target_compile_definitions(game-probe PRIVATE CRUISN_PORTABLE=1)
target_link_libraries(game-probe PRIVATE miniz adsp2100)
if(C3X_USE_HOST_FLOAT)
  target_compile_definitions(game-probe PRIVATE C3X_USE_HOST_FLOAT=1)
endif()
if(EMSCRIPTEN)
  set_target_properties(game-probe PROPERTIES SUFFIX ".js")
  target_compile_options(game-probe PRIVATE -fexceptions)
  target_link_options(game-probe PRIVATE -fexceptions -sENVIRONMENT=node -sNODERAWFS=1
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=134217728 -sSTACK_SIZE=8388608)
endif()

if(APPLE AND CRUSN_MACOS_BUNDLE)
  set_target_properties(crusn PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "Cruis'n USA"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.1amstudios.cruisn-usa"
    OUTPUT_NAME "Cruis'n USA"
  )
endif()

add_custom_target(check-game-portable-boundary
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/check_game_portable_boundary.py
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Checking game code uses only portable host interfaces"
  VERBATIM
)

add_custom_target(check-c3x-translation
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/check_c3x_translation.py
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Checking translated C3X floating-point operations"
  VERBATIM
)

add_custom_target(check-asm-source-sync
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/check_asm_source_sync.py
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Checking ordered ASM source comments"
  VERBATIM
)

target_include_directories(crusn PRIVATE src src/portable src/platform src/game)

if(NOT EMSCRIPTEN)
  if(TARGET SDL2::SDL2main)
    target_link_libraries(crusn PRIVATE SDL2::SDL2main)
  endif()
  if(TARGET SDL2::SDL2)
    target_link_libraries(crusn PRIVATE SDL2::SDL2)
  else()
    target_include_directories(crusn PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(crusn PRIVATE ${SDL2_LINK_LIBRARIES})
  endif()
endif()

if(CRUSN_TRACE)
  target_compile_definitions(crusn PRIVATE CRUSN_TRACE=1)
endif()

if(C3X_USE_HOST_FLOAT)
  target_compile_definitions(crusn PRIVATE C3X_USE_HOST_FLOAT=1)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(crusn PRIVATE
    $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
    -Werror=return-type
  )
  if(CRUSN_ENABLE_ASAN)
    foreach(target crusn game-probe)
      target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address>)
      target_link_options(${target} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address>)
    endforeach()
  endif()
endif()

if(EMSCRIPTEN)
  # Native linker-map syntax is not accepted by wasm-ld.
elseif(APPLE)
  target_link_options(crusn PRIVATE "LINKER:SHELL:-map ${CMAKE_BINARY_DIR}/port.map")
else()
  target_link_options(crusn PRIVATE "LINKER:-Map=${CMAKE_BINARY_DIR}/port.map")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  target_compile_options(crusn PRIVATE -Werror=literal-conversion)
endif()
