# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# 声明项目名，也就是动态库的名字
project("mmap-writer")
# 配置编译参数
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
#设置编译源码目录
aux_source_directory(src/main/cpp LIB_SOURCE)
# 添加动态库
add_library(mmap-writer SHARED ${LIB_SOURCE})
# 设置h头文件路径
include_directories(src/main/cpp/includes)
#打印目前配置
message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_RELEASE is ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_CURRENT_SOURCE_DIR is ${CMAKE_CURRENT_SOURCE_DIR}")
message("LIB_SOURCE is ${LIB_SOURCE}")
# 查找native库log，赋值为loglib
find_library(loglib log)
# 链接loglib给mmap-writer
target_link_libraries(mmap-writer ${loglib})