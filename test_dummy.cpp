/*
    test_dummy.cpp - test program of
 
    ============
    SHA-1 in C++
    ============
 
    100% Public Domain.
 
    Original C Code
        -- Steve Reid <steve@edmweb.com>
    Small changes to fit into bglibs
        -- Bruce Guenter <bruce@untroubled.org>
    Translation to simpler C++ Code
        -- Volker Diels-Grabsch <v@njh.eu>
    Header-only library
        -- Zlatko Michailov
*/

#include "sha1.hpp"
#include "sha1.hpp" // Test including it more than once.
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;


static void compare(const string &result, const string &expected)
{
    const string &state = (result == expected) ? "OK" : "Failure";
    cout << "Result:   " << result << endl;
    cout << "Expected: " << expected << "  (" << state << ")" << endl;
}



/*
 * Dummy cpp file to test header-only.
 */

void test_dummy()
{
    SHA1 checksum;

    cout << endl;
    cout << "Dummy:     No string" << endl;
    compare(checksum.final(), "da39a3ee5e6b4b0d3255bfef95601890afd80709");

    cout << endl;
    checksum.update("");
    cout << "Dummy:     Empty string" << endl;
    compare(checksum.final(), "da39a3ee5e6b4b0d3255bfef95601890afd80709");

    cout << endl;
    cout << "Dummy:     abcde" << endl;
    checksum.update("abcde");
    compare(checksum.final(), "03de6c570bfe24bfc328ccd7ca46b76eadaf4334");

    cout << endl;
    cout << "Dummy:     Two concurrent checksum calculations" << endl;
    SHA1 checksum1, checksum2;
    checksum1.update("abc");
    compare(checksum2.final(), "da39a3ee5e6b4b0d3255bfef95601890afd80709"); /* "" */
    compare(checksum1.final(), "a9993e364706816aba3e25717850c26c9cd0d89d"); /* "abc" */

    cout << endl;
    cout << "Dummy:     a [00] b [7F] c [80] d [FF] e [C3] [F0] f" << endl;
    checksum.update(std::string("a" "\x00" "b" "\x7f" "c" "\x80" "d" "\xff" "e" "\xc3\xf0" "f", 12));
    compare(checksum.final(), "cd0dd10814c0d4f9c6a2a0a4be2304d2371468d3");
}

