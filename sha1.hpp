#ifndef __SHA1_HPP__
#define __SHA1_HPP__

#define SHA1_DIGEST_LENGTH 20
#define SHA1_BLOCK_LENGTH 64

class SHA1
{
public:
    SHA1();
    void update(const char *buffer, size_t len);
    const char *final();
private:
    void init();
};

#endif /* __SHA1_HPP__ */
