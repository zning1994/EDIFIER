
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

	/************************���ݿ����**************************/
	char *sqlStr = "select * from travel";
	int iTravelSize = 0;
	CSqlite3 *pSqlite = CSqlite3::getInstance();
	LOG << "get sqlite3 instance successed..." << endl;

	pSqlite->getTable(sqlStr, a_stTravel, iTravelSize);

	LOG << "get sqlite3 table successed..." << endl;

	/************************�ִʲ���**************************/
	const char *sResult;
	if (!NLPIR_Init("./", UTF8_CODE))
	{
		cout << "ERROR -- NLPIR Init fails" << endl;
		return 0;
	}
	char inputFile[32]; // ����txt
	char outputFile[32]; // ���key words txt
	// for test
	//iTravelSize = 1;

	// ��ʼ��ÿһƪ���½��в���
	for (int i = 0; i < iTravelSize; ++i)
	{
		sprintf(inputFile, "./context/context%d.txt", i);
		sprintf(outputFile, "./result/key_words%d.txt", i);

		// �ִ�
		sResult = NLPIR_GetFileKeyWords(inputFile);

		char **words = NULL;
		int wordNum = 0;

		// ��ʼ�ָ�
		LOG << "Start split..." << endl;
		CSplit::split(sResult, "#", words, wordNum);
		LOG << "End split..." << endl << wordNum << endl;

		/* ------------------�ָ���-------------------- */

		// �ָ�󣬶�ÿ���ʽ��д���
		fstream KEY_WORDS;
		KEY_WORDS.open(outputFile, ios::out);
		KEY_WORDS << wordNum << endl;
		for (int j = 0; j < wordNum; ++j)
		{
			KEY_WORDS << words[j] << endl;
			delete[] words[j];
		}
		KEY_WORDS.close();
		
		// �ͷ�words����Ŀռ�
		delete[] words;
	}

	LOG.close();

	return 0;
}