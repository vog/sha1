/*
    sha1.cpp - source code of

    ============
    SHA-1 in C++
    ============

    100% Public Domain.

    Original C Code
        -- Steve Reid <steve@edmweb.com>
    Small changes to fit into bglibs
        -- Bruce Guenter <bruce@untroubled.org>
    Translation to simpler C++ Code
        -- Volker Grabsch <vog@notjusthosting.com>
    Safety fixes
        -- Eugene Hopkinson <slowriot at voxelstorm dot com>
*/

#include "sha1.hpp"
#include <sstream>
#include <iomanip>
#include <fstream>


static const unsigned int BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
static const unsigned int BLOCK_BYTES = BLOCK_INTS * 4;


static void reset(uint32_t digest[], std::string &buffer, uint64_t &transforms)
{
    /* SHA1 initialization constants */
    digest[0] = 0x67452301;
    digest[1] = 0xefcdab89;
    digest[2] = 0x98badcfe;
    digest[3] = 0x10325476;
    digest[4] = 0xc3d2e1f0;

    /* Reset counters */
    buffer = "";
    transforms = 0;
}


static uint32_t rol(uint32_t value, size_t bits)
{
    return (value << bits) | (value >> (32 - bits));
}


static uint32_t blk(uint32_t block[BLOCK_INTS], size_t i)
{
    return rol(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15], 1);
}


/*
 * (R0+R1), R2, R3, R4 are the different operations used in SHA1
 */

static void R0(uint32_t block[BLOCK_INTS], uint32_t v, uint32_t &w, uint32_t x, uint32_t y, uint32_t &z, size_t i)
{
    z += ((w&(x^y))^y) + block[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}


static void R1(uint32_t block[BLOCK_INTS], uint32_t v, uint32_t &w, uint32_t x, uint32_t y, uint32_t &z, size_t i)
{
    block[i&15] = blk(block, i);
    z += ((w&(x^y))^y) + block[i&15] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}


static void R2(uint32_t block[BLOCK_INTS], uint32_t v, uint32_t &w, uint32_t x, uint32_t y, uint32_t &z, size_t i)
{
    block[i&15] = blk(block, i);
    z += (w^x^y) + block[i&15] + 0x6ed9eba1 + rol(v, 5);
    w = rol(w, 30);
}


static void R3(uint32_t block[BLOCK_INTS], uint32_t v, uint32_t &w, uint32_t x, uint32_t y, uint32_t &z, size_t i)
{
    block[i&15] = blk(block, i);
    z += (((w|x)&y)|(w&x)) + block[i&15] + 0x8f1bbcdc + rol(v, 5);
    w = rol(w, 30);
}


static void R4(uint32_t block[BLOCK_INTS], uint32_t v, uint32_t &w, uint32_t x, uint32_t y, uint32_t &z, size_t i)
{
    block[i&15] = blk(block, i);
    z += (w^x^y) + block[i&15] + 0xca62c1d6 + rol(v, 5);
    w = rol(w, 30);
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

static void transform(uint32_t digest[], uint32_t block[BLOCK_INTS], uint64_t &transforms)
{
    /* Copy digest[] to working vars */
    uint32_t a = digest[0];
    uint32_t b = digest[1];
    uint32_t c = digest[2];
    uint32_t d = digest[3];
    uint32_t e = digest[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(block, a, b, c, d, e,  0);
    R0(block, e, a, b, c, d,  1);
    R0(block, d, e, a, b, c,  2);
    R0(block, c, d, e, a, b,  3);
    R0(block, b, c, d, e, a,  4);
    R0(block, a, b, c, d, e,  5);
    R0(block, e, a, b, c, d,  6);
    R0(block, d, e, a, b, c,  7);
    R0(block, c, d, e, a, b,  8);
    R0(block, b, c, d, e, a,  9);
    R0(block, a, b, c, d, e, 10);
    R0(block, e, a, b, c, d, 11);
    R0(block, d, e, a, b, c, 12);
    R0(block, c, d, e, a, b, 13);
    R0(block, b, c, d, e, a, 14);
    R0(block, a, b, c, d, e, 15);
    R1(block, e, a, b, c, d, 16);
    R1(block, d, e, a, b, c, 17);
    R1(block, c, d, e, a, b, 18);
    R1(block, b, c, d, e, a, 19);
    R2(block, a, b, c, d, e, 20);
    R2(block, e, a, b, c, d, 21);
    R2(block, d, e, a, b, c, 22);
    R2(block, c, d, e, a, b, 23);
    R2(block, b, c, d, e, a, 24);
    R2(block, a, b, c, d, e, 25);
    R2(block, e, a, b, c, d, 26);
    R2(block, d, e, a, b, c, 27);
    R2(block, c, d, e, a, b, 28);
    R2(block, b, c, d, e, a, 29);
    R2(block, a, b, c, d, e, 30);
    R2(block, e, a, b, c, d, 31);
    R2(block, d, e, a, b, c, 32);
    R2(block, c, d, e, a, b, 33);
    R2(block, b, c, d, e, a, 34);
    R2(block, a, b, c, d, e, 35);
    R2(block, e, a, b, c, d, 36);
    R2(block, d, e, a, b, c, 37);
    R2(block, c, d, e, a, b, 38);
    R2(block, b, c, d, e, a, 39);
    R3(block, a, b, c, d, e, 40);
    R3(block, e, a, b, c, d, 41);
    R3(block, d, e, a, b, c, 42);
    R3(block, c, d, e, a, b, 43);
    R3(block, b, c, d, e, a, 44);
    R3(block, a, b, c, d, e, 45);
    R3(block, e, a, b, c, d, 46);
    R3(block, d, e, a, b, c, 47);
    R3(block, c, d, e, a, b, 48);
    R3(block, b, c, d, e, a, 49);
    R3(block, a, b, c, d, e, 50);
    R3(block, e, a, b, c, d, 51);
    R3(block, d, e, a, b, c, 52);
    R3(block, c, d, e, a, b, 53);
    R3(block, b, c, d, e, a, 54);
    R3(block, a, b, c, d, e, 55);
    R3(block, e, a, b, c, d, 56);
    R3(block, d, e, a, b, c, 57);
    R3(block, c, d, e, a, b, 58);
    R3(block, b, c, d, e, a, 59);
    R4(block, a, b, c, d, e, 60);
    R4(block, e, a, b, c, d, 61);
    R4(block, d, e, a, b, c, 62);
    R4(block, c, d, e, a, b, 63);
    R4(block, b, c, d, e, a, 64);
    R4(block, a, b, c, d, e, 65);
    R4(block, e, a, b, c, d, 66);
    R4(block, d, e, a, b, c, 67);
    R4(block, c, d, e, a, b, 68);
    R4(block, b, c, d, e, a, 69);
    R4(block, a, b, c, d, e, 70);
    R4(block, e, a, b, c, d, 71);
    R4(block, d, e, a, b, c, 72);
    R4(block, c, d, e, a, b, 73);
    R4(block, b, c, d, e, a, 74);
    R4(block, a, b, c, d, e, 75);
    R4(block, e, a, b, c, d, 76);
    R4(block, d, e, a, b, c, 77);
    R4(block, c, d, e, a, b, 78);
    R4(block, b, c, d, e, a, 79);

    /* Add the working vars back into digest[] */
    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
    digest[4] += e;

    /* Count the number of transformations */
    transforms++;
}


static void buffer_to_block(const std::string &buffer, uint32_t block[BLOCK_INTS])
{
    /* Convert the std::string (byte buffer) to a uint32_t array (MSB) */
    for (unsigned int i = 0; i < BLOCK_INTS; i++)
    {
        block[i] = (buffer[4*i+3] & 0xff)
                   | (buffer[4*i+2] & 0xff)<<8
                   | (buffer[4*i+1] & 0xff)<<16
                   | (buffer[4*i+0] & 0xff)<<24;
    }
}


static void read(std::istream &is, std::string &s, size_t max)
{
    char sbuf[max];
    is.read(sbuf, max);
    s.assign(sbuf, is.gcount());
}


SHA1::SHA1()
{
    reset(digest, buffer, transforms);
}


void SHA1::update(const std::string &s)
{
    std::istringstream is(s);
    update(is);
}


void SHA1::update(std::istream &is)
{
    std::string rest_of_buffer;
    read(is, rest_of_buffer, BLOCK_BYTES - buffer.size());
    buffer += rest_of_buffer;

    while (is)
    {
        uint32_t block[BLOCK_INTS];
        buffer_to_block(buffer, block);
        transform(digest, block, transforms);
        read(is, buffer, BLOCK_BYTES);
    }
}


/*
 * Add padding and return the message digest.
 */

std::string SHA1::final()
{
    /* Total number of hashed bits */
    uint64_t total_bits = (transforms*BLOCK_BYTES + buffer.size()) * 8;

    /* Padding */
    buffer += 0x80;
    unsigned int orig_size = buffer.size();
    while (buffer.size() < BLOCK_BYTES)
    {
        buffer += (char)0x00;
    }

    uint32_t block[BLOCK_INTS];
    buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8)
    {
        transform(digest, block, transforms);
        for (unsigned int i = 0; i < BLOCK_INTS - 2; i++)
        {
            block[i] = 0;
        }
    }

    /* Append total_bits, split this uint64_t into two uint32_t */
    block[BLOCK_INTS - 1] = total_bits;
    block[BLOCK_INTS - 2] = (total_bits >> 32);
    transform(digest, block, transforms);

    /* Hex std::string */
    std::ostringstream result;
    for (unsigned int i = 0; i < sizeof(digest) / sizeof(digest[0]); i++)
    {
        result << std::hex << std::setfill('0') << std::setw(8);
        result << (digest[i] & 0xffffffff);
    }

    /* Reset for next run */
    reset(digest, buffer, transforms);

    return result.str();
}


std::string SHA1::from_file(const std::string &filename)
{
    std::ifstream stream(filename.c_str(), std::ios::binary);
    SHA1 checksum;
    checksum.update(stream);
    return checksum.final();
}
