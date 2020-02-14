CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY(googletest)

SET(CMAKE_CXX_STANDARD 11)

# Used for compilation with the -g flag; needed for useful gdb
SET(DEBUG_FLAG "-g")
add_definitions(${DEBUG_FLAG})

ADD_EXECUTABLE(test
	unit_tests/test.cpp
)

ADD_EXECUTABLE(rshell
    src/rshell.cpp
)

TARGET_LINK_LIBRARIES(test gtest)
TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
