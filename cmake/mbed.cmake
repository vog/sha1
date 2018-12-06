# CMake library for MBed programs
# (from here: https://os.mbed.com/cookbook/mbed-cmake)
set(MBED_PATH "" CACHE PATH "Path to mbed virtual device to upload to")

#custom function to add a mbed executable and generate mbed source files
function(add_mbed_executable EXECUTABLE)
	
	add_executable(${EXECUTABLE} ${ARGN})
	
	set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.bin)
	set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.map)
	
	# add link options to generate memory map
	target_link_libraries(${EXECUTABLE} -Wl,\"-Map=${MAP_FILE}\",--cref)

	# generate .bin firmware file
	add_custom_command(
		TARGET ${EXECUTABLE} POST_BUILD
		COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:${EXECUTABLE}> ${BIN_FILE}
		COMMAND python ${CMAKE_SOURCE_DIR}/cmake/memap/memap.py -t GCC_ARM ${MAP_FILE}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Objcopying ${EXECUTABLE} to mbed firmware ${BIN_FILE}")
		
	set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${BIN_FILE} ${EXECUTABLE}.map)
	
	# add upload target
	add_custom_target(flash-${EXECUTABLE}
		COMMAND ${CMAKE_COMMAND}
			-DBIN_FILE=${BIN_FILE}
			-DMBED_PATH=${MBED_PATH}
		 	-P ${CMAKE_SOURCE_DIR}/cmake/install_bin_file.cmake)
	add_dependencies(flash-${EXECUTABLE} ${EXECUTABLE})

endfunction(add_mbed_executable)
 
#assume we're using an LPC1768 model if it's not specified by -DMBED_TARGET= 
 if( NOT MBED_TARGET  MATCHES "LPC1768" AND NOT MBED_TARGET MATCHES "LPC2368" AND NOT MBED_TARGET MATCHES "LPC11U24")
   message(STATUS "invalid or no mbed target specified. Options are LPC1768, LPC2368 or LPC11U24. Assuming LPC1768 for now.
            Target may be specified using -DMBED_TARGET=")
   set(MBED_TARGET "LPC1768")
endif( NOT MBED_TARGET  MATCHES "LPC1768" AND NOT MBED_TARGET MATCHES "LPC2368" AND NOT MBED_TARGET MATCHES "LPC11U24")
 
 
#force min size build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)
 