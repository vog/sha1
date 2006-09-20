#include <iostream>
using namespace std;

#include "sha1.hh"

void compare(const char *result, const char *expected)
{
    const char *state = (strcmp(result, expected) == 0) ? "OK" : "Failure";
    cout << "Result:   " << result << endl;
    cout << "Expected: " << expected << "  (" << state << ")" << endl;
}

int main(int argc, char *argv[])
{
    SHA1 checksum;

    cout << endl;
    cout << "Test:     abc" << endl;
    checksum.update("abc", 3);
    compare(checksum.final(), "a9993e364706816aba3e25717850c26c9cd0d89d");

    cout << endl;
    cout << "Test:     abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" << endl;
    checksum.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56);
    compare(checksum.final(), "84983e441c3bd26ebaae4aa1f95129e5e54670f1");

    cout << endl;
    cout << "Test:     A million repetitions of 'a'" << endl;
    for (int i = 0; i < 1000000/32; ++i) {
        checksum.update("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 32);
    }
    compare(checksum.final(), "34aa973cd4c4daa4f61eeb2bdbad27316534016f");

    cout << endl;
    cout << "Test:     Empty string" << endl;
    compare(checksum.final(), "da39a3ee5e6b4b0d3255bfef95601890afd80709");

    cout << endl;
    cout << "Test:     Concurrent checksums" << endl;
    SHA1 checksum1, checksum2;
    checksum1.update("abc", 3);
    compare(checksum2.final(), "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    compare(checksum1.final(), "a9993e364706816aba3e25717850c26c9cd0d89d");

    return 0;
}
