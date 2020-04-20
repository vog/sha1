/*
    test_sha1.cpp - test program of
 
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
        -- Zlatko Michailov <zlatko@michailov.org>
*/

#include "sha1.hpp"
#include "sha1.hpp" // Intentionally included twice for testing purposes 
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;


/*
 * Produce same output as "sha1sum -b"
 */

void test_file(const string &filename)
{
    cout << SHA1::from_file(filename) << " *" << filename << endl;
}
