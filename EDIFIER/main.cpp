
#include "CPPSQLite3.h"
#include "global.h"
#include "struct.h"
#include "UTF.h"
#include "split.h"
#include "NLPIR\NLPIR.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
using namespace std;

//#pragma comment(lib, "NLPIR.lib")

sqlite3 *pDB;

STravel a_stTravel[TRAVEL_MAX_SIZE];


int main()
{
	fstream LOG;
	LOG.open("./log/main.txt", ios::out);

	/************************数据库操作**************************/
	char *sqlStr = "select * from travel";
	int iTravelSize = 0;
	CSqlite3 *pSqlite = CSqlite3::getInstance();
	LOG << "get sqlite3 instance successed..." << endl;

	pSqlite->getTable(sqlStr, a_stTravel, iTravelSize);

	LOG << "get sqlite3 table successed..." << endl;

	/************************分词操作**************************/
	const char *sResult;
	if (!NLPIR_Init("./", UTF8_CODE))
	{
		cout << "ERROR -- NLPIR Init fails" << endl;
		return 0;
	}
	char inputFile[32]; // 文章txt
	char outputFile[32]; // 输出key words txt
	// for test
	//iTravelSize = 1;

	// 开始对每一篇文章进行操作
	for (int i = 0; i < iTravelSize; ++i)
	{
		sprintf(inputFile, "./context/context%d.txt", i);
		sprintf(outputFile, "./result/key_words%d.txt", i);

		// 分词
		sResult = NLPIR_GetFileKeyWords(inputFile);

		char **words = NULL;
		int wordNum = 0;

		// 开始分割
		LOG << "Start split..." << endl;
		CSplit::split(sResult, "#", words, wordNum);
		LOG << "End split..." << endl << wordNum << endl;

		/* ------------------分割线-------------------- */

		// 分割后，对每个词进行处理
		fstream KEY_WORDS;
		KEY_WORDS.open(outputFile, ios::out);
		KEY_WORDS << wordNum << endl;
		for (int j = 0; j < wordNum; ++j)
		{
			KEY_WORDS << words[j] << endl;
			delete[] words[j];
		}
		KEY_WORDS.close();
		
		// 释放words分配的空间
		delete[] words;
	}

	LOG.close();

	return 0;
}