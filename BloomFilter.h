//
// Created by pku217 on 2019/5/14.
//

#ifndef HTFIB_BF_BLOOMFILTER_H
#define HTFIB_BF_BLOOMFILTER_H


#include <assert.h>
#include <math.h>
#include <stdint.h>
#include "hashFuncs.h"
#include "city-hash.h"
#include <string.h>
#include <cstdio>

class CBloomFilter{
public:
    CBloomFilter();
    CBloomFilter(int itemsNum, int hasFunsNum);

    ~CBloomFilter(void);

    // 读取文件中的字符串，按空格区分，然后将他们都存到位数组中
    void InitialBloomFilter(char* fileName);

    // 将某个元素插入到位数组中
    void InsertBloomFilter(char* str, unsigned int len);

    // 将某个元素从位数组中移除
    void RemoveBloomFilter(char* str, unsigned int len);

    // 判断一个元素是否在位数组中
    bool IsContain(char* str, unsigned int len);

private:
    CHashFuns hasFunHandle;
    unsigned int itemNum = 0; // 过滤元素个数，即n
    int hasFunsNum = 0;	// 哈希函数的个数，即k值
    unsigned int *hasFunsRet; // k个哈希函数的返回值
    uint8 *bitmap;

};

#endif //HTFIB_BF_BLOOMFILTER_H
