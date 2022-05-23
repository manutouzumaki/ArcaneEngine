#ifndef AHASH_H
#define AHASH_H

class AHash
{
public:
    static unsigned int murMur2(const void *key, int len, unsigned int seed);
    static unsigned long long murMur64A(const void *key, int len, unsigned long long seed);
};

#endif
