
#include "struct.h"
#include <map>

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// 游记的最大数量
const int TRAVEL_MAX_SIZE = 1000;

extern char inputFile[32];      // 文章txt
extern char keyOutputFile[32];  // 输出key words txt
extern char newOutputFile[32];  // 输出new words
extern char allOutputFile[32];  // 输出所有的words
extern char testOutputFile[32]; //测试输出


extern const char *sResult;  // 存储分词结果
extern char **words;  // 存储单词
extern int wordNum;      // 存储单词个数

extern map<SString, int> mapStr; // 使用map计算单词的个数

extern fstream BLACK_WORDS; // 黑名单
extern fstream DISTRIBUTE;  // 新词的分布
extern fstream TEST_OUTPUT; // 测试输出

#endif