/* $Id: sha1.c 668 2006-02-19 05:58:04Z bruce $ */
/*
 SHA-1 in C

 By Steve Reid <steve@edmweb.com>.
 Small changes to make it fit by Bruce Guenter <bruce@untroubled.org>

 100% Public Domain.

 Test Vectors (from FIPS PUB 180-1)
 "abc"
 A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
 "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
 A million repetitions of "a"
 34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

#include <string.h>
#include "sha1.hpp"

/* portability */

#include <glib.h>

typedef guint32 uint32;
typedef guint64 uint64;


typedef struct {
    uint32 state[5];
    uint64 bytes;
    unsigned char buffer[SHA1_BLOCK_LENGTH];
} SHA1_CTX;


void uint32_pack_msb(uint32 u, unsigned char b[4])
{
  b[3] = u & 0xff;
  u >>= 8;
  b[2] = u & 0xff;
  u >>= 8;
  b[1] = u & 0xff;
  u >>= 8;
  b[0] = u;
}

void uint64_pack_msb(uint64 u, unsigned char b[4])
{
  uint32 hi, lo;
  lo = u;
  b[7] = lo & 0xff;
  lo >>= 8;
  b[6] = lo & 0xff;
  lo >>= 8;
  b[5] = lo & 0xff;
  lo >>= 8;
  b[4] = lo;
  hi = u >> 32;
  b[3] = hi & 0xff;
  hi >>= 8;
  b[2] = hi & 0xff;
  hi >>= 8;
  b[1] = hi & 0xff;
  hi >>= 8;
  b[0] = hi;
}







#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#ifdef ENDIAN_MSB
#  define blk0(i) block->l[i]
#else
#  define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
		    |(rol(block->l[i],8)&0x00FF00FF))
#endif

#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);


/* Hash a single 512-bit block. This is the core of the algorithm. */

void SHA1Transform(uint32 state[5], const unsigned char buffer[SHA1_BLOCK_LENGTH])
{
uint32 a, b, c, d, e;
typedef union {
    unsigned char c[64];
    uint32 l[16];
} CHAR64LONG16;
CHAR64LONG16 block[1];  /* use array to appear as a pointer */
    memcpy(block, buffer, SHA1_BLOCK_LENGTH);

    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    /* Wipe variables */
    a = b = c = d = e = 0;
#ifdef SHA1HANDSOFF
    memset(block, '\0', sizeof(block));
#endif
}


/* SHA1Init - Initialize new context */

void SHA1Init(SHA1_CTX* context)
{
    /* SHA1 initialization constants */
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->bytes = 0;
}


/* Run your data through this. */

void SHA1Update(SHA1_CTX* context, const unsigned char* data, uint32 len)
{
  unsigned blen = context->bytes % SHA1_BLOCK_LENGTH;
  unsigned use;
  context->bytes += len;
  if (blen > 0 && len >= (use = SHA1_BLOCK_LENGTH - blen)) {
    memcpy(context->buffer + blen, data, use);
    SHA1Transform(context->state, context->buffer);
    blen = 0;
    len -= use;
    data += use;
  }
  while (len >= SHA1_BLOCK_LENGTH) {
    SHA1Transform(context->state, data);
    len -= SHA1_BLOCK_LENGTH;
    data += SHA1_BLOCK_LENGTH;
  }
  memcpy(context->buffer + blen, data, len);
}


/* Add padding and return the message digest. */

void SHA1Final(SHA1_CTX* context, unsigned char digest[SHA1_DIGEST_LENGTH])
{
  unsigned i;
  unsigned blen = context->bytes % SHA1_BLOCK_LENGTH;
  context->buffer[blen++] = 0x80;
  memset(context->buffer + blen, 0, SHA1_BLOCK_LENGTH - blen);
  if (blen > SHA1_BLOCK_LENGTH-8) {
    SHA1Transform(context->state, context->buffer);
    memset(context->buffer, 0, SHA1_BLOCK_LENGTH-8);
  }
  uint64_pack_msb((context->bytes << 3),
		  context->buffer + SHA1_BLOCK_LENGTH - 8);
  SHA1Transform(context->state, context->buffer);
  for (i = 0; i < 5; ++i, digest += 4)
    uint32_pack_msb(context->state[i], digest);
  /* Wipe variables */
  memset(context, 0, sizeof(*context));
}

#ifdef SELFTEST_MAIN
#include "selftest.c"

static SHA1_CTX ctx;

static void init(void) { SHA1_Init(&ctx); }
static void hash(const char* ptr, long len) { SHA1_Update(&ctx, ptr, len); }
static void dump(void)
{
  unsigned i;
  unsigned char digest[SHA1_DIGEST_LENGTH];
  SHA1_Final(&ctx, digest);
  for (i = 0; i < sizeof(digest); ++i)
    obuf_putXw(&outbuf, digest[i], 2, '0');
  NL();
}

static void test(const char* ptr, long len)
{
  init();
  hash(ptr, len);
  dump();
}

MAIN
{
  unsigned i;
  /* Test Vectors (from FIPS PUB 180-1) */
  test("abc", 3);
  test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56);
  init();
  for (i = 0; i < 1000000/64; ++i)
    hash("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
	 64);
  dump();
}
#endif
#ifdef SELFTEST_EXP
A9993E364706816ABA3E25717850C26C9CD0D89D
84983E441C3BD26EBAAE4AA1F95129E5E54670F1
34AA973CD4C4DAA4F61EEB2BDBAD27316534016F
#endif



#include "sha1.hpp"
#include <stdio.h>

static SHA1_CTX ctx;
static unsigned char digest[SHA1_DIGEST_LENGTH];
static char outbuf[SHA1_DIGEST_LENGTH * 2 + 1];

SHA1::SHA1()
{
    init();
}

void SHA1::init()
{
    SHA1Init(&ctx);
}

void SHA1::update(const char *buffer, size_t len)
{
    SHA1Update(&ctx, (unsigned char *)buffer, len);
}

const char *SHA1::final()
{
    //char hex[] = "0123456789abcdef";
    SHA1Final(&ctx, digest);
    for (size_t i = 0; i < sizeof(digest); i++)
        snprintf(outbuf + 2*i, 3, "%02x", digest[i]);
    init();
    return outbuf;
}
