cmake_minimum_required(VERSION 3.10)

project(hello-world-swift LANGUAGES Swift)
set(SOURCES hello-world.swift)
add_executable(hello-world-swift ${SOURCES})