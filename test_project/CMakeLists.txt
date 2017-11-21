cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

project(test_project)

find_package(aws-sdk-cpp)

add_definitions(-DUSE_IMPORT_EXPORT)

add_executable(a.out main.cpp)

target_link_libraries(a.out aws-cpp-sdk-core aws-cpp-sdk-ec2)

# also issue: export CMAKE_PREFIX_PATH=/path/to/sdk_build_dir
# also remove the -Wall -Werror flags from build_dir/CMakeFiles/a.out.dir/flags.make if that is want you want
