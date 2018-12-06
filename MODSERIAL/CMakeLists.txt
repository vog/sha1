
set(MODSERIAL_SOURCES
	FLUSH.cpp
	GETC.cpp
	INIT.cpp
	ISR_RX.cpp
	ISR_TX.cpp
	MODSERIAL.cpp
	MODSERIAL_IRQ_INFO.cpp
	PUTC.cpp
	RESIZE.cpp)

add_library(MODSERIAL ${MODSERIAL_SOURCES})
target_include_directories(MODSERIAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MODSERIAL mbed-classic)