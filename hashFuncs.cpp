//
// Created by pku217 on 2019/5/14.
//


#include "hashFuncs.h"


CHashFuns::CHashFuns(void)
{
    this->hasNum = 10;
}

CHashFuns::CHashFuns(int num)
{
    if((num <= 0) || (num >= 12))
    {
        this->hasNum = 10;
    }else
    {
        this->hasNum = num;
    }
}

CHashFuns::~CHashFuns(void)
{

}

// 生成retlen个 随机hash值
int CHashFuns::GenerateHashValue(char* str, unsigned int len, unsigned int* Ret, int retLen)
{
    if(retLen > 12) // 总共有12个hash 函数
    {
        return -1;
    }
    for(int i = 0; i < retLen; i++)
    {
        Ret[i] = this->GenerateHashValue(str, len, i);
    }
    return retLen;
}
// 生成第hasFunIndex个 hash值
unsigned int CHashFuns::GenerateHashValue(char* str, unsigned int len, int hasFunIndex)
{
    switch(hasFunIndex)
    {
        case 0:
            return this->RSHash(str, len);
        case 1:
            return this->JSHash(str, len);
        case 2:
            return this->PJWHash(str, len);
        case 3:
            return this->ELFHash(str, len); // used in Unix
        case 4:
            return this->BKDRHash(str,len);
        case 5:
            return this->SDBMHash(str,len);
        case 6:
            return this->DJBHash(str, len);
        case 7:
            return this->DEKHash(str, len); // Knuth <The art of computer programming>
        case 8:
            return this->BPHash(str, len);
        case 9:
            return this->FNVHash(str, len);
        case 10:
            return this->APHash(str, len);
    }
}


unsigned int CHashFuns::RSHash(char* str, unsigned int len)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = hash * a + (*str);
        a    = a * b;
    }
    return hash;
}
/* End Of RS Hash Function */

unsigned int CHashFuns::JSHash(char* str, unsigned int len)
{
    unsigned int hash = 1315423911;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }
    return hash;
}
/* End Of JS Hash Function */

unsigned int CHashFuns::PJWHash(char* str, unsigned int len)
{
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash              = 0;
    unsigned int test              = 0;
    unsigned int i                 = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = (hash << OneEighth) + (*str);
        if((test = hash & HighBits)  != 0)
        {
            hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }
    return hash;
}
/* End Of  P. J. Weinberger Hash Function */

unsigned int CHashFuns::ELFHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = (hash << 4) + (*str);
        if((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}
/* End Of ELF Hash Function */

unsigned int CHashFuns::BKDRHash(char* str, unsigned int len)
{
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = (hash * seed) + (*str);
    }
    return hash;
}
/* End Of BKDR Hash Function */

unsigned int CHashFuns::SDBMHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}
/* End Of SDBM Hash Function */

unsigned int CHashFuns::DJBHash(char* str, unsigned int len)
{
    unsigned int hash = 5381;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = ((hash << 5) + hash) + (*str);
    }
    return hash;
}
/* End Of DJB Hash Function */

unsigned int CHashFuns::DEKHash(char* str, unsigned int len)
{
    unsigned int hash = len;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }
    return hash;
}
/* End Of DEK Hash Function */

unsigned int CHashFuns::BPHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = hash << 7 ^ (*str);
    }
    return hash;
}
/* End Of BP Hash Function */

unsigned int CHashFuns::FNVHash(char* str, unsigned int len)
{
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash      = 0;
    unsigned int i         = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash *= fnv_prime;
        hash ^= (*str);
    }
    return hash;
}
/* End Of FNV Hash Function */

unsigned int CHashFuns::APHash(char* str, unsigned int len)
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                (~((hash << 11) + (*str) ^ (hash >> 5)));
    }
    return hash;
}
/* End Of AP Hash Function */
