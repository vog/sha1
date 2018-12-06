#include "SHA1ToVerilog.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <cstdio>

#include "mbed.h"
#include "MODSERIAL.h"

#define REPETITIONS 1000

// Compare a result and the expected value
// this code is copypasta'd from their test suite in test_sha1
//
// @param[in]  result    The result
// @param[in]  expected  The expected
//
bool compare(const std::string &result, const std::string &expected)
{
    const std::string &state = (result == expected) ? "OK" : "Failure";
    std::cout << "Result:   " << result << std::endl;
    std::cout << "Expected: " << expected << "  (" << state << ")" << std::endl;
    return (result == expected);
}

/**
 * @brief      Execute a test run of the SHA-1 algorithm, and print out a timing analysis
 *
 * @param[in]  input           The input std::string to hash
 * @param[in]  expectedOutput  The expected output hash
 */
bool runTest(const std::string& input, const std::string& expectedOutput)
{
	SHA1Top checksum;
    std::cout << std::endl;
    std::cout << "Test: " << input << std::endl;

    std::string hash;
    int runtime=0;
    int sum=0, min=INT32_MAX;
    Timer runTimer;
	for (int i=0; i<REPETITIONS; i++)
	{
        runTimer.reset();
        runTimer.start();
	    hash = checksum.hash(input);
        runTimer.stop();
        runtime = runTimer.read_us();
		min = (runtime < min ? runtime : min);
		sum+=runtime;
	}
	  
    bool valid = compare(hash, expectedOutput);

    std::cout << "Runtimes over " << REPETITIONS << " reps:" << std::endl;;
    std::cout << "Mean:	" << sum/(double)REPETITIONS << "us" << std::endl;
    std::cout << "Min: 	" << min << "us" << std::endl;
    std::cout << std::endl;
    return valid;
}

int main(int argc, const char *argv[])
{
    MODSERIAL pc(USBTX, USBRX, 512); //DEBUG serial line, 512 byte buffers
    std::cout << "Hello World! <3 MBED LPC1768" << std::endl;
    runTest("abc", "a9993e364706816aba3e25717850c26c9cd0d89d");
    runTest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
}