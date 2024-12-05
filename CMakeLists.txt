# CMake最低版本要求
cmake_minimum_required(VERSION 3.0)

# 项目名称
project(sqlpool)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

# 源文件列表
set(SOURCE_FILES
    ./src/conn.cpp
    ./src/conn.h
    ./src/connpool.cpp
    ./src/connpool.h
    ./src/log.h
    ./main.cpp
)

# 定义可执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加可执行文件
add_executable(sqlpool ${SOURCE_FILES})

# 链接库
target_link_libraries(sqlpool mysqlclient)