cmake_minimum_required(VERSION 3.18)
project("SHA1_tests")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

get_filename_component(PARENT_DIR ../ ABSOLUTE)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	add_definitions(-D_WIN32)
	add_compile_options("/O2")
else()
	add_definitions(-D_LINUX)
   if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		message(WARNING "Assuming system in Linux")
	endif()
	add_compile_options("-O3")
	add_compile_options("-g")
endif()

#---------------------------------------------------------------------

include(FetchContent)

FetchContent_Declare(TimeProfiler
    GIT_REPOSITORY "https://github.com/volatilflerovium/time_profiler_visualizer"
    GIT_TAG "origin/main"
)

FetchContent_MakeAvailable(TimeProfiler)

#---------------------------------------------------------------------

add_library(SHA1_lib INTERFACE)

target_include_directories(
	SHA1_lib
	INTERFACE
	"${CMAKE_SOURCE_DIR}/.."
)

#---------------------------------------------------------------------

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

	execute_process(COMMAND ls /tmp/1G_sha1_test_file.txt
		ERROR_VARIABLE error
		OUTPUT_VARIABLE output
	)
	
	if(NOT ("${error}" STREQUAL ""))
		message("Creating a 1G file of random data in /tmp directory...")
		execute_process(COMMAND dd if=/dev/urandom of=/tmp/1G_sha1_test_file.txt bs=1G count=1 iflag=fullblock
			ERROR_VARIABLE error
			OUTPUT_VARIABLE output
		)
	endif()
	add_compile_definitions(TEST_FILE="/tmp/1G_sha1_test_file.txt")
endif()

#---------------------------------------------------------------------

set(TEST "big_file_test")

add_executable(
	"${TEST}"
	big_file_test.cpp
)

target_link_libraries(
	"${TEST}"
	PRIVATE
	TimeProfiler
	SHA1_lib
)

target_compile_definitions(
	"${TEST}"
	PRIVATE
	-DENABLE_STOPWATCH
)

#---------------------------------------------------------------------
