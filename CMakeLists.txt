cmake_minimum_required(VERSION 3.10)

# set the project name
project(IMService)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告

# add the executable
aux_source_directory(. src_list)
add_executable(IMService ${src_list})
target_link_libraries(IMService pthread)
target_link_libraries(IMService jsoncpp)
target_link_libraries(IMService mysqlcppconn)
