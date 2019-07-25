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

    //CBF���ǰ׺ƥ��(ͨ�����ֲ���)+HT�����Ի���
    ForwardInfo* LPM_search(const string& input) const;

    //HT���ǰ׺ƥ��(ͨ�����Բ���)
    ForwardInfo* linear_search(const string& input) const;

    //���ֲ���
    void insert(const string& input, const ForwardInfo& info);

    //����ɾ��
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
