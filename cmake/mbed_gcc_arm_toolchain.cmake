# CMake toolchain file to set compiler for the gcc-arm toolchain
# (from here: https://os.mbed.com/cookbook/mbed-cmake)
 
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_EXECUTABLE_SUFFIX elf)
 
# specify the cross compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
 
set(COMMON_FLAGS "-mcpu=cortex-m3 -mthumb -mthumb-interwork -msoft-float -ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -fomit-frame-pointer")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections --specs=nosys.specs -lc -lstdc++ -lsupc++ -lm -lc -lgcc")
add_definitions(-DTOOLCHAIN_GCC_ARM -DTOOLCHAIN_GCC -DARM_MATH_CM3 -DTARGET_CORTEX_M -DTARGET_LPC176X -DTARGET_NXP -DTARGET_MBED_LPC1768 -DTARGET_LPC1768 -D__CORTEX_M3 -DTARGET_M3 -D__MBED__=1)