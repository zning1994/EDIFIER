
#include "NLPIR\NLPIR.h"
#include "split.h"
#include "struct.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <algorithm>
#include <vector>
#include <sstream>
using namespace std;

#ifndef _PROCESS_H_
#define _PROCESS_H_

extern char inputFile[32];
extern char newOutputFile[32];
extern char keyOutputFile[32];   // 输出key words txt
extern char testNewFile[32];     // 测试输出
extern char testKeyFile[32];     // 测试输出
extern char distributeNewFile[32];  // 新词分布文件
extern char distributeKeyFile[32];  // 新词分布文件
extern char blackWordsFile[32];  // 黑名单
extern char whiteWordsFile[32];  // 白名单

extern const char *sResult;  // 存储分词结果
extern char **words;         // 存储单词
extern int wordNum;          // 存储单词个数

extern map<SString, int> mapNew;    // new单词
extern map<SString, int> mapKey;    // key单词

extern vector<vector<SString>> transRecordsNew;  // 构建FP-Tree的项集new
extern vector<vector<SString>> transRecordsKey;  // 构建FP-Tree的项集key
extern const int minSup;

extern fstream BLACK_WORDS;     // 黑名单
extern fstream DISTRIBUTE_NEW;  // 新词的分布
extern fstream DISTRIBUTE_KEY;  // 关键词的分布
extern fstream TEST_OUTPUT;     // 测试输出
extern fstream WHITE_WORDS;     // 白名单

typedef pair<SString, int> PAIR;

// 处理新词
// 过滤并存入词典
bool ProcessNewWords(int iTravelSize);

// 处理关键词
bool ProcessKeyWords(int iTravelSize);

// 处理每一行的关键词
bool ProcessParagraphKeyWords(int iTravelSize);

bool cmp_by_value(const PAIR& a, const PAIR& b);

bool cmp_by_new(const SString &a, const SString &b);

bool cmp_by_key(const SString &a, const SString &b);

#endif