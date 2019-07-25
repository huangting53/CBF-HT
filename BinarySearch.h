#ifndef H_BINARY_SEARCH
#define H_BINARY_SEARCH

#include "city-hash.h"
#include "hashtable.h"
#include "NDN-Config.h"
#include <string>
#include <string.h>
#include <iostream>
using namespace std;

//转发信息

class BinarySearch {
public:
    //接口

    //最长前缀匹配(通过二分查找)
    ForwardInfo* LPM_search(const string& input) const;

    //最长前缀匹配(通过线性查找)
    ForwardInfo* linear_search(const string& input) const;

    //前缀插入
    void insert(const string& input, const ForwardInfo& info);

    //前缀删除
    bool erase(const string& input);


private:
    AuxTreeNode* root; //名字前缀逻辑树的根

public:
    HashTable* mMap;

    BinarySearch(uint64 htSize) {
        root = new AuxTreeNode;
        mMap = hash_table_new(htSize);
    }

    ~BinarySearch() {
        removeAllChild(root);
        delete root;
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


#endif // !H_NDN_HASH_BASED_FIB

