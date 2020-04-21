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
 * This method is defined in test_sha1_file.cpp.
 * The purpose of the split is to test linking of multiple files that include sha1.hpp.
 */
void test_file(const string &filename);


void compare(const string &result, const string &expected)
{
    const string &state = (result == expected) ? "OK" : "Failure";
    cout << "Result:   " << result << endl;
    cout << "Expected: " << expected << "  (" << state << ")" << endl;
}


/*
 * The 3 test vectors from FIPS PUB 180-1
 */

void test_standard()
{
    // https://www.di-mgt.com.au/sha_testvectors.html
    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA1.pdf

    SHA1 checksum;

    cout << endl;
    cout << "Test:     abc" << endl;
    checksum.update("abc");
    compare(checksum.final(), "a9993e364706816aba3e25717850c26c9cd0d89d");

    cout << endl;
    cout << "Test:     abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" << endl;
    checksum.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
    compare(checksum.final(), "84983e441c3bd26ebaae4aa1f95129e5e54670f1");


    cout << endl;
    cout << "Test:     abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" << endl;
    checksum.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
    compare(checksum.final(), "a49b2446a02c645bf419f995b67091253a04a259");

    cout << endl;
    cout << "Test:     A million repetitions of 'a'" << endl;
    for (int i = 0; i < 1000000/200; ++i)
    {
        checksum.update("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                       );
    }
    compare(checksum.final(), "34aa973cd4c4daa4f61eeb2bdbad27316534016f");

    // https://en.wikipedia.org/wiki/SHA-1
    cout << endl;
    cout << "Test:     The quick brown fox jumps over the lazy dog" << endl;
    checksum.update("The quick brown fox jumps over the lazy dog");
    compare(checksum.final(), "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");

    cout << endl;
    cout << "Test:     The quick brown fox jumps over the lazy cog" << endl;
    checksum.update("The quick brown fox jumps over the lazy cog");
    compare(checksum.final(), "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3");
}

void test_slow()
{
    // https://www.di-mgt.com.au/sha_testvectors.html

    SHA1 checksum;

    cout << endl;
    cout << "Test:     16,777,216 repititions of abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno" << endl;
    for (int i = 0; i < 16777216; ++i)
    {
        checksum.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno");
    }
    compare(checksum.final(), "7789f0c9ef7bfc40d93311143dfbe69e2017f592");
}

/*
 * Other tests
 */

void test_other()
{
    SHA1 checksum;

    cout << endl;
    cout << "Test:     No string" << endl;
    compare(checksum.final(), "da39a3ee5e6b4b0d3255bfef95601890afd80709");

    cout << endl;
    checksum.update("");
    cout << "Test:     Empty string" << endl;
    compare(checksum.final(), "da39a3ee5e6b4b0d3255bfef95601890afd80709");

    cout << endl;
    cout << "Test:     abcde" << endl;
    checksum.update("abcde");
    compare(checksum.final(), "03de6c570bfe24bfc328ccd7ca46b76eadaf4334");

    cout << endl;
    cout << "Test:     Two concurrent checksum calculations" << endl;
    SHA1 checksum1, checksum2;
    checksum1.update("abc");
    compare(checksum2.final(), "da39a3ee5e6b4b0d3255bfef95601890afd80709"); /* "" */
    compare(checksum1.final(), "a9993e364706816aba3e25717850c26c9cd0d89d"); /* "abc" */

    cout << endl;
    cout << "Test:     a [00] b [7F] c [80] d [FF] e [C3] [F0] f" << endl;
    checksum.update(std::string("a" "\x00" "b" "\x7f" "c" "\x80" "d" "\xff" "e" "\xc3\xf0" "f", 12));
    compare(checksum.final(), "cd0dd10814c0d4f9c6a2a0a4be2304d2371468d3");
}

/*
 * main
 */

int main(int argc, const char *argv[])
{
    const bool slow = (argc == 2 && std::string("--slow") == argv[1]);

    if (argc > 1 && !slow)
    {
        for (int i = 1; i < argc; i++)
        {
            test_file(argv[i]);
        }
    }
    else
    {
        test_standard();
        test_other();
        if (slow) {
            test_slow();
        }
        cout << endl;
        cout << endl;
    }

    return 0;
}
