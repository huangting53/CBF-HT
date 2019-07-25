#include "PrefixGenerator.h"

string PrefixGenerator::generate_prefix() {
    int comp_num = ranCompNum(*generator) + 1;
    if (comp_num >= MAX_COMP_NUM) { comp_num = MAX_COMP_NUM; }
    prefix_num_sum += comp_num;
    string res(comp_num * (2 + compDigit), '/');
    for (int i = 0; i < comp_num; i++) {
        res[i * (2 + compDigit) + 1] = 's';
        int comp_ID = ranCompID(*generator);
        for (int j = 0; j < compDigit; j++) {
            res[(i + 1) * (2 + compDigit) - 1 - j] = '0' + (comp_ID % 10);
            comp_ID /= 10;
        }
    }
    return res;
}

string PrefixGenerator::generate_shorter_prefix() {
    int comp_num = ranCompNum(*generator) + 1;
    if (comp_num >= MAX_COMP_NUM) { comp_num = MAX_COMP_NUM; }
    int prefix_num = ranCompNum2(*generator2) + 1;
    if (prefix_num >= comp_num) { prefix_num = comp_num; }
    prefix_num_sum += prefix_num;
    string res(prefix_num * (2 + compDigit), '/');
    for (int i = 0; i < comp_num; i++) {
        int comp_ID = ranCompID(*generator);
        if (i < prefix_num) {
            res[i * (2 + compDigit) + 1] = 's';
            for (int j = 0; j < compDigit; j++) {
                res[(i + 1) * (2 + compDigit) - 1 - j] = '0' + (comp_ID % 10);
                comp_ID /= 10;
            }
        }
    }
    return res;
}
