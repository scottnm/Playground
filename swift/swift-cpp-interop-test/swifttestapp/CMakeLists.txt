cmake_minimum_required(VERSION 3.10)

project(swifttestapp LANGUAGES Swift)
set(SOURCES main.swift)
add_executable(swifttestapp ${SOURCES})
target_include_directories(
    swifttestapp
    PRIVATE "${ROOTDIR}/cpptestlib")
target_compile_options(
    swifttestapp
    PRIVATE -cxx-interoperability-mode=default)