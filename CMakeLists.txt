cmake_minimum_required(VERSION 2.6)
project(debugger)

include_directories(vendor)


add_library(linenoise vendor/linenoise/linenoise.c)

add_library(debugger src/debugger.c)
add_library(converter src/convert.c)
target_link_libraries(debugger converter)

add_executable(sdebugger src/main.c)
target_link_libraries(sdebugger debugger linenoise)




