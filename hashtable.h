#pragma once

#include "city-hash.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;
typedef struct HashTable HashTable;

//�ýڵ�����ԣ�ʵ�����飬��
enum NodeType { Real, SemiVirtual, Virtual };

struct AuxTreeNode {
        //ת����Ϣ
        ForwardInfo info;
        //�ڵ�����
        NodeType type;

        //��ָ�룺���ڵ㣬�ֵܽڵ㣬�ӽڵ�
        AuxTreeNode* parent;
        AuxTreeNode* nextSibling;
        AuxTreeNode* firstChild;

        AuxTreeNode():
                type(Virtual), parent(NULL), nextSibling(NULL),
                firstChild(NULL), info(NULL) {}
        ~AuxTreeNode(){}

};

//������
void addChild(AuxTreeNode* nParent, AuxTreeNode* nChild);
void removeChild(AuxTreeNode* nParent, AuxTreeNode* nChild);
void setNonrealNodesInSubtreeTo(AuxTreeNode* nNode, NodeType type);
void removeAllChild(AuxTreeNode* nNode);


/* element of the hash table's chain list */
struct kv
{
    //string key;
    char *key;

    //string value;
    ForwardInfo info; //for CBF-HT
    AuxTreeNode *value; //for BS

    struct kv* next;
};


/* HashTable */
struct HashTable
{
    uint64 tableSize; //�����С
    uint64 content; //ʵ�ʴ�С
    uint64 conflict;
    double loadFactor;
    struct kv ** table;
};



/* new an instance of HashTable */
HashTable* hash_table_new(uint64 htSize);

/*
delete an instance of HashTable,
all values are removed auotmatically.
*/
void hash_table_delete(HashTable* ht);

/*
add or update a value to ht,
free_value(if not NULL) is called automatically when the value is removed.
return 0 if success, -1 if error occurred.
*/
int hash_table_put(HashTable* ht, string key, ForwardInfo info);
int hash_table_put2(HashTable* ht, string key, AuxTreeNode *value);

/* get a value indexed by key, return NULL if not found. */
kv* hash_table_get(HashTable* ht, string key);

/* remove a value indexed by key */
void hash_table_rm(HashTable* ht, string key);

const uint64 gettableSize(HashTable* ht);
const uint64 getContentSize(HashTable* ht);
const uint64 getConflict(HashTable* ht);
const double gethtLF(HashTable* ht);


