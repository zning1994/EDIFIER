
#include "split.h"
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;

// 该函数用来帮一个字符串数组获取更大的空间
// 传入一个char*数组的引用
bool getMoreSpace(char **&sMatrix, const int preRowSize, const int preColumnSize, const int curRowSize, int curColumnSize = 32)
{
	if (curRowSize <= preRowSize || curColumnSize < preColumnSize)
		return false;

	char **tmpMatrix = NULL;

	tmpMatrix = new char* [curRowSize];

	for (int i = 0; i < curRowSize; ++i)
	{
		tmpMatrix[i] = new char[curColumnSize];
		memset(tmpMatrix[i], '\0', curColumnSize);
	}

	for (int i = 0; i < preRowSize; ++i)
	{
		strncpy(tmpMatrix[i], sMatrix[i], preColumnSize);
		delete[] sMatrix[i];
	}
	delete[] sMatrix;

	sMatrix = tmpMatrix;
	tmpMatrix = NULL;

	return true;
}

void CSplit::split(const char *sInput, const char *cSplitSymbol, char **&words, int &wordNum)
{

	wordNum = 0;
	words = NULL;
	//char **tmpWords = NULL;
	unsigned int wordLen = 32; // 为每个单词分配的长度
	unsigned int preSymbol = 0; //记录上一个分割符号之后的位置
	int wordMaxNum = 50; // 分配存储单词的空间
	const int wordBaseNum = 50; // 分配存储单词的空间不够时再增加空间的数量

	// 分配一个存储
	words = new char* [wordMaxNum];

	for (int i = 0; i < wordMaxNum; ++i)
	{
		words[i] = new char[wordLen];
		memset(words[i], '\0', wordLen * sizeof(char));
	}

	for (unsigned int i = 0; i < strlen(sInput); ++i)
	{
		if (sInput[i] == '#')
		{
			// 判断该符号是否至少比preSymbol大1
			if (i <= preSymbol)
				continue;
			
			// 如果words空间不够，重新分配内存
			if (wordNum >= wordMaxNum)
			{
				getMoreSpace(words, wordMaxNum, wordLen, wordMaxNum + wordBaseNum, wordLen);
				wordMaxNum += wordBaseNum;
			}

			// 将每个单词放入words中
			strncpy(words[wordNum++], 
				&sInput[preSymbol], 
				(i - preSymbol) < wordLen ? (i - preSymbol) : wordLen);

			preSymbol = i + 1;
		}
	}
	
	return;
}