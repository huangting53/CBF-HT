//
// Created by pku217 on 2019/5/14.
//

#include "BloomFilter.h"
unsigned long long Mht = 8e7;  //λ����Ĵ�С���� m

CBloomFilter::CBloomFilter()
{
    this->hasFunsNum = 2;	// ��ϣ�����ĸ���k
    this->bitmap = new uint8_t[Mht];			// λͼ��ʼ��
    memset(bitmap,0,Mht*sizeof(uint8_t));
    this->hasFunsRet = new unsigned int[this->hasFunsNum]; // k����ϣ�����ķ���ֵ
}

CBloomFilter::CBloomFilter(int itemsNum, int hasFunsNum)
{
    this->itemNum = itemsNum; // ����Ԫ�ظ�������n
    this->hasFunsNum = hasFunsNum;	// ��ϣ�����ĸ���k
    this->bitmap = new uint8_t[Mht];			// λͼ��ʼ��
    memset(bitmap,0,Mht*sizeof(uint8_t));
    this->hasFunsRet = new unsigned int[this->hasFunsNum]; // k����ϣ�����ķ���ֵ
}


CBloomFilter::~CBloomFilter(void)
{
    delete [] bitmap;
}

// ��ȡ�ļ��е��ַ��������ո����֣�Ȼ�����Ƕ��浽λ������
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

// ��ĳ��Ԫ�ز��뵽λ������
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

// ��ĳ��Ԫ�ز��뵽λ������
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

// �ж�һ��Ԫ���Ƿ���λ������
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