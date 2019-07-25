#ifndef H_NDN_FIB_CONFIG
#define H_NDN_FIB_CONFIG
#include <stdint.h>

//是否使用footprint代替名字作为哈希表的key
#define FOOTPRINT_AS_KEY_ON false

//footprint的字节数(大于等于1, 小于等于8)
#define FOOTPRINT_BYTE_LEN 4

//进行footprint的计算时，将该nonce和name拼接后再hash (用于保证每个路由器产生的映射唯一 防止碰撞)
#define FOOTPRINT_HASH_NONCE "aaa"

//可处理的名字的最大组件数
#define MAX_COMP_NUM 50

//前缀生成器生成的名字期望组件数
#define EXPECTATION_COMP_NUM 10

#define EXPECTATION_COMP_NUM_SHORT 4

//显示表操作细节
#define NDN_HASH_BASED_FIB_DEBUG_ON false

#endif
