
#include "CPPSQLite3.h"
#include "global.h"
#include "process.h"
#include "struct.h"
#include "UTF.h"
#include "split.h"
#include "FP_tree.h"
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

bool isGetNewWords = false;  // 是否获取了新单词

/************************ 全局变量 **************************/
// txt文件路径
char inputFile[32];       // 文章txt
char keyOutputFile[32];   // 输出key words txt
char newOutputFile[32];   // 输出new words
char allOutputFile[32];   // 输出所有的words
char blackWordsFile[32];  // 黑名单
char whiteWordsFile[32];  // 白名单
char testNewFile[32];     // 测试输出
char testKeyFile[32];     // 测试输出
char distributeNewFile[32];  // 新词分布文件
char distributeKeyFile[32];  // 新词分布文件
char logFile[32];         // 日志
char recordsKeyFile[32];
char recordsNewFile[32];
char resultKeyFile[32];

/* --------------------存储变量---------------------- */
char **words = NULL;      // 存储单词
int wordNum = 0;          // 存储单词个数
const char *sResult;      // 存储分词结果

map<SString, int> mapNew;    // new单词
map<SString, int> mapKey;    // key单词

int mapFlag = E_MAP_START;

/* --------------------FP-Tree变量---------------------- */
vector<vector<SString>> transRecordsNew;  // 构建FP-Tree的项集new
vector<vector<SString>> transRecordsKey;  // 构建FP-Tree的项集key
const int minSup = 10;                     // 最小支持度

/* --------------------文本输出变量---------------------- */
fstream DISTRIBUTE_KEY;
fstream DISTRIBUTE_NEW;
fstream BLACK_WORDS;
fstream WHITE_WORDS;
fstream TEST_OUTPUT;
fstream LOG;
fstream TREE;
fstream TABLE;
fstream RESULT;

int main()
{
	sprintf(blackWordsFile, "./import_list/black_words.txt");
	sprintf(whiteWordsFile, "./import_list/white_words.txt");
	sprintf(distributeNewFile, "./distribute/new.txt");
	sprintf(distributeKeyFile, "./distribute/key.txt");
	sprintf(logFile, "./log/log.txt");
	sprintf(recordsKeyFile, "./records/key.txt");
	sprintf(recordsNewFile, "./records/new.txt");
	sprintf(testNewFile, "./test/new.txt");
	sprintf(testKeyFile, "./test/key.txt");
	sprintf(resultKeyFile, "./result/key.txt");
	
	LOG.open(logFile, ios::out);
	TREE.open("./tree/tree.txt", ios::out);
	TABLE.open("./tree/table.txt", ios::out);
	RESULT.open(resultKeyFile, ios::out);

	/************************ 数据库操作 **************************/
	char *sqlStr = "select * from travel";
	int iTravelSize = 0;
	CSqlite3 *pSqlite = CSqlite3::getInstance();

	pSqlite->getTable(sqlStr, a_stTravel, iTravelSize);

	/************************ 分词操作 **************************/
	
	if (!NLPIR_Init("./", UTF8_CODE))
	{
		cout << "ERROR -- NLPIR Init fails" << endl;
		return 0;
	}

	// process new words
	if (!isGetNewWords)
	{
		cout << "Start process new words..." << endl;
		ProcessNewWords(iTravelSize);
		// 输出transRecords
		fstream RECORD_NEW(recordsNewFile, ios::out);
		for (int i = 0; i < transRecordsNew.size(); ++i)
		{
			for (int j = 0; j < transRecordsNew[i].size(); ++j)
			{
				RECORD_NEW << transRecordsNew[i][j].sWord << "\t";
			}
			RECORD_NEW << endl;
		}
		RECORD_NEW.close();
	}

    LOG << "Import white words num=" << NLPIR_ImportUserDict(whiteWordsFile, true) << endl;

	// process key words
	// 一行一行分词
	cout << "Start process key words..." << endl;
	ProcessParagraphKeyWords(iTravelSize);
	// 输出transRecords
	fstream RECORD_KEY(recordsKeyFile, ios::out);
	for (int i = 0; i < transRecordsKey.size(); ++i)
	{
		for (int j = 0; j < transRecordsKey[i].size(); ++j)
		{
			RECORD_KEY << transRecordsKey[i][j].sWord << "\t";
		}
		if (transRecordsKey[i].size() == 0)
			cout << "size is 0 -- " << i << endl;
		RECORD_KEY << endl;
	}
	RECORD_KEY.close();

	/************************ FP tree操作 **************************/
	/************************
	 * 1.以每一篇文章的新词为一个项集进行FP tree操作
	 * 2.以文章中每一段文字的关键词为项集进行FP tree操作
	 ************************/
	/*if (!isGetNewWords)
	{
		cout << "Construct new FP Tree..." << endl;
		vector<SString> postPatternNew;
		mapFlag = E_MAP_NEW;
		FPGrowth(transRecordsNew, postPatternNew);
	}*/

	cout << "Construct key FP Tree..." << endl;
	vector<SString> postPatternKey;
	mapFlag = E_MAP_KEY;
	FPGrowth(transRecordsKey, postPatternKey);


	// 关闭文件
	LOG.close();
	TREE.close();
	TABLE.close();
	RESULT.close();

	NLPIR_Exit();

	return 0;
}