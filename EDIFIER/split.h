

#ifndef _SPLIT_H_
#define _SPLIT_H_

class CSplit
{
public:
	/******************************�ָ����************************************/
	/* Param:
	/*	sInput: �������ݣ�һ������
	/*	cSplitSymbol: �ָ���ַ�
	/*	words: �洢�ָ��Ĵʣ��ɺ��������ڴ�,��һ������,
	        ��������char ***words��ʱ������쳣����δ�����
	/*	wordNum: ��¼�ʵĸ��� 
	/*	*/
	static void split(const char *sInput, const char *cSplitSymbol, char **&words, int &wordNum);
};

#endif