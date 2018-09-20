cmake_minimum_required(VERSION 2.8.12)
project(NotificationDemo CXX)

add_subdirectory(kaa)
add_subdirectory(convenience)
add_executable(kaa-app NotificationDemo_with_actual_sampling.cpp )

target_link_libraries(kaa-app kaacpp rtlsdr)
