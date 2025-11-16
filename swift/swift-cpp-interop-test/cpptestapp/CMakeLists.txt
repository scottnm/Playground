set(SOURCES main.cpp)
add_executable(cpptestapp ${SOURCES})
target_include_directories(cpptestapp "${ROOTDIR}/cpptestlib")