#ifndef H_NDN_PREFIX_GENERATOR
#define H_NDN_PREFIX_GENERATOR

#include "NDN-Config.h"
#include <random>

#define COMP_TYPE_NUM 20000000

using namespace std;

class PrefixGenerator {
private:
    default_random_engine* generator;
    default_random_engine* generator2;
    poisson_distribution<int> ranCompNum;
    poisson_distribution<int> ranCompNum2;
    uniform_int_distribution<int> ranCompID;
    int compDigit;
public:
    PrefixGenerator(int seed = 1):
            ranCompNum(EXPECTATION_COMP_NUM - 1),
            ranCompNum2(EXPECTATION_COMP_NUM_SHORT - 1),
            ranCompID(0, COMP_TYPE_NUM - 1),
            compDigit(0) {
        generator = new default_random_engine(seed);
        generator2 = new default_random_engine(seed);
        int temp = COMP_TYPE_NUM;
        while (temp > 0) {
            temp /= 10; compDigit++;
        }
    }
    ~PrefixGenerator() { delete generator; }
    //随机生成一个名字
    string generate_prefix();
    string generate_shorter_prefix();

    void set_seed(int seed) {
        delete generator;
        generator = new default_random_engine(seed);
        delete generator2;
        generator2 = new default_random_engine(seed);
    }

    long long prefix_num_sum;
};

#endif // !H_NDN_PREFIX_GENERATOR
