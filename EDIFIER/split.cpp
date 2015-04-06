
#include "split.h"
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;

// �ú���������һ���ַ��������ȡ����Ŀռ�
// ����һ��char*���������
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
	unsigned int wordLen = 32; // Ϊÿ�����ʷ���ĳ���
	unsigned int preSymbol = 0; //��¼��һ���ָ����֮���λ��
	int wordMaxNum = 50; // ����洢���ʵĿռ�
	const int wordBaseNum = 50; // ����洢���ʵĿռ䲻��ʱ�����ӿռ������

	// ����һ���洢
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
			// �жϸ÷����Ƿ����ٱ�preSymbol��1
			if (i <= preSymbol)
				continue;
			
			// ���words�ռ䲻�������·����ڴ�
			if (wordNum >= wordMaxNum)
			{
				getMoreSpace(words, wordMaxNum, wordLen, wordMaxNum + wordBaseNum, wordLen);
				wordMaxNum += wordBaseNum;
			}

			// ��ÿ�����ʷ���words��
			strncpy(words[wordNum++], 
				&sInput[preSymbol], 
				(i - preSymbol) < wordLen ? (i - preSymbol) : wordLen);

			preSymbol = i + 1;
		}
	}
	
	return;
}