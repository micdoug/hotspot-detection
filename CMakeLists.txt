cmake_minimum_required(VERSION 3.0.0)
project(hotspot-detection VERSION 0.0.0)

find_package(Boost 1.32 REQUIRED COMPONENTS program_options)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(Boost_USE_STATIC_LIBS TRUE)

add_executable(hotspot-detection main.cpp grid.cpp movement.cpp)

target_link_libraries(hotspot-detection ${Boost_LIBRARIES})

install(TARGETS hotspot-detection DESTINATION bin)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
