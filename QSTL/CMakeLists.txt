CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE_DIRECTORIES(./Base ./Base/Profiler ./Containers ./Test)

SET(CMAKE_SOURCE_DIR .)

#AUX_SOURCE_DIRECTORY(./Test SRC_LIST)
SET(SRC_LIST ./Test/main.cpp ./Test/VectorTest.cpp)
AUX_SOURCE_DIRECTORY(./Base/Profiler SRC_LIST)
AUX_SOURCE_DIRECTORY(./Base SRC_LIST)
AUX_SOURCE_DIRECTORY(./Containers SRC_LIST)

message(STATUS "SRC_LIST: " ${SRC_LIST})

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    message(STATUS "optional: -std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

ADD_EXECUTABLE(main ${SRC_LIST})