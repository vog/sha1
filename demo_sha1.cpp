/*
    demo_sha1.cpp - demo program of

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
*/

#include "sha1.hpp"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

int main(int /* argc */, char const ** /* argv */) {
  string const input = "abc";

  SHA1 checksum;
  checksum.update(input);
  string const hash = checksum.final();

  cout << "The SHA-1 of \"" << input << "\" is: " << hash << endl;

  return 0;
}
