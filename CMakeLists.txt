cmake_minimum_required(VERSION 3.0.0...3.16.3)
project(game VERSION 0.1.0 LANGUAGES C)

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") # gcc 32bit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32") # g++ 32bit


if(DEMO STREQUAL "sn")
    message("snake demo")
    add_definitions(-DSNAKE_DEMO=1)
    add_executable(${PROJECT_NAME} ./demo/snake/snake.c)
elseif(DEMO STREQUAL "fb")
    message("fb demo")
    # add_definitions(-DTEST_DEMO=1)
    add_executable(${PROJECT_NAME} ./demo/falling_brick_game/main.c)
else()
    message("default demo")
    add_definitions(-DTEST_DEMO=1)
    add_executable(${PROJECT_NAME} ./src/test/test.c)
endif()


aux_source_directory(./src/utils UTILS_SRCS)
aux_source_directory(./src/utils TEST_SRCS)

target_sources(${PROJECT_NAME} PRIVATE ${UTILS_SRCS})
target_sources(${PROJECT_NAME} PRIVATE ${TEST_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/utils)
target_include_directories(${PROJECT_NAME} PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/test)


target_link_libraries(${PROJECT_NAME} m) # to link againt the math library (libm)

target_compile_options(${PROJECT_NAME} PRIVATE -pthread)
