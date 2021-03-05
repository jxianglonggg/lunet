cmake_minimum_required (VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -g -std=c++11 -pthread -ldl")
project(lunet)

include_directories(lunet 3rd/lua)
AUX_SOURCE_DIRECTORY(lunet/core CORE_SRC)

# 编译lunet
add_executable(lunet main.cc ${CORE_SRC}) 

# 编译 logger.so 
include_directories(lunet/libs-src/logger)
SET(LIBRARY_OUTPUT_PATH ./libs)
AUX_SOURCE_DIRECTORY(lunet/libs-src/logger LIB_LOGGER_SRC)
ADD_LIBRARY(logger SHARED ${LIB_LOGGER_SRC})
set_target_properties(logger PROPERTIES  PREFIX "")