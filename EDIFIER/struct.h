
#include <cstring>
#include <vector>
using namespace std;

#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef unsigned short WORD;
// 存储文章
struct STravel{
	// 文章id
	int m_iId;
	// 文章url
	char m_cUrl[128];
	// 文章题目
	WORD m_wTitle[32];
	// 文章时间
	char m_cDate[11];
	// 旅游人数
	WORD m_wPeople[8];
	// 旅游天数
	short int m_siDays;
	// 旅游形式
	WORD m_wType[8];
	// 文章内容的存储文件名
	char m_cFileName[16];
	// 文章内容
	// 由于文章内容比较大，所以需要动态分配内存
	char *m_cContext;
	// 文章大小
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

// 供map使用
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

// FP建模的数据结构
// 树节点
struct STreeNode
{
	char *m_sWord;
	int m_supNum;           // 支持度
	STreeNode *m_stParent;  // 链接父节点 
	STreeNode *m_stBrother; // 链接下一个相同项的节点
	vector<STreeNode*> m_pstChildren; // 子节点

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