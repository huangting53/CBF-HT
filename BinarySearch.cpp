

#if NDN_HASH_BASED_FIB_DEBUG_ON
#define fib_debug(M, ...) printf(M, __VA_ARGS__)
#else
#define fib_debug(M, ...)
#endif //!NDN_HASH_BASED_FIB_DEBUG_ON

#include "BinarySearch.h"


int BinarySearch::lookup_times = 0;
int BinarySearch::no_matchs = 0;
//����Ӧ����"/s1/s2/s3/s4..."
ForwardInfo* BinarySearch::LPM_search(const string& input) const {
    //ͳ�Ʋ�ѯ����ǰ׺�������Ŀ�Լ�������Ŀ�ʼ�±�
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
    if (comp_num == 0) { //�������
        printf("Wrong Input\n"); system("pause");
        exit(-1);
    }
    //���ֲ���
    string lastHitStr;
    struct kv* lastHit = NULL;
    int lastHitComp = 0;
    int high = comp_num; int low = 1;
    while (low <= high) {
        lookup_times++;
        int mid = (high + low) / 2;
        struct kv* result = (struct kv*)hash_table_get(mMap, input.substr(0, slash_pos[mid]));
        fib_debug("    L = %d, Try prefix %s: ", mid, input.substr(0, slash_pos[mid]).c_str());
        if (result != NULL) { //����
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
    //�����ѯ���
    if (lastHitComp != 0) {
        fib_debug("    Last hit %s ", lastHitStr.c_str());
        if (lastHit->value->type == Real) {
            //�������Ϊʵǰ׺
            //printf("match!\n");
            fib_debug("is real.\n", );
            return &lastHit->value->info;
        }
        else if (lastHit->value->type == SemiVirtual) {
            //�������Ϊ����ǰ׺ �����ϻ���ֱ���ҵ�һ��ʵǰ׺ (������һ������)
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

    //��mid�������Ի���
//    if (lastHit != NULL) {
//        fib_debug("    Last hit %s ", lastHitStr.c_str());
//        //printf("lastHitComp = %d\n",lastHitComp);
//
//        if(lastHit->value->type == Real) return &lastHit->value->info;
//
//        while(lastHitComp>=1){
//            lookup_times++;
//            struct kv* result = hash_table_get(mMap, input.substr(0, slash_pos[lastHitComp]));
//            if ((result != NULL) &&( result->value->type == Real)) { //����
//                fib_debug("HIT.\n")
//                //printf("match name prefix: %s\n", input.substr(0, slash_pos[lastHitComp]).c_str());
//                return &result->value->info;
//            }
//            lastHitComp--;
//        }
//    }

    //����ʧ��
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
    if (comp_num == 0) { //�������
        printf("Wrong Input\n"); system("pause");
        exit(-1);
    }

    for (int i = comp_num; i > 0; i--) {
        lookup_times++;
        struct kv* result = (struct kv*)hash_table_get(mMap, input.substr(0, slash_pos[i]));
        fib_debug("    L = %d, Try prefix %s: ", i, input.substr(0, slash_pos[i]).c_str());
        if ((result != NULL) &&( result->value->type == Real)) { //����
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
        //�Ѿ�������Ӧ����
        fib_debug("    %s already exists in table, ", input.c_str())
        switch (result->value->type) {
            case Virtual:
                //Ϊ��������ת����Ϣ���޸ı�����Ϊʵ�����������е����޸�Ϊ����
                result->value->type = Real;
                setNonrealNodesInSubtreeTo(result->value, SemiVirtual);
            case SemiVirtual:
                //Ϊ��������ת����Ϣ���޸ı�����Ϊʵ
                result->value->type = Real;
                result->value->info = info;
                fib_debug("its entry has been modified to real.\n")
                break;
            case Real:
                //Ϊʵ����滻ת����Ϣ
                result->value->info = info;
                fib_debug("its forward info has been updated.\n")
        }
    }
    else {
        //��������Ӧ�����ԭ�л������ӳ�
        //����ʵ�ڵ�
        AuxTreeNode *nNode, *preNode;
        nNode = new AuxTreeNode();
        preNode = new AuxTreeNode();
        nNode->type = Real;
        nNode->info = info;
        hash_table_put2(mMap,input,nNode);
        fib_debug("    %s doesn't exist in table, insert real entry.\n", input.c_str())
        int end_pos = input.length();
        //���ϵݹ�ؽ�����/����ڵ�
        while (1) {
            while (input[end_pos] != '/')
                end_pos--;
            if (end_pos == 0) {
                //�ѱ���������ֵ�ǰ׺
                addChild(root, nNode);
                return;
            }
            //����ǰһ��ǰ׺�Ƿ�����ڱ���
            result = (struct kv*)hash_table_get(mMap, input.substr(0, end_pos));
            if (result != NULL) {
                //ǰһ��ǰ׺�Ѵ��ڣ����ӵ��ýڵ���
                addChild(result->value, nNode);
                //����ýڵ�Ϊʵ����飬�������������з�ʵ�ڵ��Ϊ����
                if (result->value->type != Virtual) {
                    setNonrealNodesInSubtreeTo(nNode, SemiVirtual);
                }
                return;
            } else {
                //ǰһ��ǰ׺�������ڱ��У�������ڵ�
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
    if (result == NULL) { //���в����ڴ�ɾ����
        fib_debug("    %s doesn't exists in table.\n", input.c_str());
        return false;
    }
    if (result->value->firstChild != NULL) {
        fib_debug("    %s has child nodes, thus modified to ", input.c_str());
        //��������Ů ��ýڵ㲻�Ƴ� ������Ϊ��/����ڵ����
        result->value->info = 0;
        if (result->value->parent->type == Virtual) {
            //�ϼ��ڵ�Ϊ�飬������Ϊ��
            fib_debug("virtual, as well as its subtree.\n");
            result->value->type = Virtual;
            setNonrealNodesInSubtreeTo(result->value, Virtual);
        }
        else {
            fib_debug("semi-virtual\n");
            //�ϼ��ڵ�Ϊʵ/���飬������Ϊ���飬�������޸�
            result->value->type = SemiVirtual;
        }
    }
    else {
        fib_debug("    %s has no child nodes, thus removed.\n", input.c_str());
        //��������Ů ������ɾ��
        //preָ��ɾ���ڵ� curָpre�ĸ�ĸ
        AuxTreeNode* curNode;
        AuxTreeNode* preNode = result->value;
        hash_table_rm(mMap, input);
        int end_pos = input.length() + 1;
        while (1) {
            curNode = preNode->parent;
            removeChild(curNode, preNode);
            //�ж���һ���ڵ��Ƿ���Ҫɾ��
            if (curNode->firstChild == NULL &&
                curNode->type != Real &&
                curNode != root) {
                //�ϼ��ڵ���û����Ů�ķ�ʵ�ڵ�,���Ƿ�root�ڵ㣬��ɾ��
                delete preNode;
                preNode = curNode;
                //�ڱ���ɾ����Ӧǰ׺
                end_pos--;
                while (input[end_pos] != '/')
                    end_pos--;
                fib_debug("    %s is non-real and has no child nodes, thus removed.\n", input.substr(0, end_pos).c_str());
                hash_table_rm(mMap, input.substr(0, end_pos));
            }
            else
                //�ϼ��ڵ�����ɾ��
                break;
        }
        delete preNode;
    }
    return true;
}



