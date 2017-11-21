cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

project(test_project)

find_package(aws-sdk-cpp)

add_definitions(-DUSE_IMPORT_EXPORT)

add_executable(master src/main.cpp src/mode1.cpp src/mode2.cpp src/ec2.cpp src/utility.cpp)

target_link_libraries(master aws-cpp-sdk-core aws-cpp-sdk-ec2 pthread)

# also issue: export CMAKE_PREFIX_PATH=/path/to/sdk_build_dir ... if not already present in .bashrc or exported for the current session
