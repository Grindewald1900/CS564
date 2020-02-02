cmake_minimum_required(VERSION 3.15)
project(sshell C)

set(CMAKE_C_STANDARD 99)

add_executable(sshell main.c cmake-build-debug/Headers/tools.h)