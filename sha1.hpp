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
     /**
     * Partially calculate the sha1sum of its argument. The calculation
     * is completed by calling final(). Consecutive calls to update()
     * will have the effect of calculating the sha1 sum of the concatenated
     * string of its arguments. For example:
     * SHA1 sha1;
     * sha1.update(str1);
     * sha1.update(str2);
     * sha1.update(str3);
     * sha1.final();
     * is the same than
     * sha1.update(str1 + str2 + str3);
     * sha1.final();
     *
     * @param cstr null terminated string
     *
     * */
    void update(const std::string &s);
     void update(std::istream &is);

     /**
     * Complete the calculation of the sha1sum initiated by calls to
     * SHA1::update.
     *
     * @return return the sha1sum of the string (or strings)
     *         fed by calls to SHA1::update.
     * */
    std::string final();

     /**
     * Get the sha1sum of a file/
     *
     * @return return the sha1sum of the content of the specified file.
     * 
     * */
    static std::string from_file(const std::string &filename);

private:
    static const size_t BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
    static const size_t BLOCK_BYTES = BLOCK_INTS * 4;

    uint32_t m_dataBlock[BLOCK_INTS];
    uint32_t m_digest[5];
    std::string m_buffer;
    uint64_t transforms;

    void reset();
    template<int N, int M>
    uint32_t rol(uint32_t value);

    void blk(const size_t i);
    /*
    * R0, R1, R2, R3, R4 are the different operations used in SHA1
    */
    template<int v, int w, int x, int y, int z>
    void R0(const size_t i);

    template<int v, int w, int x, int y, int z>
    void R1(const size_t i);

    template<int v, int w, int x, int y, int z>
    void R2(const size_t i);

    template<int v, int w, int x, int y, int z>
    void R3(const size_t i);

    template<int v, int w, int x, int y, int z>
    void R4(const size_t i);

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


template<int N, int M>
inline uint32_t SHA1::rol(uint32_t value)
{
    return (value << N) | (value >> M);
}


inline void SHA1::blk(const size_t i)
{
     m_dataBlock[i]=rol<1, 31>(m_dataBlock[(i+13)&15] ^ m_dataBlock[(i+8)&15] ^ m_dataBlock[(i+2)&15] ^ m_dataBlock[i]);
}


/*
 * (R0+R1), R2, R3, R4 are the different operations used in SHA1
 */

template<int v, int w, int x, int y, int z>
inline void SHA1::R0(const size_t i)
{
    m_digest[z] =m_digest[z]+ ((m_digest[w]&(m_digest[x]^m_digest[y]))^m_digest[y]) + m_dataBlock[i] + 0x5a827999 + rol<5, 27>(m_digest[v]);
    m_digest[w] = rol<30, 2>(m_digest[w]);
}

template<int v, int w, int x, int y, int z>
inline void SHA1::R1(const size_t i)
{
    blk(i);
    R0<v,w,x,y,z>(i);
}

template<int v, int w, int x, int y, int z>
inline void SHA1::R2(const size_t i)
{
    blk(i);
    m_digest[z] =m_digest[z]+ (m_digest[w]^m_digest[x]^m_digest[y]) + m_dataBlock[i] + 0x6ed9eba1 + rol<5, 27>(m_digest[v]);
    m_digest[w] = rol<30, 2>(m_digest[w]);
}

template<int v, int w, int x, int y, int z>
inline void SHA1::R3(const size_t i)
{
    blk(i);
    m_digest[z] =m_digest[z]+ (((m_digest[w]|m_digest[x])&m_digest[y])|(m_digest[w]&m_digest[x])) + m_dataBlock[i] + 0x8f1bbcdc + rol<5, 27>(m_digest[v]);
    m_digest[w] = rol<30, 2>(m_digest[w]);
}

template<int v, int w, int x, int y, int z>
inline void SHA1::R4(const size_t i)
{
    blk(i);
    m_digest[z] =m_digest[z]+ (m_digest[w]^m_digest[x]^m_digest[y]) + m_dataBlock[i] + 0xca62c1d6 + rol<5, 27>(m_digest[v]);
    m_digest[w] = rol<30, 2>(m_digest[w]);
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
    R0<0, 1, 2, 3, 4>( 0);
    R0<4, 0, 1, 2, 3>( 1);
    R0<3, 4, 0, 1, 2>( 2);
    R0<2, 3, 4, 0, 1>( 3);
    R0<1, 2, 3, 4, 0>( 4);
    R0<0, 1, 2, 3, 4>( 5);
    R0<4, 0, 1, 2, 3>( 6);
    R0<3, 4, 0, 1, 2>( 7);
    R0<2, 3, 4, 0, 1>( 8);
    R0<1, 2, 3, 4, 0>( 9);
    R0<0, 1, 2, 3, 4>(10);
    R0<4, 0, 1, 2, 3>(11);
    R0<3, 4, 0, 1, 2>(12);
    R0<2, 3, 4, 0, 1>(13);
    R0<1, 2, 3, 4, 0>(14);
    R0<0, 1, 2, 3, 4>(15);
    R1<4, 0, 1, 2, 3>( 0);
    R1<3, 4, 0, 1, 2>( 1);
    R1<2, 3, 4, 0, 1>( 2);
    R1<1, 2, 3, 4, 0>( 3);
    R2<0, 1, 2, 3, 4>( 4);
    R2<4, 0, 1, 2, 3>( 5);
    R2<3, 4, 0, 1, 2>( 6);
    R2<2, 3, 4, 0, 1>( 7);
    R2<1, 2, 3, 4, 0>( 8);
    R2<0, 1, 2, 3, 4>( 9);
    R2<4, 0, 1, 2, 3>(10);
    R2<3, 4, 0, 1, 2>(11);
    R2<2, 3, 4, 0, 1>(12);
    R2<1, 2, 3, 4, 0>(13);
    R2<0, 1, 2, 3, 4>(14);
    R2<4, 0, 1, 2, 3>(15);
    R2<3, 4, 0, 1, 2>( 0);
    R2<2, 3, 4, 0, 1>( 1);
    R2<1, 2, 3, 4, 0>( 2);
    R2<0, 1, 2, 3, 4>( 3);
    R2<4, 0, 1, 2, 3>( 4);
    R2<3, 4, 0, 1, 2>( 5);
    R2<2, 3, 4, 0, 1>( 6);
    R2<1, 2, 3, 4, 0>( 7);
    R3<0, 1, 2, 3, 4>( 8);
    R3<4, 0, 1, 2, 3>( 9);
    R3<3, 4, 0, 1, 2>(10);
    R3<2, 3, 4, 0, 1>(11);
    R3<1, 2, 3, 4, 0>(12);
    R3<0, 1, 2, 3, 4>(13);
    R3<4, 0, 1, 2, 3>(14);
    R3<3, 4, 0, 1, 2>(15);
    R3<2, 3, 4, 0, 1>( 0);
    R3<1, 2, 3, 4, 0>( 1);
    R3<0, 1, 2, 3, 4>( 2);
    R3<4, 0, 1, 2, 3>( 3);
    R3<3, 4, 0, 1, 2>( 4);
    R3<2, 3, 4, 0, 1>( 5);
    R3<1, 2, 3, 4, 0>( 6);
    R3<0, 1, 2, 3, 4>( 7);
    R3<4, 0, 1, 2, 3>( 8);
    R3<3, 4, 0, 1, 2>( 9);
    R3<2, 3, 4, 0, 1>(10);
    R3<1, 2, 3, 4, 0>(11);
    R4<0, 1, 2, 3, 4>(12);
    R4<4, 0, 1, 2, 3>(13);
    R4<3, 4, 0, 1, 2>(14);
    R4<2, 3, 4, 0, 1>(15);
    R4<1, 2, 3, 4, 0>( 0);
    R4<0, 1, 2, 3, 4>( 1);
    R4<4, 0, 1, 2, 3>( 2);
    R4<3, 4, 0, 1, 2>( 3);
    R4<2, 3, 4, 0, 1>( 4);
    R4<1, 2, 3, 4, 0>( 5);
    R4<0, 1, 2, 3, 4>( 6);
    R4<4, 0, 1, 2, 3>( 7);
    R4<3, 4, 0, 1, 2>( 8);
    R4<2, 3, 4, 0, 1>( 9);
    R4<1, 2, 3, 4, 0>(10);
    R4<0, 1, 2, 3, 4>(11);
    R4<4, 0, 1, 2, 3>(12);
    R4<3, 4, 0, 1, 2>(13);
    R4<2, 3, 4, 0, 1>(14);
    R4<1, 2, 3, 4, 0>(15);

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
