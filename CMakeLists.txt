cmake_minimum_required(VERSION 3.0.0...3.16.3)
project(game VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") # gcc 32bit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32") # g++ 32bit

add_compile_options(-pthread)

include_directories(
    src/utils
    src/test
)

file(GLOB_RECURSE SOURCES
    "src/utils/*.*"
    "src/test/*.*"
    "demo/snake/*.*"
)

if(DEMO STREQUAL "sn")
    message("snake demo")
    add_definitions(-DSNAKE_DEMO=1)
elseif(DEMO STREQUAL "fb")
    message("fb demo")
    # add_definitions(-DTEST_DEMO=1)
else()
    message("default demo")
    add_definitions(-DTEST_DEMO=1)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} m) # to link againt the math library (libm)
