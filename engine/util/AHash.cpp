#include "AHash.h"

unsigned int AHash::murMur2(const void *key, int len, unsigned int seed)
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    unsigned int h = seed ^ len;
    const unsigned char *data = (const unsigned char *)key;
    while(len >= 4)
    {
        unsigned int k = *(unsigned int *)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch(len)
    {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
                h *= m;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;

}

unsigned long long AHash::murMur64A(const void *key, int len, unsigned long long seed)
{
    const unsigned long long m = 0xc6a4a7935bd1e995LLU;
    const int r = 47;

    unsigned long long h = seed ^ (len * m);

    const unsigned long long *data = (const unsigned long long *)key;
    const unsigned long long *end = data + (len/8);

    while(data != end)
    {
      unsigned long long k = *data++;

      k *= m; 
      k ^= k >> r; 
      k *= m; 
      
      h ^= k;
      h *= m; 
    }

    const unsigned char * data2 = (const unsigned char*)data;

    switch(len & 7)
    {
    case 7: h ^= unsigned long long(data2[6]) << 48;
    case 6: h ^= unsigned long long(data2[5]) << 40;
    case 5: h ^= unsigned long long(data2[4]) << 32;
    case 4: h ^= unsigned long long(data2[3]) << 24;
    case 3: h ^= unsigned long long(data2[2]) << 16;
    case 2: h ^= unsigned long long(data2[1]) << 8;
    case 1: h ^= unsigned long long(data2[0]);
            h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}
