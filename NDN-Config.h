#ifndef H_NDN_FIB_CONFIG
#define H_NDN_FIB_CONFIG
#include <stdint.h>

//�Ƿ�ʹ��footprint����������Ϊ��ϣ���key
#define FOOTPRINT_AS_KEY_ON false

//footprint���ֽ���(���ڵ���1, С�ڵ���8)
#define FOOTPRINT_BYTE_LEN 4

//����footprint�ļ���ʱ������nonce��nameƴ�Ӻ���hash (���ڱ�֤ÿ��·����������ӳ��Ψһ ��ֹ��ײ)
#define FOOTPRINT_HASH_NONCE "aaa"

//�ɴ�������ֵ���������
#define MAX_COMP_NUM 50

//ǰ׺���������ɵ��������������
#define EXPECTATION_COMP_NUM 10

#define EXPECTATION_COMP_NUM_SHORT 4

//��ʾ�����ϸ��
#define NDN_HASH_BASED_FIB_DEBUG_ON false

#endif
