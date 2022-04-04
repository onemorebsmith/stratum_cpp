cmake_minimum_required(VERSION 3.20)
project(statum_test_miner)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_FILES main.cpp test_miner.cpp)                                                                                                                                                                                              
find_package(Threads REQUIRED)


ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
    stratum_lib
    nlohmann_json::nlohmann_json
    Threads::Threads
)

if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endif()