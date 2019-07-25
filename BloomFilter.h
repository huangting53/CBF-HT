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

    // ��ȡ�ļ��е��ַ��������ո����֣�Ȼ�����Ƕ��浽λ������
    void InitialBloomFilter(char* fileName);

    // ��ĳ��Ԫ�ز��뵽λ������
    void InsertBloomFilter(char* str, unsigned int len);

    // ��ĳ��Ԫ�ش�λ�������Ƴ�
    void RemoveBloomFilter(char* str, unsigned int len);

    // �ж�һ��Ԫ���Ƿ���λ������
    bool IsContain(char* str, unsigned int len);

private:
    CHashFuns hasFunHandle;
    unsigned int itemNum = 0; // ����Ԫ�ظ�������n
    int hasFunsNum = 0;	// ��ϣ�����ĸ�������kֵ
    unsigned int *hasFunsRet; // k����ϣ�����ķ���ֵ
    uint8 *bitmap;

};

#endif //HTFIB_BF_BLOOMFILTER_H
