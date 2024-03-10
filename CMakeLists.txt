cmake_minimum_required(VERSION 3.10)
project(DockerClone)

set(CMAKE_CXX_STANDARD 14)

add_executable(ccrun src/dockerClone.cpp)

# Add another target
add_executable(test src/dockerClone.cpp)

# Move the test executable to ../test/alpineTest after it's built
add_custom_command(TARGET test POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ccrun> ${CMAKE_SOURCE_DIR}/test/alpineTest/
                   COMMENT "Moving test executable to ../test/alpineTest")