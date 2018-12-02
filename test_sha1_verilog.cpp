#include "SHA1ToVerilog.hpp"
#include <chrono>
#include <iostream>

using namespace std;

//this code is copypasta'd from their test suite in test_sha1	
void compare(const string &result, const string &expected)
{
    const string &state = (result == expected) ? "OK" : "Failure";
    cout << "Result:   " << result << endl;
    cout << "Expected: " << expected << "  (" << state << ")" << endl;
}

void runTest(const string& input, const string& expectedOutput)
{
	SHA1Top checksum;
    cout << endl;
    cout << "Test: " << input << endl;

	auto start = chrono::high_resolution_clock::now(); 
    std::string hash = checksum.hash(input);
	auto stop = chrono::high_resolution_clock::now(); 

	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); 
	  
	// To get the value of duration use the count() 
	// member function on the duration object 

    compare(hash, expectedOutput);

	cout << "The hash took " << duration.count() << " microseconds." << endl; 
}

int main(int argc, const char *argv[])
{
    runTest("abc", "a9993e364706816aba3e25717850c26c9cd0d89d");
    runTest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
}