CROSS =
CXX   = $(CROSS)g++
RM    = rm -f
BIN 	= bin

all: demo_sha1 test_sha1 test_sha1_verilog

demo_sha1: demo_sha1.cpp sha1.cpp sha1.hpp
	$(CXX) -std=c++11 -o $(BIN)/$@ demo_sha1.cpp sha1.cpp

test_sha1: test_sha1.cpp sha1.cpp sha1.hpp
	$(CXX) -std=c++11 -o $(BIN)/$@ test_sha1.cpp sha1.cpp

test_sha1_verilog: test_sha1_verilog.cpp sha1.cpp sha1.hpp SHA1ToVerilog.hpp
	$(CXX) -std=c++11 -o $(BIN)/$@ test_sha1_verilog.cpp sha1.cpp

check: test_sha1
	./test_sha1

clean:
	$(RM) $(BIN)/*
