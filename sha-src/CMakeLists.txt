add_mbed_executable(test-sha1 test_sha1_verilog.cpp sha1.cpp)
target_link_libraries(test-sha1 MODSERIAL mbed-classic)
