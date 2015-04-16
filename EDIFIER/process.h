
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
extern char keyOutputFile[32];   // ���key words txt
extern char testNewFile[32];     // �������
extern char testKeyFile[32];     // �������
extern char distributeNewFile[32];  // �´ʷֲ��ļ�
extern char distributeKeyFile[32];  // �´ʷֲ��ļ�
extern char blackWordsFile[32];  // ������
extern char whiteWordsFile[32];  // ������

extern const char *sResult;  // �洢�ִʽ��
extern char **words;         // �洢����
extern int wordNum;          // �洢���ʸ���

extern map<SString, int> mapNew;    // new����
extern map<SString, int> mapKey;    // key����

extern vector<vector<SString>> transRecordsNew;  // ����FP-Tree���new
extern vector<vector<SString>> transRecordsKey;  // ����FP-Tree���key
extern const int minSup;

extern fstream BLACK_WORDS;     // ������
extern fstream DISTRIBUTE_NEW;  // �´ʵķֲ�
extern fstream DISTRIBUTE_KEY;  // �ؼ��ʵķֲ�
extern fstream TEST_OUTPUT;     // �������
extern fstream WHITE_WORDS;     // ������

typedef pair<SString, int> PAIR;

// �����´�
// ���˲�����ʵ�
bool ProcessNewWords(int iTravelSize);

// ����ؼ���
bool ProcessKeyWords(int iTravelSize);

// ����ÿһ�еĹؼ���
bool ProcessParagraphKeyWords(int iTravelSize);

bool cmp_by_value(const PAIR& a, const PAIR& b);

bool cmp_by_new(const SString &a, const SString &b);

bool cmp_by_key(const SString &a, const SString &b);

#endif