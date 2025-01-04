cmake_minimum_required(VERSION 3.10)
project(ThreadPoolTest)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(ThreadPoolTest main.cpp ThreadPool.cpp)

target_link_libraries(${PROJECT_NAME} pthread)  # 直接链接pthread库