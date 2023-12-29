cmake_minimum_required(VERSION 3.6)

option(BUILD_EXAMPLE "Build example program" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "" FORCE) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(O3GL)

file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(O3GL STATIC ${SRCS})
target_include_directories(O3GL PUBLIC "include")

if(BUILD_EXAMPLE)
    set(SDL_STATIC ON)
    set(SDL_SHARED OFF)
    add_subdirectory(vendor/SDL3)
    add_executable(O3GL-Example "example/Main.cpp")
    target_link_libraries(O3GL-Example O3GL SDL3-static)
endif()