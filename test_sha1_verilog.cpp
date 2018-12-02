#include "SHA1ToVerilog.hpp"
#include <iostream>

using namespace std;

//this code is copypasta'd from their test suite in test_sha1	
void compare(const string &result, const string &expected)
{
    const string &state = (result == expected) ? "OK" : "Failure";
    cout << "Result:   " << result << endl;
    cout << "Expected: " << expected << "  (" << state << ")" << endl;
}

int main(int argc, const char *argv[])
{
    SHA1Top checksum;

    cout << endl;
    cout << "Test:     abc" << endl;
    std::string hash = checksum.hash("abc");
    compare(hash, "a9993e364706816aba3e25717850c26c9cd0d89d");
}