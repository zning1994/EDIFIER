

#ifndef _SPLIT_H_
#define _SPLIT_H_

class CSplit
{
public:
	/******************************分割操作************************************/
	/* Param:
	/*	sInput: 传入数据，一段文字
	/*	cSplitSymbol: 分割的字符
	/*	words: 存储分割后的词，由函数分配内存,是一个引用,
	        （传的是char ***words的时候出现异常，仍未解决）
	/*	wordNum: 记录词的个数 
	/*  wordMaxNum: sInput中含有的单词数的保守估计，避免创建的数组过大或过小
	/*	*/
	static void split(const char *sInput, const char cSplitSymbol, char **&words, int &wordNum, int wordMaxNum = 100);
};

#endif