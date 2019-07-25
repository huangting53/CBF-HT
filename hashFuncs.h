//
// Created by pku217 on 2019/5/14.
//

#ifndef HTFIB_BF_HASHFUNCS_H
#define HTFIB_BF_HASHFUNCS_H

#include <stdlib.h>

class CHashFuns{
public:
    CHashFuns(void);

    CHashFuns(int num);

    ~CHashFuns(void);

    // 生成retlen个随机hash值
    int GenerateHashValue(char* str, unsigned int len, unsigned int* Ret, int retLen);
    // 生成第hasFunIndex个 hash值
    unsigned int GenerateHashValue(char* str, unsigned int len, int hasFunIndex);
    // hash函数
    unsigned int RSHash(char* str, unsigned int len);
    /* End Of RS Hash Function */

    unsigned int JSHash(char* str, unsigned int len);
    /* End Of JS Hash Function */

    unsigned int PJWHash(char* str, unsigned int len);
    /* End Of  P. J. Weinberger Hash Function */

    unsigned int ELFHash(char* str, unsigned int len);
    /* End Of ELF Hash Function */

    unsigned int BKDRHash(char* str, unsigned int len);
    /* End Of BKDR Hash Function */

    unsigned int SDBMHash(char* str, unsigned int len);
    /* End Of SDBM Hash Function */

    unsigned int DJBHash(char* str, unsigned int len);
    /* End Of DJB Hash Function */

    unsigned int DEKHash(char* str, unsigned int len);
    /* End Of DEK Hash Function */

    unsigned int BPHash(char* str, unsigned int len);
    /* End Of BP Hash Function */

    unsigned int FNVHash(char* str, unsigned int len);
    /* End Of FNV Hash Function */

    unsigned int APHash(char* str, unsigned int len);
    /* End Of AP Hash Function */

private:
    unsigned int hasNum = 0; // k值，哈希函数的个数
};



#endif //HTFIB_BF_HASHFUNCS_H
