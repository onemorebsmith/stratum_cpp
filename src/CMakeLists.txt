cmake_minimum_required(VERSION 3.20)
project(stratum_lib)

set(CMAKE_BUILD_TYPE Debug)

add_library (${PROJECT_NAME} socket.cpp stratum_client.cpp stratum_protocol.hpp stratum_request.cpp stratum_response.cpp job_queue.hpp)

target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

