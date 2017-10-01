# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project(safeCanary)

# ADD_LIBRARY(instCanary SHARED libinstCanary.cpp)

# 指定生成目标
add_executable(safeCanary safeCanary.cpp)
target_link_libraries (safeCanary /usr/lib/libdyninstAPI.so)
target_link_libraries (safeCanary /usr/lib/libpatchAPI.so)
target_link_libraries (safeCanary /usr/lib/libinstructionAPI.so)
target_link_libraries (safeCanary /usr/lib/libdynC_API.so)
target_link_libraries (safeCanary /usr/lib/libcommon.so)
target_link_libraries (safeCanary /usr/lib/libparseAPI.so)
target_link_libraries (safeCanary /usr/lib/libsymtabAPI.so)
target_link_libraries (safeCanary /usr/lib/libdynDwarf.so)

target_link_libraries (safeCanary /usr/lib/libstackwalk.so)


# demo
add_executable(demo demo.cpp)


if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)
