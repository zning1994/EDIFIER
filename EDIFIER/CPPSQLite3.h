/**
* �򵥵Ĵ�sqlite3���ݿ��л�ȡ����
* sqlite3���ݿ���ʹ�õ�utf-8�����ʽ
* �����ǳ��õ���unicode����
* ���Ի�ȡ���ݿ��е�����֮����Ҫ����
**/

#include "SQLite3\sqlite3.h"
#include "struct.h"
#include <fstream>
#include <cstring>
#include <iostream>
using namespace std;

#ifndef _CPPSQLITE3_H_
#define _CPPSQLITE3_H_

// ����ģʽ
class CSqlite3{

public:
	~CSqlite3();

	/* ��ȡ����ģʽ */
	static CSqlite3 *getInstance();

	/* ��ȡ�������
	/* Param:
	/*	return: ���ݿ�ķ��ʽ��
	/*  astTravel: �洢���
	/*	iTravelSize: �洢���� */
	int getTable(char *sqlStr, STravel *astTravel, int& iTravelSize);

	// ��ȡ��ѯ��row
	int getRow(){}
	// ��ȡ��ѯ��column
	int getColumn(){}
	// *******************����qlite����ز���******************* //

private:
	CSqlite3(){ init(); }
	bool init();

	// ���ݿ�����
	sqlite3 *m_pDB;

	// result����Ҫ��ʼ��
	char **m_cResult;
	int m_iRow;
	int m_iColumn;
	char *m_cErrMsg;

	// ��־���
	fstream LOG;

};

#endif