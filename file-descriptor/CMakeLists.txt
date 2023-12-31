cmake_minimum_required(VERSION "3.22.1")

project("fd")
set(CMAKE_C_STANDARD 11 )
add_executable("fd_decoder" "fd_decoder.c")
add_executable("no_relation" "no_relation.c")
add_executable("fork_check" "fork_check.c")
add_executable("sleep" "sleep.c")
add_executable("fd_dup" "fd_dup.c")
