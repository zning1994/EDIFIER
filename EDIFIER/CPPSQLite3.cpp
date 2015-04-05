
#include "CPPSQLite3.h"


CSqlite3::~CSqlite3()
{
	sqlite3_free_table(m_cResult);
	sqlite3_close(m_pDB);

	this->LOG.close();
}

CSqlite3 *CSqlite3::getInstance()
{
	static CSqlite3 instance;
	return &instance;
}

bool CSqlite3::init()
{
	int iResult = sqlite3_open("travels.db", &this->m_pDB);
	if (SQLITE_OK != iResult)
	{
		cerr << "ERROR -- open sqlite db failed!" << endl;
		return false;
	}

	m_iRow = 0;
	m_iColumn = 0;

	const char *logName = "./log/SQLite3.log";
	this->LOG.open(logName, ios::out);

	return true;
}

int CSqlite3::getTable(char *sqlStr, STravel *astTravel, int& iTravelSize)
{
	cerr << "start get table..." << endl;
	iTravelSize = 0;
	int iResult = sqlite3_get_table(this->m_pDB, sqlStr, &m_cResult, &m_iRow, &m_iColumn, &m_cErrMsg);
	if (SQLITE_OK == iResult)
	{
		cerr << "start transport data..." << endl;
		int index = m_iColumn; // result的第一行是字段名，从column索引开始才是真正的数据
		// 遍历result，将数据转移到astTravel中

		// *******************************开始转移数据******************************************** //
		
		for (int rIndex = 1; rIndex < m_iRow; ++rIndex) // 从第一行开始遍历数据
		{
			cout << rIndex << ">>>" << m_iRow << ">>>" << iTravelSize << endl;

			sprintf(astTravel[iTravelSize].m_cFileName, "./context/context%d.txt", iTravelSize);
			fstream OUT;
			OUT.open(astTravel[iTravelSize].m_cFileName, ios::out);

			for (int cIndex = 0; cIndex < m_iColumn; ++ cIndex) // 一行数据
			{
				//cerr << m_cResult[cIndex] << endl;
				if (strcmp(m_cResult[cIndex], "id") == 0)
				{
					astTravel[iTravelSize].m_iId = atoi(m_cResult[rIndex * m_iColumn + cIndex]);
					LOG << rIndex << " -- " << cIndex << " -- id: " << astTravel[iTravelSize].m_iId << endl;
				}
				else if(strcmp(m_cResult[cIndex], "url") == 0)
				{
					strncpy(astTravel[iTravelSize].m_cUrl, m_cResult[rIndex * m_iColumn + cIndex], 128);
					LOG << rIndex << " -- " << cIndex << " -- url: " << astTravel[iTravelSize].m_cUrl << endl;
				}
				else if(strcmp(m_cResult[cIndex], "title") == 0)
				{
					memcpy((char*)&astTravel[iTravelSize].m_wTitle, m_cResult[rIndex * m_iColumn + cIndex], 32 * sizeof(WORD));
					LOG << rIndex << " -- " << cIndex << " -- title: " << astTravel[iTravelSize].m_wTitle << endl;
				}
				else if(strcmp(m_cResult[cIndex], "date") == 0)
				{
					strncpy(astTravel[iTravelSize].m_cDate, m_cResult[rIndex * m_iColumn + cIndex], 11);
					LOG << rIndex << " -- " << cIndex << " -- date: " << astTravel[iTravelSize].m_cDate << endl;
				}
				else if(strcmp(m_cResult[cIndex], "people") == 0)
				{
					memcpy((char*)&astTravel[iTravelSize].m_wPeople, m_cResult[rIndex * m_iColumn + cIndex], 8 * sizeof(WORD));
					LOG << rIndex << " -- " << cIndex << " -- people: " << astTravel[iTravelSize].m_wPeople << endl;
				}
				else if(strcmp(m_cResult[cIndex], "days") == 0)
				{
					astTravel[iTravelSize].m_siDays = atoi(m_cResult[rIndex * m_iColumn + cIndex]);
					LOG << rIndex << " -- " << cIndex << " -- days: " << astTravel[iTravelSize].m_siDays << endl;
				}
				else if(strcmp(m_cResult[cIndex], "type") == 0)
				{
					memcpy((char*)&astTravel[iTravelSize].m_wType, m_cResult[rIndex * m_iColumn + cIndex], 8 * sizeof(WORD));
					LOG << rIndex << " -- " << cIndex << " -- type: " << astTravel[iTravelSize].m_wType << endl;
				}
				else if(strcmp(m_cResult[cIndex], "context") == 0)
				{
					LOG << rIndex << " -- " << cIndex  << " -- size: " << strlen(m_cResult[rIndex * m_iColumn + cIndex]) << endl;
					LOG << rIndex << " -- " << cIndex << " -- file: " << astTravel[iTravelSize].m_cFileName << endl;
					astTravel[iTravelSize].m_iSize = strlen(m_cResult[rIndex * m_iColumn + cIndex]);
					OUT << m_cResult[rIndex * m_iColumn + cIndex] << endl;
					
					LOG << rIndex << " -- " << cIndex << endl << "------------------分割线-----------------" << endl;
				}
				else
					continue;
			}
			++iTravelSize;
			OUT.close();
		}
	}
	
	return iResult;
}