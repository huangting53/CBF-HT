//
// Created by pku217 on 2019/5/14.
//

#ifndef HTFIB_BF_OBS_BF_H
#define HTFIB_BF_OBS_BF_H

#include "BloomFilter.h"
#include "hashtable.h"
#include "NDN-Config.h"


class CBF_HT {
public:

    //CBF中最长前缀匹配(通过二分查找)+HT中线性回溯
    ForwardInfo* LPM_search(const string& input) const;

    //HT中最长前缀匹配(通过线性查找)
    ForwardInfo* linear_search(const string& input) const;

    //名字插入
    void insert(const string& input, const ForwardInfo& info);

    //名字删除
    bool erase(const string& input);


public:

    HashTable* mMap;

    CBF_HT(uint64 htSize){
        mMap = hash_table_new(htSize);
    }

    ~CBF_HT(){
        hash_table_delete(mMap);
    }

private:
    static int lookup_times;
    static int no_matchs;

public:
    static int get_lookup_times() { return lookup_times; }
    static void reset_lookup_times() { lookup_times = 0; }
    static int get_no_matchs() { return no_matchs; }
    static void reset_no_matchs() { no_matchs = 0; }

}; // !class HashBasedFIB


#endif //HTFIB_BF_OBS_BF_H
