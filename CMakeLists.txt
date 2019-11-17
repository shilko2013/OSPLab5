cmake_minimum_required(VERSION 3.10)
project(lab5 C)

set(CMAKE_C_STANDARD 99)

add_executable(p1_1_server p1_1_server.c info.h server.c server.h)
add_executable(p1_1_client info.h server.c server.h p1_1_client.c client.c client.h)
add_executable(p1_2_server p1_2_server.c info.h server.c server.h)
add_executable(p1_2_client info.h server.c server.h  p1_2_client.c client.c client.h)
add_executable(p1_3_server p1_3_server.c info.h server.c server.h)
add_executable(p1_3_client info.h server.c server.h  p1_3_client.c client.c client.h)