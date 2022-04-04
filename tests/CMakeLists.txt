cmake_minimum_required(VERSION 3.20)
project(stratum_tests)

set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCE_FILES stratum_tests.cpp tcp_tests.cpp)

enable_testing()

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
    stratum_lib
    gtest_main
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
