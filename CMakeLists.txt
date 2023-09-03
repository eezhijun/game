cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0 LANGUAGES C)

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") # gcc 32bit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32") # g++ 32bit


if(DEFINED SN)
    message("Macro MY_MACRO_NAME is defined.")
    add_definitions(-DSNAKE_DEMO=1)
    add_executable(main ./demo/snake/snake.c)
else()
    message("Macro MY_MACRO_NAME is not defined.")
    add_definitions(-DTEST_DEMO=1)
    add_executable(main ./src/test/test.c)
endif()

# add_executable(main ./demo/snake/snake.c)

aux_source_directory(./src/utils UTILS_SRCS)
aux_source_directory(./src/lib/cjson LIB_CJSON_SRCS)
aux_source_directory(./src/lib/log LIB_LOG_SRCS)
aux_source_directory(./src/lib/queue LIB_QUEUE_SRCS)
aux_source_directory(./src/lib/ringbuffer LIB_RINGBUFFER_SRCS)
aux_source_directory(./src/lib/stack LIB_STACK_SRCS)
aux_source_directory(./src/lib/tiny_crypto LIB_TINY_CYPTO_SRCS)
aux_source_directory(./src/lib/tlsf LIB_TLSF_SRCS)
aux_source_directory(./src/utils TEST_SRCS)

target_sources(main PRIVATE ${UTILS_SRCS})
target_sources(main PRIVATE ${LIB_CJSON_SRCS})
target_sources(main PRIVATE ${LIB_LOG_SRCS})
target_sources(main PRIVATE ${LIB_QUEUE_SRCS})
target_sources(main PRIVATE ${LIB_RINGBUFFER_SRCS})
target_sources(main PRIVATE ${LIB_STACK_SRCS})
target_sources(main PRIVATE ${LIB_TINY_CYPTO_SRCS})
target_sources(main PRIVATE ${LIB_TLSF_SRCS})
target_sources(main PRIVATE ${TEST_SRCS})

target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/utils)
target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/lib/cjson)
target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/lib/log)
target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/lib/queue)
target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/lib/ringbuffer)
target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/lib/stack)
target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/lib/tiny_crypto)
target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/lib/tlsf)
target_include_directories(main PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/src/test)

target_link_libraries(main m) # to link againt the math library (libm)










