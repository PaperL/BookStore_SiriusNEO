cmake_minimum_required(VERSION 3.17)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(BookStore_SiriusNEO main.cpp bookstore.h bookstore.cpp blocklist.h blocklist.cpp usermanager.h usermanager.cpp)