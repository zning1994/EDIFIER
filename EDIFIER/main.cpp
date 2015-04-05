
#include "CPPSQLite3.h"
#include "global.h"
#include "struct.h"
#include "UTF.h"
#include "NLPIR\NLPIR.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

//#pragma comment(lib, "NLPIR.lib")

sqlite3 *pDB;

STravel a_stTravel[TRAVEL_MAX_SIZE];


int main()
{
	/************************数据库操作**************************/
	char *sqlStr = "select * from travel";
	int iTravelSize = 0;
	CSqlite3 *pSqlite = CSqlite3::getInstance();
	cerr << "get sqlite3 instance successed..." << endl;

	pSqlite->getTable(sqlStr, a_stTravel, iTravelSize);

	cerr << "get sqlite3 table successed..." << endl;

	/************************分词操作**************************/

	const char *sResult;
	if (!NLPIR_Init("./", UTF8_CODE))
	{
		cout << "ERROR -- NLPIR Init fails" << endl;
		return 0;
	}
	sResult = NLPIR_GetFileKeyWords("./context/context0.txt");
	fstream KEY_WORDS;
	KEY_WORDS.open("./result/key_words.txt", ios::out);
	KEY_WORDS << sResult << endl;
	KEY_WORDS.close();

	/*char sSentence[2000];
    const char * sResult;
	if(!NLPIR_Init())
	{
		printf("Init fails\n");
		return 0;
	}
	printf("Input sentence now('q' to quit)!\n");
	scanf("%s",sSentence);
	while(_stricmp(sSentence,"q")!=0)
	{
		sResult = NLPIR_GetNewWords(sSentence);
		printf("%s\nInput string now('q' to quit)!\n", sResult);
		scanf("%s",sSentence);
	}
	NLPIR_Exit();*/

	/*wchar_t wStr[] = L"这里是一段中文";
	wcout << wStr << endl;*/

	return 0;
}