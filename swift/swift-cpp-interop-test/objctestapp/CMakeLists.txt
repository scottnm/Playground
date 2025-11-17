cmake_minimum_required(VERSION 3.10)

set(SOURCES main.mm)
add_executable(objctestapp ${SOURCES})
target_include_directories(
    objctestapp 
    PRIVATE "${ROOTDIR}/cpptestlib")
target_link_libraries(
    objctestapp 
    PRIVATE cpptestlib)
target_link_options(
    objctestapp 
    PRIVATE -framework Cocoa
    )