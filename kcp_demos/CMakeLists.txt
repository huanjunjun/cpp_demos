cmake_minimum_required(VERSION 3.10)
project(kcp_wrapper)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置默认构建类型为 Debug

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 添加头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party/kcp
)

# 添加KCP库
add_library(kcp STATIC
    ${PROJECT_SOURCE_DIR}/third_party/kcp/ikcp.c
)

# 添加KCP包装库
add_library(kcp_wrapper STATIC
    ${PROJECT_SOURCE_DIR}/src/kcp_wrapper.cpp
)
target_link_libraries(kcp_wrapper PRIVATE kcp)

# 添加可执行文件
add_executable(sender ${PROJECT_SOURCE_DIR}/src/sender.cpp)
add_executable(receiver ${PROJECT_SOURCE_DIR}/src/receiver.cpp)

# 链接库
target_link_libraries(sender PRIVATE kcp_wrapper pthread)
target_link_libraries(receiver PRIVATE kcp_wrapper pthread)

# 安装规则
install(TARGETS sender receiver
    RUNTIME DESTINATION bin
)
