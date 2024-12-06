CROSS     =
CXX       = $(CROSS)g++
RM        = rm -f
CXX_CLANG = clang++
CXX_EXTRA_FLAGS = -Werror -Weverything -Wno-c++98-compat-pedantic -Wno-unsafe-buffer-usage -Wno-switch-default

all: demo_sha1 test_sha1

demo_sha1: demo_sha1.cpp sha1.cpp sha1.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -Wall -Wextra -std=c++11 -Wconversion -o $@ demo_sha1.cpp sha1.cpp

test_sha1: test_sha1.cpp test_sha1_file.cpp sha1.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -Wall -Wextra -std=c++11 -Wconversion -o $@ test_sha1.cpp test_sha1_file.cpp sha1.cpp

test_clang1: test_sha1.cpp test_sha1_file.cpp sha1.hpp
	$(CXX_CLANG) $(CXX_EXTRA_FLAGS) -o $@  test_sha1.cpp test_sha1_file.cpp sha1.cpp


check: test_sha1
	./test_sha1

clean:
	$(RM) demo_sha1 test_sha1 test_clang
