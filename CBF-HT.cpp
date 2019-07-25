//
// Created by pku217 on 2019/5/14.
//

//
// Created by pku217 on 2019/4/24.
//



#if NDN_HASH_BASED_FIB_DEBUG_ON
#define fib_debug(M, ...) printf(M, __VA_ARGS__)
#else
#define fib_debug(M, ...)
#endif //!NDN_HASH_BASED_FIB_DEBUG_ON

#include "CBF-HT.h"
#include <string.h>

int CBF_HT::lookup_times = 0;
int CBF_HT::no_matchs = 0;

CBloomFilter bloomFilter;


//输入应形如"/s1/s2/s3/s4..."
ForwardInfo* CBF_HT::LPM_search(const string& input) const {
    //统计查询名字前缀的组件数目以及各组件的开始下标
    unsigned int slash_pos[MAX_COMP_NUM];
    int comp_num = 0;
    for (unsigned int i = 0; i < input.length(); i++) {
        if (input[i] == '/') {
            slash_pos[comp_num] = i;
            comp_num++;
        }
    }
    slash_pos[comp_num] = input.length();
    if (comp_num == 0) { //输入错误
        printf("Wrong Input\n"); system("pause");
        exit(-1);
    }

    int flag[MAX_COMP_NUM];
    memset(flag,0,sizeof(int)*MAX_COMP_NUM);

    //二分查找
    int lastHitComp = 0;
    bool isExist;
    int high = comp_num; int low = 1;
    while (low <= high) {
        lookup_times++;
        int mid = (high + low) / 2;
        //printf("mid = %d\n",mid);
        if((mid%2)==0){
            if(flag[mid+1]==0) mid = mid + 1;
            else if(flag[mid-1]==0) mid = mid - 1;
            else break;
        }
        if(flag[mid]==1) break;
        isExist = bloomFilter.IsContain((char*)input.c_str(),slash_pos[mid]);
        //printf("    L = %d, Try prefix: %s\n", mid, input.substr(0, slash_pos[mid]).c_str());
        flag[mid] = 1;
        if (isExist) { //命中
            lastHitComp = mid;
            low = mid + 2;
        }
        else {
            high = mid - 2;
        }
    }
    //从mid+1进行线性回溯哈希表
    if (lastHitComp != 0) {
        lastHitComp = lastHitComp + 1;
        //printf("lastHitComp = %d\n",lastHitComp);
        while(lastHitComp>=1){
            lookup_times++;
            //printf("lastHitComp = %d\n",lastHitComp);
            struct kv* item = hash_table_get(mMap,input.substr(0,slash_pos[lastHitComp]));
            //printf("try name prefix: %s\n", input.substr(0, slash_pos[lastHitComp]).c_str());
            if ((item != NULL)){  //命中
                //printf("match name prefix: %s\n", input.substr(0, slash_pos[lastHitComp]).c_str());
                return &item->info;
            }
            lastHitComp--;
        }
    }

    //搜索失败
    //printf("No name prefix match!\n");
    no_matchs++;

    return NULL;
}



ForwardInfo* CBF_HT::linear_search(const string& input) const {
    int slash_pos[MAX_COMP_NUM + 1];
    int comp_num = 0;
    for (unsigned int i = 0; i < input.length(); i++) {
        if (input[i] == '/') {
            slash_pos[comp_num] = i;
            comp_num++;
        }
    }
    slash_pos[comp_num] = input.length();
    if (comp_num == 0) { //输入错误
        printf("Wrong Input\n"); system("pause");
        exit(-1);
    }

    for (int i = comp_num; i > 0; i--) {
        lookup_times++;
        struct kv* item = hash_table_get(mMap,input.substr(0,slash_pos[i]));
        //printf("    L = %d, Try prefix: %s\n", i, input.substr(0, slash_pos[i]).c_str());
        if ((item != NULL)){
            //printf("match!\n");
            return &item->info;
        }
    }
    //printf("No name prefix match!\n");
    no_matchs++;
    return NULL;
}


//内存优化二分查询：插入过程
void CBF_HT::insert(const string& input, const ForwardInfo& info) {
    struct kv* item = hash_table_get(mMap,input);
    if (item != NULL) {
        //已经存在相应表项
        //printf("    %s already exists in table\n", input.c_str());
        item->info = info;
    }
    else {
        //不存在相应表项，把该名字及其奇数长度的前缀加进FIB表
        int comp_num = 0;
        unsigned int slash_pos[MAX_COMP_NUM];
        for (int i = 0; i < input.length(); i++) {
            if (input[i] == '/') {
                slash_pos[comp_num] = i;
                comp_num++;
            }
        }
        slash_pos[comp_num] = input.length();
        if (comp_num == 0) { //输入错误
            printf("Wrong Input\n"); system("pause");
            exit(-1);
        }
        //建立实节点,存进hash表
        int result = hash_table_put(mMap,input,info);
        if(result != 0) printf("insert error!\n");
        //printf("    %s doesn't exist in table, insert real entry.\n", input.c_str());

        //向上递归地建立虚节点，存进 counting bloom filter
        int index = ((comp_num%2)==0) ? comp_num-1 : comp_num;
        for(int i = index; i >= 1; i-=2){
            //查找前一级前缀是否存在于表中
            bloomFilter.InsertBloomFilter((char*)input.c_str(),slash_pos[i]);
            //printf("    %s doesn't exist in table, insert virtual entry.\n", input.substr(0,slash_pos[i]).c_str());
        }
    }
}




bool CBF_HT::erase(const string& input) {
    struct kv* item = hash_table_get(mMap,input);
    if (item == NULL) { //表中不存在待删除项
        return false;
    }
    else {
        hash_table_rm(mMap, input);

        int comp_num = 0;
        unsigned int slash_pos[MAX_COMP_NUM];
        for (int i = 0; i < input.length(); i++) {
            if (input[i] == '/') {
                slash_pos[comp_num] = i;
                comp_num++;
            }
        }
        slash_pos[comp_num] = input.length();
        if (comp_num == 0) { //输入错误
            printf("Wrong Input\n"); system("pause");
            exit(-1);
        }

        int index = ((comp_num%2)==0) ? comp_num-1 : comp_num;
        for(int i = index; i >= 1; i-=2){
            bloomFilter.RemoveBloomFilter((char*)input.c_str(),slash_pos[i]);
        }
    }
    return true;
}
