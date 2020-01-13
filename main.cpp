#include <iostream>
#include <sys/time.h>
#include <unordered_map>
#include "BinarySearch.h"
#include "CBF-HT.h"
#include "PrefixGenerator.h"
using namespace std;

#define MAX 1e6   //整数范围 1 ～ MAX
#define M 1
#define base MAX/M
struct timeval	gStartTime,gEndTime;
struct timeval	startTime,endTime;


BinarySearch BS_FIB(MAX*4);
CBF_HT cbf_ht(MAX);

ForwardInfo info;
int seed = 242;
PrefixGenerator gen(seed);

typedef unordered_map<string, uint32> prefixs;
prefixs baseFIB;

vector<string> vec;
vector<string> erase_vec;

void generate_baseFIB(){
    for (uint32 i = 0; i < base; i++){
        while(1){
            string s = gen.generate_shorter_prefix();
            prefixs::iterator it = baseFIB.find(s);
            if (it == baseFIB.end()){
                baseFIB.insert(make_pair(s, i));
                break;
            }
        }
    }
    printf("\tThe actual size of baseFIB is %d\n", baseFIB.size());
    printf("\tgenerate_baseFIB finished\n\n");
}

void insert_BS_FIB(){

    string ht;
    for (int j = 0; j < M; j++) {
        for (auto& x: baseFIB) {
            //cout << " " << x.first << ":" << x.second << endl;
            //ht = x.first + "/ht" + std::to_string(j);
            ht = x.first;
            //cout << ht << endl;
            BS_FIB.insert(ht,info);
        }
    }
    printf("\tinsert_BS_FIB finished\n");
    printf("\tThe actual size of BS_FIB is %d\n",getContentSize(BS_FIB.mMap));
    std::cout<<"\tconflict num: "<<getConflict(BS_FIB.mMap)<<std::endl;
    std::cout<<"\tbucket size: "<<gettableSize(BS_FIB.mMap)<<std::endl;
    std::cout<<"\tload factor: "<<gethtLF(BS_FIB.mMap)<<std::endl;
}

void insert_CBF_HT(){

    string ht;
    for (int j = 0; j < M; j++) {
        for (auto& x: baseFIB) {
            //cout << " " << x.first << ":" << x.second << endl;
            //ht = x.first + "/ht" + std::to_string(j);
            ht = x.first;
            //cout << ht << endl;
            cbf_ht.insert(ht,info);
        }
    }
    printf("\tinsert_CBF_HT finished\n");
    printf("\tThe actual size of CBF_HT is %d\n", getContentSize(cbf_ht.mMap));
    std::cout<<"\tconflict num: "<<getConflict(cbf_ht.mMap)<<std::endl;
    std::cout<<"\tbucket size: "<<gettableSize(cbf_ht.mMap)<<std::endl;
    std::cout<<"\tload factor: "<<gethtLF(cbf_ht.mMap)<<std::endl;

}

int main()
{

    double  a1, a2, a3, a4, a5, a6, a7, a8;

    int fibSize = MAX;
    //设置名字前缀查询次数
    int operation_time = 1e6;
    printf("FIB_size: %d  query_prefix_times: %d \n", fibSize, operation_time);

    gettimeofday(&startTime,NULL);

    printf(">> Insert\n");
    generate_baseFIB();

    string getMem = "ps -aux | grep fib > base.txt";
    //cout << getMem << endl;
    system(getMem.data());

    //insert_BS_FIB
    gettimeofday(&gStartTime,NULL);
    insert_BS_FIB();
    gettimeofday(&gEndTime,NULL);
    a1 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tInsert BS_FIB Use Time: %f(ms)\n\n", a1);
    getMem = "ps -aux | grep fib > BS_FIB.txt";
    //cout << getMem << endl;
    system(getMem.data());


    //insert_CBF_HT
    gettimeofday(&gStartTime,NULL);
    insert_CBF_HT();
    gettimeofday(&gEndTime,NULL);
    a2 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tInsert CBF_HT Use Time: %f(ms)\n\n", a2);
    getMem = "ps -aux | grep fib > CBF_HT.txt";
    //cout << getMem << endl;
    system(getMem.data());

    //生成名字前缀用于查找
    printf(">> Generate Lookup prefix\n");
    gettimeofday(&gStartTime,NULL);
    int count = 0;
    string ht;
    for (auto& x: baseFIB) {
        //cout << " " << x.first << ":" << x.second << endl;
        ht = x.first + "/ht0/ht1/ht2/ht3/ht4/ht5";  //全hit
        //ht = x.first + "/hy0/ht1/ht2/ht3/ht4/ht5/ht6";  //全miss
        //ht = "/ht0/ht1/ht2/ht3/ht4/ht5/ht6/ht7/ht8/ht9";  //全miss
        //cout << ht << endl;
        vec.push_back(ht);
        ht = x.first;
        //ht = x.first + "/ht0";
        erase_vec.push_back(ht);
        count++;
        if(count == operation_time) break;
    }
    gettimeofday(&gEndTime,NULL);
    printf("\tThe actual size of Lookup vec is %d\n", vec.size());
    printf("\tThe actual size of Erase vec is %d\n", erase_vec.size());
    a3 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tGenerate Lookup prefix Use Time: %f(ms)\n\n", a3);


    printf(">> Search\n");
    //HT-tree + 普通二分查询
    BS_FIB.reset_lookup_times();
    BS_FIB.reset_no_matchs();
    gettimeofday(&gStartTime,NULL);
    for (int i = 0; i < operation_time; i++) {
        BS_FIB.LPM_search(vec[i]); //printf("\n");
    }
    gettimeofday(&gEndTime,NULL);
    a4 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tBS_FIB-total query time: %f(ms)\n", a4);
    printf("\tAverage memory access: %f\n", BS_FIB.get_lookup_times() / (double)operation_time); //平均查找次数：访存次数
    printf("\tno match count: %d\n\n", BS_FIB.get_no_matchs()); //没有匹配到名字的查找次数


    //CBF-HT + 内存优化二分查询
    cbf_ht.reset_lookup_times();
    cbf_ht.reset_no_matchs();
    gettimeofday(&gStartTime,NULL);
    for (int i = 0; i < operation_time; i++) {
        cbf_ht.LPM_search(vec[i]); //printf("\n");
    }
    gettimeofday(&gEndTime,NULL);
    a5 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tCBF_HT-total query time: %f(ms)\n", a5);
    printf("\tAverage memory access: %f\n", cbf_ht.get_lookup_times() / (double)operation_time);
    printf("\tno match count: %d\n\n", cbf_ht.get_no_matchs()); //没有匹配到名字的查找次数

    //线性查询
    BS_FIB.reset_lookup_times();
    BS_FIB.reset_no_matchs();
    gettimeofday(&gStartTime,NULL);
    for (int i = 0; i < operation_time; i++) {
        BS_FIB.linear_search(vec[i]); //printf("\n");
    }
    gettimeofday(&gEndTime,NULL);
    a6 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tLinear Search-total query time: %f(ms)\n", a6);
    printf("\tAverage memory access: %f\n", BS_FIB.get_lookup_times() / (double)operation_time);
    printf("\tno match count: %d\n\n", BS_FIB.get_no_matchs()); //没有匹配到名字的查找次数
    BS_FIB.reset_lookup_times();
    BS_FIB.reset_no_matchs();


    //线性查询
    cbf_ht.reset_lookup_times();
    cbf_ht.reset_no_matchs();
    gettimeofday(&gStartTime,NULL);
    for (int i = 0; i < operation_time; i++) {
        cbf_ht.linear_search(vec[i]); //printf("\n");
    }
    gettimeofday(&gEndTime,NULL);
    a7 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tLinear Search-total query time: %f(ms)\n", a7);
    printf("\tAverage memory access: %f\n", cbf_ht.get_lookup_times() / (double)operation_time);
    printf("\tno match count: %d\n\n", cbf_ht.get_no_matchs()); //没有匹配到名字的查找次数

    printf("\tRatio of Linear/OptimalBinary: %f, Binary/OptimalBinary:  %f\n", (a6) / (a5), (a4) / (a5));


    printf(">> Erase\n");
    operation_time = 1e2;
    //HT-tree + 普通二分查询
    gettimeofday(&gStartTime,NULL);
    for (int i = 0; i < operation_time; i++) {
        BS_FIB.erase(erase_vec[i]); //printf("\n");
    }
    gettimeofday(&gEndTime,NULL);
    a4 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tBS_FIB Erasing Use Time: %f(ms)\n", a4);


    //CBF-HT + 内存优化二分查询
    gettimeofday(&gStartTime,NULL);
    for (int i = 0; i < operation_time; i++) {
        cbf_ht.erase(erase_vec[i]); //printf("\n");
    }
    gettimeofday(&gEndTime,NULL);
    a5 = 1000*(gEndTime.tv_sec - gStartTime.tv_sec) + (double)(gEndTime.tv_usec - gStartTime.tv_usec)/1000;
    printf("\tCBF_HT Erasing Use Time: %f(ms)\n", a5);

    printf("\tRatio of BS_FIB/CBF_HT:  %f\n", (a4) / (a5));

    gettimeofday(&endTime,NULL);
    a8 = endTime.tv_sec - startTime.tv_sec + (double)(endTime.tv_usec - startTime.tv_usec)/1e6;
    printf("\tTotal Time: %f(s)\n", a8);


}