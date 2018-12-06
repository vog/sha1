cmake_minimum_required(VERSION 3.9) # need v3.9 for new version of FindDoxygen

set(MBED_TARGET LPC1768)

cmake_policy(SET CMP0056 NEW)
include(cmake/mbed_gcc_arm_toolchain.cmake)

# now detect compilers
project(Hamster C CXX)

include(cmake/mbed.cmake)

# warning flags
add_compile_options(-Wall)

add_subdirectory(mbed-src) #first get MBed standard library
add_subdirectory(MODSERIAL) #then get the modserial library for buffered serial IO
add_subdirectory(sha-src) #build the SHA source code
