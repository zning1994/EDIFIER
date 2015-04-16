
#include "struct.h"
#include <map>

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// �μǵ��������
const int TRAVEL_MAX_SIZE = 1000;

extern char inputFile[32];      // ����txt
extern char keyOutputFile[32];  // ���key words txt
extern char newOutputFile[32];  // ���new words
extern char allOutputFile[32];  // ������е�words
extern char testOutputFile[32]; //�������


extern const char *sResult;  // �洢�ִʽ��
extern char **words;  // �洢����
extern int wordNum;      // �洢���ʸ���

extern map<SString, int> mapStr; // ʹ��map���㵥�ʵĸ���

extern fstream BLACK_WORDS; // ������
extern fstream DISTRIBUTE;  // �´ʵķֲ�
extern fstream TEST_OUTPUT; // �������

#endif