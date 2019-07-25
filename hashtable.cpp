#include "hashtable.h"
#include <memory.h>
#define TABLE_SIZE (1024*1024)
#define MAX_LOAD_FACTOR 0.5


//树操作 添加子节点
void addChild(AuxTreeNode* nParent, AuxTreeNode* nChild) {
    if (nChild == NULL){
        printf("NULL\n");
        return;
    }
    if(nParent == NULL){
        printf("NULL\n");
        return;
    }
    nChild->nextSibling = nParent->firstChild;
    nChild->parent = nParent;
    nParent->firstChild = nChild;
}

//树操作 删除子节点
void removeChild(AuxTreeNode* nParent, AuxTreeNode* nChild) {
    if (nChild == NULL){
        printf("NULL\n");
        return;
    }
    if(nParent == NULL){
        printf("NULL\n");
        return;
    }
    if (nParent != nChild->parent)
        return;
    if (nParent->firstChild == nChild)
        nParent->firstChild = nChild->nextSibling;
    else {
        AuxTreeNode* pre = nParent->firstChild;
        while (pre->nextSibling != NULL) {
            if (pre->nextSibling == nChild) {
                pre->nextSibling = nChild->nextSibling;
                break;
            }
            pre = pre->nextSibling;
        }
    }
    nChild->nextSibling = NULL;
    nChild->parent = NULL;
}

//树操作 将子树中截至实前缀的所有节点设置为虚/半虚
void setNonrealNodesInSubtreeTo(AuxTreeNode* nNode, NodeType type) {
    if (nNode == NULL){
        printf("NULL\n");
        return;
    }
    if(nNode->type != Real) nNode->type = type;
    AuxTreeNode* ptr = nNode->firstChild;
    while (ptr != NULL) {
        if (ptr->type != Real)
            setNonrealNodesInSubtreeTo(ptr, type);
        ptr = ptr->nextSibling;
    }

}

//树操作 删除所有子节点
void removeAllChild(AuxTreeNode* nNode) {
    if(nNode == NULL){
        printf("NULL\n");
        return;
    }
    AuxTreeNode* temp;
    while (nNode->firstChild != NULL) {
        temp = nNode->firstChild->nextSibling;
        removeAllChild(nNode->firstChild);
        delete nNode->firstChild;
        nNode->firstChild = temp;
    }
}

/* constructor of struct kv */
static void init_kv(struct kv* kv)
{
    kv->next = NULL;
}

/* the classic Times33 hash function */
uint64 hash_33(const char* key)
{
    uint64 hash = 0;
    while (*key) {
        hash = (hash << 5) + hash + *key++;
    }
    return hash;
}

/* new a HashTable instance */
HashTable* hash_table_new(uint64 htSize)
{
    //htSize = htSize*3;
    HashTable* ht = (HashTable* )malloc(sizeof(HashTable));
    if (NULL == ht) {
        hash_table_delete(ht);
        return NULL;
    }
    ht->table = (kv ** )malloc(sizeof(struct kv*) * htSize);
    if (NULL == ht->table) {
        hash_table_delete(ht);
        return NULL;
    }
    memset(ht->table, NULL, sizeof(struct kv*) * htSize);

    ht->content = 0;
    ht->tableSize = htSize;
    ht->conflict = 0;

    return ht;
}
/* delete a HashTable instance */
void hash_table_delete(HashTable* ht)
{
    if (ht) {
        if (ht->table) {
            int i = 0;
            for (i = 0; i<ht->tableSize; i++) {
                struct kv* p = ht->table[i];
                struct kv* q = NULL;
                while (p) {
                    q = p->next;
                    p = q;
                }
            }
            free(ht->table);
            ht->table = NULL;
        }
        free(ht);
    }
}

char* copyString(const char *src){
    //string dst = src;
    char *dst = new char[strlen(src)+1];
    for(int i = 0; i <= strlen(src); i++){
        dst[i] = src[i];
    }
    return dst;
}


/* insert or update a value indexed by key */
int hash_table_put(HashTable* ht, string key, ForwardInfo info) //for CBF-HT
{
    //uint64 i = hash_33(key.c_str()) % ht->tableSize;
    uint64 i = CityHash64(key.c_str(), key.length()) % ht->tableSize;
    struct kv* p = ht->table[i];
    struct kv* prep = p;

    while (p) { /* if key is already stroed, update its value */
        if (strcmp(key.c_str(), p->key) == 0) {
            p->info = info;
            ht->conflict ++;
            break;
        }
        prep = p;
        p = p->next;
    }

    if (p == NULL) {/* if key has not been stored, then add it */
        struct kv * mkv = (struct kv*)malloc(sizeof(struct kv));
        if (NULL == mkv) {
            return -1;
        }
        mkv->next = NULL;
        mkv->info = info;
        //mkv->key = copyString(key.c_str());
        mkv->key = new char[key.length()+1];
        strcpy(mkv->key, key.c_str());

        if (prep == NULL) {
            ht->table[i] = mkv;
        }
        else {
            prep->next = mkv;
        }

        ht->content ++;
    }

    return 0;
}

/* insert or update a value indexed by key */
int hash_table_put2(HashTable* ht, string key, AuxTreeNode *value) //for BS
{
    //uint64 i = hash_33(key.c_str()) % ht->tableSize;
    uint64 i = CityHash64(key.c_str(), key.length()) % ht->tableSize;
    struct kv* p = ht->table[i];
    struct kv* prep = p;

    while (p) { /* if key is already stroed, update its value */
        if (strcmp(key.c_str(), p->key) == 0) {
            p->value = value;
            ht->conflict ++;
            break;
        }
        prep = p;
        p = p->next;
    }

    if (p == NULL) {/* if key has not been stored, then add it */
        struct kv * mkv = (struct kv*)malloc(sizeof(struct kv));
        if (NULL == mkv) {
            return -1;
        }
        mkv->next = NULL;
        mkv->value = new AuxTreeNode;
        mkv->value = value;
        //mkv->key = copyString(key.c_str());
        mkv->key = new char[key.length()+1];
        strcpy(mkv->key, key.c_str());

        if (prep == NULL) {
            ht->table[i] = mkv;
        }
        else {
            prep->next = mkv;
        }

        ht->content ++;
    }

    return 0;
}

/* get a value indexed by key */
kv* hash_table_get(HashTable* ht, string key)
{
    //printf("  hash_table_get Try prefix: %s\n", key.c_str());
    //uint64 i = hash_33(key.c_str()) % ht->tableSize;
    uint64 i = CityHash64(key.c_str(), key.length()) % ht->tableSize;
    struct kv* p = ht->table[i];
    while (p) {
        if (strcmp(key.c_str(), p->key) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

/* remove a value indexed by key */
void hash_table_rm(HashTable* ht, string key)
{
    //uint64 i = hash_33(key.c_str()) % ht->tableSize;
    uint64 i = CityHash64(key.c_str(), key.length()) % ht->tableSize;
    struct kv* p = ht->table[i];
    struct kv* prep = p;
    while (p) {
        if (strcmp(key.c_str(), p->key) == 0) {
            if (p == prep) {
                ht->table[i] = p->next;
            }
            else {
                prep->next = p->next;
            }
            ht->content --;
            break;
        }
        prep = p;
        p = p->next;
    }
}

const uint64 gettableSize(HashTable* ht){
    return ht->tableSize;
}
const uint64 getContentSize(HashTable* ht){
    return ht->content;
}
const uint64 getConflict(HashTable* ht){
    return ht->conflict;
}
const double gethtLF(HashTable* ht){
    ht->loadFactor = (double)ht->content/ht->tableSize;
    return ht->loadFactor;
}