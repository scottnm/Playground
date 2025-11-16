cmake_minimum_required(VERSION 3.10)

set(SOURCES main.cpp)
add_executable(cpptestapp ${SOURCES})
target_include_directories(
    cpptestapp 
    PRIVATE "${ROOTDIR}/cpptestlib")
target_link_libraries(
    cpptestapp 
    PRIVATE cpptestlib)