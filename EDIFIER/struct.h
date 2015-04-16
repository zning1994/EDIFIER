
#include <cstring>
#include <vector>
using namespace std;

#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef unsigned short WORD;
// �洢����
struct STravel{
	// ����id
	int m_iId;
	// ����url
	char m_cUrl[128];
	// ������Ŀ
	WORD m_wTitle[32];
	// ����ʱ��
	char m_cDate[11];
	// ��������
	WORD m_wPeople[8];
	// ��������
	short int m_siDays;
	// ������ʽ
	WORD m_wType[8];
	// �������ݵĴ洢�ļ���
	char m_cFileName[16];
	// ��������
	// �����������ݱȽϴ�������Ҫ��̬�����ڴ�
	char *m_cContext;
	// ���´�С
	int m_iSize;
};

enum ETravel
{
	EN_T_ID = 0,
	EN_T_URL,
	EN_T_TITLE,
	EN_T_DATE,
	EN_T_PEOPLE,
	EN_T_DAYS,
	EN_T_TYPE,
	EN_T_CONTEXT,

};

// ��mapʹ��
struct SString
{
	char *sWord;
	SString(char *word)
	{
		this->sWord = new char[strlen(word)];
		strcpy(sWord, word);
	}
	bool operator<(const SString &sStr) const
	{
		return strcmp(this->sWord, sStr.sWord) < 0;
	}
};

// FP��ģ�����ݽṹ
// ���ڵ�
struct STreeNode
{
	char *m_sWord;
	int m_supNum;           // ֧�ֶ�
	STreeNode *m_stParent;  // ���Ӹ��ڵ� 
	STreeNode *m_stBrother; // ������һ����ͬ��Ľڵ�
	vector<STreeNode*> m_pstChildren; // �ӽڵ�

	STreeNode(char *word = "", int supNum = 0){
		this->m_sWord = new char[32];
		strcpy(m_sWord, word);
		this->m_supNum = supNum;
		m_stParent = NULL;
		m_stBrother = NULL;
	}

	int getChildrenNum(){
		return this->m_pstChildren.size();
	}
};

enum  EMapFlag
{
	E_MAP_START = 0,
	E_MAP_KEY,
	E_MAP_NEW,
};

#endif