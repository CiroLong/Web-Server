cmake_minimum_required(VERSION 3.10)

project(WEBSERVER)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generate the compile_commands.json
set(CMAKE_CXX_STANDARD 17) # we need to use cxx 17 features
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WEBSERVER_BUILD_TEST "Build WEBSERVER test (currently not enabled)" OFF) # the option variable for enable the test

# set include path
set(WEBSERVER_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)

# include path at the root level
include_directories(${WEBSERVER_SRC_INCLUDE_DIR})


# add subdirectories
add_subdirectory(src)
#add_subdirectory(test)
#add_subdirectory(third_party)

# enable google test
# enable_testing()
