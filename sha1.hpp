/*
    sha1.hpp - source code of

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
    Safety fixes
        -- Eugene Hopkinson <slowriot at voxelstorm dot com>
    Header-only library
        -- Zlatko Michailov <zlatko@michailov.org>
    C++ refactoring
        -- Dan Machado <dan-machado@yandex.com>
*/

#ifndef SHA1_HPP
#define SHA1_HPP


#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


class SHA1
{
public:
    SHA1();
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();
    static std::string from_file(const std::string &filename);

private:    

    static const size_t BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
    static const size_t BLOCK_BYTES = BLOCK_INTS * 4;

     uint32_t m_dataBlock[BLOCK_INTS];
    uint32_t m_digest[5];
    std::string m_buffer;
    uint64_t transforms;

    void reset();
    uint32_t rol(const uint32_t value, const size_t bits);
    uint32_t blk(const size_t i);
    void R0(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void R1(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void R2(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void R3(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void R4(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
    void transform();
    void buffer_to_block();
};


inline void SHA1::reset()
{
    /* SHA1 initialization constants */
    m_digest[0] = 0x67452301;
    m_digest[1] = 0xefcdab89;
    m_digest[2] = 0x98badcfe;
    m_digest[3] = 0x10325476;
    m_digest[4] = 0xc3d2e1f0;

    /* Reset counters */
    m_buffer = "";
    transforms = 0;
}


inline uint32_t SHA1::rol(const uint32_t value, const size_t bits)
{
    return (value << bits) | (value >> (32 - bits));
}


inline uint32_t SHA1::blk(const size_t i)
{
    return rol(m_dataBlock[(i+13)&15] ^ m_dataBlock[(i+8)&15] ^ m_dataBlock[(i+2)&15] ^ m_dataBlock[i], 1);
}


/*
 * (R0+R1), R2, R3, R4 are the different operations used in SHA1
 */

inline void SHA1::R0(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    z += ((w&(x^y))^y) + m_dataBlock[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}


inline void SHA1::R1(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    m_dataBlock[i] = blk(i);
    z += ((w&(x^y))^y) + m_dataBlock[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}


inline void SHA1::R2(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    m_dataBlock[i] = blk(i);
    z += (w^x^y) + m_dataBlock[i] + 0x6ed9eba1 + rol(v, 5);
    w = rol(w, 30);
}


inline void SHA1::R3(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    m_dataBlock[i] = blk(i);
    z += (((w|x)&y)|(w&x)) + m_dataBlock[i] + 0x8f1bbcdc + rol(v, 5);
    w = rol(w, 30);
}


inline void SHA1::R4(const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    m_dataBlock[i] = blk(i);
    z += (w^x^y) + m_dataBlock[i] + 0xca62c1d6 + rol(v, 5);
    w = rol(w, 30);
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

inline void SHA1::transform()
{
    /* Copy m_digest[] to working vars */
    uint32_t a = m_digest[0];
    uint32_t b = m_digest[1];
    uint32_t c = m_digest[2];
    uint32_t d = m_digest[3];
    uint32_t e = m_digest[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a, b, c, d, e,  0);
    R0(e, a, b, c, d,  1);
    R0(d, e, a, b, c,  2);
    R0(c, d, e, a, b,  3);
    R0(b, c, d, e, a,  4);
    R0(a, b, c, d, e,  5);
    R0(e, a, b, c, d,  6);
    R0(d, e, a, b, c,  7);
    R0(c, d, e, a, b,  8);
    R0(b, c, d, e, a,  9);
    R0(a, b, c, d, e, 10);
    R0(e, a, b, c, d, 11);
    R0(d, e, a, b, c, 12);
    R0(c, d, e, a, b, 13);
    R0(b, c, d, e, a, 14);
    R0(a, b, c, d, e, 15);
    R1(e, a, b, c, d,  0);
    R1(d, e, a, b, c,  1);
    R1(c, d, e, a, b,  2);
    R1(b, c, d, e, a,  3);
    R2(a, b, c, d, e,  4);
    R2(e, a, b, c, d,  5);
    R2(d, e, a, b, c,  6);
    R2(c, d, e, a, b,  7);
    R2(b, c, d, e, a,  8);
    R2(a, b, c, d, e,  9);
    R2(e, a, b, c, d, 10);
    R2(d, e, a, b, c, 11);
    R2(c, d, e, a, b, 12);
    R2(b, c, d, e, a, 13);
    R2(a, b, c, d, e, 14);
    R2(e, a, b, c, d, 15);
    R2(d, e, a, b, c,  0);
    R2(c, d, e, a, b,  1);
    R2(b, c, d, e, a,  2);
    R2(a, b, c, d, e,  3);
    R2(e, a, b, c, d,  4);
    R2(d, e, a, b, c,  5);
    R2(c, d, e, a, b,  6);
    R2(b, c, d, e, a,  7);
    R3(a, b, c, d, e,  8);
    R3(e, a, b, c, d,  9);
    R3(d, e, a, b, c, 10);
    R3(c, d, e, a, b, 11);
    R3(b, c, d, e, a, 12);
    R3(a, b, c, d, e, 13);
    R3(e, a, b, c, d, 14);
    R3(d, e, a, b, c, 15);
    R3(c, d, e, a, b,  0);
    R3(b, c, d, e, a,  1);
    R3(a, b, c, d, e,  2);
    R3(e, a, b, c, d,  3);
    R3(d, e, a, b, c,  4);
    R3(c, d, e, a, b,  5);
    R3(b, c, d, e, a,  6);
    R3(a, b, c, d, e,  7);
    R3(e, a, b, c, d,  8);
    R3(d, e, a, b, c,  9);
    R3(c, d, e, a, b, 10);
    R3(b, c, d, e, a, 11);
    R4(a, b, c, d, e, 12);
    R4(e, a, b, c, d, 13);
    R4(d, e, a, b, c, 14);
    R4(c, d, e, a, b, 15);
    R4(b, c, d, e, a,  0);
    R4(a, b, c, d, e,  1);
    R4(e, a, b, c, d,  2);
    R4(d, e, a, b, c,  3);
    R4(c, d, e, a, b,  4);
    R4(b, c, d, e, a,  5);
    R4(a, b, c, d, e,  6);
    R4(e, a, b, c, d,  7);
    R4(d, e, a, b, c,  8);
    R4(c, d, e, a, b,  9);
    R4(b, c, d, e, a, 10);
    R4(a, b, c, d, e, 11);
    R4(e, a, b, c, d, 12);
    R4(d, e, a, b, c, 13);
    R4(c, d, e, a, b, 14);
    R4(b, c, d, e, a, 15);

    /* Add the working vars back into m_digest[] */
    m_digest[0] += a;
    m_digest[1] += b;
    m_digest[2] += c;
    m_digest[3] += d;
    m_digest[4] += e;

    /* Count the number of transformations */
    transforms++;
}


inline void SHA1::buffer_to_block()
{
    /* Convert the std::string (byte m_buffer) to a uint32_t array (MSB) */
    for (size_t i = 0; i < BLOCK_INTS; i++)
    {
        m_dataBlock[i] = (m_buffer[4*i+3] & 0xff)
                   | (m_buffer[4*i+2] & 0xff)<<8
                   | (m_buffer[4*i+1] & 0xff)<<16
                   | (m_buffer[4*i+0] & 0xff)<<24;
    }
}


inline SHA1::SHA1()
{
    reset();
}


inline void SHA1::update(const std::string &s)
{
    std::istringstream is(s);
    update(is);
}


inline void SHA1::update(std::istream &is)
{
    while (true)
    {
        char sbuf[BLOCK_BYTES];
        is.read(sbuf, BLOCK_BYTES - m_buffer.size());
        m_buffer.append(sbuf, (std::size_t)is.gcount());
        if (m_buffer.size() != BLOCK_BYTES)
        {
            return;
        }
        //uint32_t m_dataBlock[BLOCK_INTS];
        buffer_to_block();
        transform();
        m_buffer.clear();
    }
}


/*
 * Add padding and return the message m_digest.
 */

inline std::string SHA1::final()
{
    /* Total number of hashed bits */
    uint64_t total_bits = (transforms*BLOCK_BYTES + m_buffer.size()) * 8;

    /* Padding */
    m_buffer += (char)0x80;
    size_t orig_size = m_buffer.size();
    while (m_buffer.size() < BLOCK_BYTES)
    {
        m_buffer += (char)0x00;
    }

    //uint32_t m_dataBlock[BLOCK_INTS];
    buffer_to_block();

    if (orig_size > BLOCK_BYTES - 8)
    {
        transform();
        for (size_t i = 0; i < BLOCK_INTS - 2; i++)
        {
            m_dataBlock[i] = 0;
        }
    }

    /* Append total_bits, split this uint64_t into two uint32_t */
    m_dataBlock[BLOCK_INTS - 1] = (uint32_t)total_bits;
    m_dataBlock[BLOCK_INTS - 2] = (uint32_t)(total_bits >> 32);
    transform();

    /* Hex std::string */
    std::ostringstream result;
    for (size_t i = 0; i < sizeof(m_digest) / sizeof(m_digest[0]); i++)
    {
        result << std::hex << std::setfill('0') << std::setw(8);
        result << m_digest[i];
    }

    /* Reset for next run */
    reset();

    return result.str();
}


inline std::string SHA1::from_file(const std::string &filename)
{
    std::ifstream stream(filename.c_str(), std::ios::binary);
    SHA1 checksum;
    checksum.update(stream);
    return checksum.final();
}


#endif /* SHA1_HPP */
