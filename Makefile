CROSS =
CXX   = $(CROSS)g++
RM    = rm -f

all: demo_sha1 test_sha1

demo_sha1: demo_sha1.cpp sha1.cpp sha1.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -Wall -Wextra -std=c++11 -o $@ demo_sha1.cpp sha1.cpp

test_sha1: test_sha1.cpp test_sha1_file.cpp sha1.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -Wall -Wextra -std=c++11 -o $@ test_sha1.cpp test_sha1_file.cpp sha1.cpp

check: test_sha1
	./test_sha1

clean:
	$(RM) demo_sha1 test_sha1
