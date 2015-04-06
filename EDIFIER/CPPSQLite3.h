/**
* 简单的从sqlite3数据库中获取数据
* sqlite3数据库中使用的utf-8编码格式
* 而我们常用的是unicode编码
* 所以获取数据库中的数据之后需要解码
**/

#include "SQLite3\sqlite3.h"
#include "struct.h"
#include <fstream>
#include <cstring>
#include <iostream>
using namespace std;

#ifndef _CPPSQLITE3_H_
#define _CPPSQLITE3_H_

// 单例模式
class CSqlite3{

public:
	~CSqlite3();

	/* 获取单例模式 */
	static CSqlite3 *getInstance();

	/* 获取表格数据
	/* Param:
	/*	return: 数据库的访问结果
	/*  astTravel: 存储结果
	/*	iTravelSize: 存储数量 */
	int getTable(char *sqlStr, STravel *astTravel, int& iTravelSize);

	// 获取查询的row
	int getRow(){}
	// 获取查询的column
	int getColumn(){}
	// *******************访问qlite的相关操作******************* //

private:
	CSqlite3(){ init(); }
	bool init();

	// 数据库连接
	sqlite3 *m_pDB;

	// result不需要初始化
	char **m_cResult;
	int m_iRow;
	int m_iColumn;
	char *m_cErrMsg;

	// 日志输出
	fstream LOG;

};

#endif