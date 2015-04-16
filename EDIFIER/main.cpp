
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

bool isGetNewWords = false;  // �Ƿ��ȡ���µ���

/************************ ȫ�ֱ��� **************************/
// txt�ļ�·��
char inputFile[32];       // ����txt
char keyOutputFile[32];   // ���key words txt
char newOutputFile[32];   // ���new words
char allOutputFile[32];   // ������е�words
char blackWordsFile[32];  // ������
char whiteWordsFile[32];  // ������
char testNewFile[32];     // �������
char testKeyFile[32];     // �������
char distributeNewFile[32];  // �´ʷֲ��ļ�
char distributeKeyFile[32];  // �´ʷֲ��ļ�
char logFile[32];         // ��־
char recordsKeyFile[32];
char recordsNewFile[32];
char resultKeyFile[32];

/* --------------------�洢����---------------------- */
char **words = NULL;      // �洢����
int wordNum = 0;          // �洢���ʸ���
const char *sResult;      // �洢�ִʽ��

map<SString, int> mapNew;    // new����
map<SString, int> mapKey;    // key����

int mapFlag = E_MAP_START;

/* --------------------FP-Tree����---------------------- */
vector<vector<SString>> transRecordsNew;  // ����FP-Tree���new
vector<vector<SString>> transRecordsKey;  // ����FP-Tree���key
const int minSup = 10;                     // ��С֧�ֶ�

/* --------------------�ı��������---------------------- */
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

	/************************ ���ݿ���� **************************/
	char *sqlStr = "select * from travel";
	int iTravelSize = 0;
	CSqlite3 *pSqlite = CSqlite3::getInstance();

	pSqlite->getTable(sqlStr, a_stTravel, iTravelSize);

	/************************ �ִʲ��� **************************/
	
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
		// ���transRecords
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
	// һ��һ�зִ�
	cout << "Start process key words..." << endl;
	ProcessParagraphKeyWords(iTravelSize);
	// ���transRecords
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

	/************************ FP tree���� **************************/
	/************************
	 * 1.��ÿһƪ���µ��´�Ϊһ�������FP tree����
	 * 2.��������ÿһ�����ֵĹؼ���Ϊ�����FP tree����
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


	// �ر��ļ�
	LOG.close();
	TREE.close();
	TABLE.close();
	RESULT.close();

	NLPIR_Exit();

	return 0;
}