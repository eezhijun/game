cmake_minimum_required(VERSION 3.0.0...3.16.3)
project(game VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") # gcc 32bit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32") # g++ 32bit

add_compile_options(-pthread)

include_directories(
    demo/dungeon_rush
    src/utils
    src/test
)

file(GLOB_RECURSE SOURCES
    "demo/snake/*.*"
    "demo/dungeon_rush/*.*"
    "src/utils/*.*"
    "src/test/*.*"
)

if(DEMO STREQUAL "sn")
    message("snake demo")
    add_definitions(-DSNAKE_DEMO=1)
elseif(DEMO STREQUAL "dr")
    message("dungeon_rush demo")
    add_definitions(-DDR_DEMO=1)
else()
    message("default demo")
    add_definitions(-DTEST_DEMO=1)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} m) # to link againt the math library (libm)

# SDL2 SDL2_image SDL2_ttf SDL2_mixer
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Net SDL2::Image SDL2::Mixer SDL2::TTF)
