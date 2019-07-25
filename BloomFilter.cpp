//
// Created by pku217 on 2019/5/14.
//

#include "BloomFilter.h"
unsigned long long Mht = 8e7;  //位数组的大小，即 m

CBloomFilter::CBloomFilter()
{
    this->hasFunsNum = 2;	// 哈希函数的个数k
    this->bitmap = new uint8_t[Mht];			// 位图初始化
    memset(bitmap,0,Mht*sizeof(uint8_t));
    this->hasFunsRet = new unsigned int[this->hasFunsNum]; // k个哈希函数的返回值
}

CBloomFilter::CBloomFilter(int itemsNum, int hasFunsNum)
{
    this->itemNum = itemsNum; // 过滤元素个数，即n
    this->hasFunsNum = hasFunsNum;	// 哈希函数的个数k
    this->bitmap = new uint8_t[Mht];			// 位图初始化
    memset(bitmap,0,Mht*sizeof(uint8_t));
    this->hasFunsRet = new unsigned int[this->hasFunsNum]; // k个哈希函数的返回值
}


CBloomFilter::~CBloomFilter(void)
{
    delete [] bitmap;
}

// 读取文件中的字符串，按空格区分，然后将他们都存到位数组中
void CBloomFilter::InitialBloomFilter(char* fileName)
{
    FILE* fin = fopen(fileName, "r");
    assert(fin);

    char ch;
    char str[1000] ={'\0'};
    int  iCnt = 0;
    while(fscanf(fin, "%c", &ch) != EOF)
    {
        if(ch != ' ')
        {
            str[iCnt] = ch;
            iCnt ++;
        }else
        {
            str[iCnt] = '\0';
            this->InsertBloomFilter(str, strlen(str));
            iCnt = 0;
        }
    }

    if(NULL != fin)
    {
        fclose(fin);
        fin = NULL;
    }
}

// 将某个元素插入到位数组中
void CBloomFilter::InsertBloomFilter(char* str, unsigned int len)
{
    this->hasFunHandle.GenerateHashValue(str, len, this->hasFunsRet, this->hasFunsNum);
    for(int i = 0; i < this->hasFunsNum; i++)
    {
        int bitMapPos = abs(this->hasFunsRet[i]%Mht);
        //this->bitmap[bitMapPos] = 1;
        this->bitmap[bitMapPos] += 1;
    }
}

// 将某个元素插入到位数组中
void CBloomFilter::RemoveBloomFilter(char* str, unsigned int len)
{
    this->hasFunHandle.GenerateHashValue(str, len, this->hasFunsRet, this->hasFunsNum);
    for(int i = 0; i < this->hasFunsNum; i++)
    {
        int bitMapPos = abs(this->hasFunsRet[i]%Mht);
        //this->bitmap[bitMapPos] = 0;
        this->bitmap[bitMapPos] -= 1;
    }
}

// 判断一个元素是否在位数组中
bool CBloomFilter::IsContain(char* str, unsigned int len)
{
    //char*   unsigned int  unsigned int *   int
    this->hasFunHandle.GenerateHashValue(str, len, this->hasFunsRet, this->hasFunsNum);
    for(int i = 0; i < this->hasFunsNum; i++)
    {
        int bitMapPos = abs(this->hasFunsRet[i]%Mht);
        //printf("%d\n",bitmap[bitMapPos]);
        if(this->bitmap[bitMapPos] == 0)
        {
            return false;
        }
    }
    return true;
}