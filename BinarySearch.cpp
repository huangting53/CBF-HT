

#if NDN_HASH_BASED_FIB_DEBUG_ON
#define fib_debug(M, ...) printf(M, __VA_ARGS__)
#else
#define fib_debug(M, ...)
#endif //!NDN_HASH_BASED_FIB_DEBUG_ON

#include "BinarySearch.h"


int BinarySearch::lookup_times = 0;
int BinarySearch::no_matchs = 0;
//输入应形如"/s1/s2/s3/s4..."
ForwardInfo* BinarySearch::LPM_search(const string& input) const {
    //统计查询名字前缀的组件数目以及各组件的开始下标
    fib_debug("[LPM BINARY] To search prefix %s:\n", input.c_str())
    int slash_pos[MAX_COMP_NUM];
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
    //二分查找
    string lastHitStr;
    struct kv* lastHit = NULL;
    int lastHitComp = 0;
    int high = comp_num; int low = 1;
    while (low <= high) {
        lookup_times++;
        int mid = (high + low) / 2;
        struct kv* result = (struct kv*)hash_table_get(mMap, input.substr(0, slash_pos[mid]));
        fib_debug("    L = %d, Try prefix %s: ", mid, input.substr(0, slash_pos[mid]).c_str());
        if (result != NULL) { //命中
            fib_debug("HIT.\n")
            lastHit = result;
            lastHitStr = input.substr(0, slash_pos[mid]);
            lastHitComp = mid;
            low = mid + 1;
        }
        else {
            fib_debug("MISS.\n")
            high = mid - 1;
        }
    }
    //检验查询结果
    if (lastHitComp != 0) {
        fib_debug("    Last hit %s ", lastHitStr.c_str());
        if (lastHit->value->type == Real) {
            //搜索结果为实前缀
            //printf("match!\n");
            fib_debug("is real.\n", );
            return &lastHit->value->info;
        }
        else if (lastHit->value->type == SemiVirtual) {
            //搜索结果为半虚前缀 则向上回溯直到找到一个实前缀 (理论上一定存在)
            fib_debug("is semi-virtual.\n")
            AuxTreeNode* ptr = lastHit->value;
            while (ptr->type != Real) {
                lookup_times++;
                ptr = ptr->parent;
            }
            //printf("match!\n");
            return &ptr->info;
        }
    }

    //从mid进行线性回溯
//    if (lastHit != NULL) {
//        fib_debug("    Last hit %s ", lastHitStr.c_str());
//        //printf("lastHitComp = %d\n",lastHitComp);
//
//        if(lastHit->value->type == Real) return &lastHit->value->info;
//
//        while(lastHitComp>=1){
//            lookup_times++;
//            struct kv* result = hash_table_get(mMap, input.substr(0, slash_pos[lastHitComp]));
//            if ((result != NULL) &&( result->value->type == Real)) { //命中
//                fib_debug("HIT.\n")
//                //printf("match name prefix: %s\n", input.substr(0, slash_pos[lastHitComp]).c_str());
//                return &result->value->info;
//            }
//            lastHitComp--;
//        }
//    }

    //搜索失败
    //printf("No name prefix match!\n");
    no_matchs++;
    fib_debug("    No prefix found.\n")

    return NULL;
}


ForwardInfo* BinarySearch::linear_search(const string & input) const {
    fib_debug("[LINEAR] To search prefix %s:\n", input.c_str())
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
        struct kv* result = (struct kv*)hash_table_get(mMap, input.substr(0, slash_pos[i]));
        fib_debug("    L = %d, Try prefix %s: ", i, input.substr(0, slash_pos[i]).c_str());
        if ((result != NULL) &&( result->value->type == Real)) { //命中
            //printf("match!\n");
            fib_debug("HIT.\n")
            return &result->value->info;
        }
    }
    //printf("No name prefix match!\n");
    no_matchs++;
    return NULL;
}



void BinarySearch::insert(const string& input, const ForwardInfo& info) {
    fib_debug("[INSERT] To insert prefix %s:\n", input.c_str());
    struct kv* result = (kv*)hash_table_get(mMap, input);
    if (result != NULL) {
        //已经存在相应表项
        fib_debug("    %s already exists in table, ", input.c_str())
        switch (result->value->type) {
            case Virtual:
                //为虚表项，设置转发信息，修改本表项为实，并将子树中的虚修改为半虚
                result->value->type = Real;
                setNonrealNodesInSubtreeTo(result->value, SemiVirtual);
            case SemiVirtual:
                //为半表项，设置转发信息，修改本表项为实
                result->value->type = Real;
                result->value->info = info;
                fib_debug("its entry has been modified to real.\n")
                break;
            case Real:
                //为实表项，替换转发信息
                result->value->info = info;
                fib_debug("its forward info has been updated.\n")
        }
    }
    else {
        //不存在相应表项，在原有基础上延长
        //建立实节点
        AuxTreeNode *nNode, *preNode;
        nNode = new AuxTreeNode();
        preNode = new AuxTreeNode();
        nNode->type = Real;
        nNode->info = info;
        hash_table_put2(mMap,input,nNode);
        fib_debug("    %s doesn't exist in table, insert real entry.\n", input.c_str())
        int end_pos = input.length();
        //向上递归地建立虚/半虚节点
        while (1) {
            while (input[end_pos] != '/')
                end_pos--;
            if (end_pos == 0) {
                //已遍历完该名字的前缀
                addChild(root, nNode);
                return;
            }
            //查找前一级前缀是否存在于表中
            result = (struct kv*)hash_table_get(mMap, input.substr(0, end_pos));
            if (result != NULL) {
                //前一级前缀已存在，连接到该节点上
                addChild(result->value, nNode);
                //如果该节点为实或半虚，则将其子树的所有非实节点改为半虚
                if (result->value->type != Virtual) {
                    setNonrealNodesInSubtreeTo(nNode, SemiVirtual);
                }
                return;
            } else {
                //前一级前缀不存在于表中，创建虚节点
                preNode = nNode;
                nNode = new AuxTreeNode();
                nNode->type = Virtual;
                addChild(nNode, preNode);
                hash_table_put2(mMap, input.substr(0, end_pos), nNode);
                fib_debug("    Insert non-real entry %s.\n", input.substr(0, end_pos).c_str());
                end_pos--;
            }
        }
    }
}


bool BinarySearch::erase(const string& input) {
    fib_debug("[ERASE] To erase prefix %s:\n", input.c_str());
    struct kv* result = (struct kv*)hash_table_get(mMap, input);
    //lookup_times++;
    if (result == NULL) { //表中不存在待删除项
        fib_debug("    %s doesn't exists in table.\n", input.c_str());
        return false;
    }
    if (result->value->firstChild != NULL) {
        fib_debug("    %s has child nodes, thus modified to ", input.c_str());
        //若存在子女 则该节点不移除 而是作为虚/半虚节点存在
        result->value->info = 0;
        if (result->value->parent->type == Virtual) {
            //上级节点为虚，则设置为虚
            fib_debug("virtual, as well as its subtree.\n");
            result->value->type = Virtual;
            setNonrealNodesInSubtreeTo(result->value, Virtual);
        }
        else {
            fib_debug("semi-virtual\n");
            //上级节点为实/半虚，则设置为半虚，子树不修改
            result->value->type = SemiVirtual;
        }
    }
    else {
        fib_debug("    %s has no child nodes, thus removed.\n", input.c_str());
        //不存在子女 则向上删除
        //pre指待删除节点 cur指pre的父母
        AuxTreeNode* curNode;
        AuxTreeNode* preNode = result->value;
        hash_table_rm(mMap, input);
        int end_pos = input.length() + 1;
        while (1) {
            curNode = preNode->parent;
            removeChild(curNode, preNode);
            //判断上一级节点是否需要删除
            if (curNode->firstChild == NULL &&
                curNode->type != Real &&
                curNode != root) {
                //上级节点是没有子女的非实节点,且是非root节点，故删除
                delete preNode;
                preNode = curNode;
                //在表中删除对应前缀
                end_pos--;
                while (input[end_pos] != '/')
                    end_pos--;
                fib_debug("    %s is non-real and has no child nodes, thus removed.\n", input.substr(0, end_pos).c_str());
                hash_table_rm(mMap, input.substr(0, end_pos));
            }
            else
                //上级节点无需删除
                break;
        }
        delete preNode;
    }
    return true;
}



