
#include "process.h"


bool ProcessNewWords(int iTravelSize)
{
	BLACK_WORDS.open(blackWordsFile, ios::out);
	WHITE_WORDS.open(whiteWordsFile, ios::out);
	DISTRIBUTE_NEW.open(distributeNewFile, ios::out);
	TEST_OUTPUT.open(testNewFile, ios::out);
	// 构建FP-Tree所需要的数据
	vector<vector<SString>> tmpTransRecords;

	// 开始对每一篇文章进行操作
	for (int i = 0; i < iTravelSize; ++i)
	{
		cout << i << endl;
		sprintf(inputFile, "./context/context%d.txt", i);
		sprintf(newOutputFile, "./result_new/new_words%d.txt", i);

		/* ------------------------- 分割线 ----------------------------- */
		// 分词，获取关键词，包括词性
		sResult = NLPIR_GetFileNewWords(inputFile, 50, true);

		// 开始分割
		CSplit::split(sResult, '#', words, wordNum, 30);

		// 分割后，对每个词进行处理，过滤
		// 临时项集存储变量
		vector<SString> tmpSet;
		for (int j = 0; j < wordNum; ++j)
		{
			// 输出到TEST
			TEST_OUTPUT << words[j] << endl;

			// 将每个词的单词与词性分割开来
			char **subWords = NULL;
			int subWordsNum = 0;
			CSplit::split(words[j], '/', subWords, subWordsNum, 4);
			if (subWords[1][0] != 'n') // 如果是动词，则过滤掉
			{
				continue;
			}

			// 用map存储数据
			mapNew[SString(subWords[0])] += 1; // (atoi(subWords[2]) > 0) ? atoi(subWords[2]) : 1;

			// 更新项集
			tmpSet.push_back(SString(subWords[0]));

			// 释放空间
			delete[] words[j];
			for (int wordIndex = 0; wordIndex < subWordsNum; ++wordIndex)
			{
				delete[] subWords[wordIndex];
			}
			delete[] subWords;
		}
		// 更新transRecords
		tmpTransRecords.push_back(tmpSet);
		
		// 释放words分配的空间
		delete[] words;
	}
	/* ------------------------- 分割线 ----------------------------- */
	// 过滤tmpTransRecords到transRecordsNew
	for (int i = 0; i < tmpTransRecords.size(); ++i)
	{
		vector<SString> tmpRecords;
		for (int j = 0; j < tmpTransRecords[i].size(); ++j)
		{
			if (mapNew[tmpTransRecords[i][j]] < minSup)
				continue;
			tmpRecords.push_back(tmpTransRecords[i][j]);
		}
		// 对tmpRecords进行排序
		sort(tmpRecords.begin(), tmpRecords.end(), cmp_by_new);
		// 将排好序的vector<SString>压如transRecordsNew
		if (tmpRecords.size() >= 2)
			transRecordsNew.push_back(tmpRecords);
	}


	/* ------------------------- 分割线 ----------------------------- */
	// map排序输出，效果特别好
	vector<PAIR> vecStr(mapNew.begin(), mapNew.end());
	sort(vecStr.begin(), vecStr.end(), cmp_by_value);
	for (int i = 0; i < vecStr.size(); ++i)
	{
		DISTRIBUTE_NEW << vecStr[i].first.sWord << " -- " << vecStr[i].second << endl;
	}

	/* ------------------------- 分割线 ----------------------------- */
	// 输出map的内容，并将新词过滤加入词典
	for (map<SString, int>::iterator iter = mapNew.begin(); iter != mapNew.end(); iter++)
	{
		// 词过滤
		if (iter->second < 5)
			continue;
		WHITE_WORDS << iter->first.sWord << endl;
	}

	/* ------------------------- 分割线 ----------------------------- */

	// 释放map
	//mapStr.clear();
	vecStr.clear();

	// 关闭文件
	BLACK_WORDS.close();
	WHITE_WORDS.close();
	DISTRIBUTE_NEW.close();
	TEST_OUTPUT.close();

	return true;
}

bool ProcessKeyWords(int iTravelSize)
{
	DISTRIBUTE_KEY.open(distributeKeyFile, ios::out);
	// 开始对每一篇文章进行操作
	for (int i = 0; i < iTravelSize; ++i)
	{
		cout << i << endl;
		sprintf(inputFile, "./context/context%d.txt", i);
		sprintf(keyOutputFile, "./result_key/key_words%d.txt", i);

		/* ------------------------- 分割线 ----------------------------- */
		// GetFileKeyWords
		// 分词，获取关键词，包括词性
		sResult = NLPIR_GetFileKeyWords(inputFile, 50, true);

		// 开始分割
		CSplit::split(sResult, '#', words, wordNum, 30);

		// 分割后，对每个词进行处理，过滤
		fstream KEY_WORDS;
		KEY_WORDS.open(keyOutputFile, ios::out);
		KEY_WORDS << wordNum << endl;
		sprintf(testKeyFile, "./test_key/test%d.txt", i);
		TEST_OUTPUT.open(testKeyFile, ios::out);
		for (int j = 0; j < wordNum; ++j)
		{
			// 输出到TEST
			TEST_OUTPUT << words[j] << endl;

			// 将每个词的单词与词性分割开来
			char **subWords = NULL;
			int subWordsNum = 0;
			CSplit::split(words[j], '/', subWords, subWordsNum, 4);
			if (subWords[1][0] != 'n') // 如果是动词，则过滤掉
			{
				continue;
			}

			// 输出到文本
			KEY_WORDS << subWords[0] << endl;

			// 用map存储数据，出现次数也加进来
			mapKey[SString(subWords[0])] += atoi(subWords[2]);

			// 释放空间
			delete[] words[j];
			for (int wordIndex = 0; wordIndex < subWordsNum; ++wordIndex)
			{
				delete[] subWords[wordIndex];
			}
			delete[] subWords;
		}
		KEY_WORDS.close();
		TEST_OUTPUT.close();
		
		// 释放words分配的空间
		delete[] words;
	}

	/* ------------------------- 分割线 ----------------------------- */
	// 对map进行排序
	vector<PAIR> vecStr(mapKey.begin(), mapKey.end());
	sort(vecStr.begin(), vecStr.end(), cmp_by_value);
	for (int i = 0; i < vecStr.size(); ++i)
	{
		DISTRIBUTE_KEY << vecStr[i].first.sWord << " -- " << vecStr[i].second << endl;
	}

	/* ------------------------- 分割线 ----------------------------- */

	// 输出map的内容，并将新词过滤加入词典
	// 不同频率的词输出到不同的文件
	fstream DISTRIBUTE100("./result/distribute_100.txt", ios::out);
	fstream DISTRIBUTE50("./result/distribute_50.txt", ios::out);
	fstream DISTRIBUTE30("./result/distribute_30.txt", ios::out);
	fstream DISTRIBUTE10("./result/distribute_10.txt", ios::out);
	fstream DISTRIBUTE5("./result/distribute_5.txt", ios::out);
	for (map<SString, int>::iterator iter = mapKey.begin(); iter != mapKey.end(); iter++)
	{
		// 词过滤
		if (iter->second < 5)
		{
			DISTRIBUTE5 << iter->first.sWord << " -- " << iter->second << endl;
			continue;
		}
		if (iter->second >= 5 && iter->second < 10)
		{
			DISTRIBUTE10 << iter->first.sWord << " -- " << iter->second << endl;
			continue;
		}
		if (iter->second >= 10 && iter->second < 30)
		{
			DISTRIBUTE30 << iter->first.sWord << " -- " << iter->second << endl;
			continue;
		}
		if (iter->second >= 30 && iter->second < 50)
		{
			DISTRIBUTE50 << iter->first.sWord << " -- " << iter->second << endl;
			continue;
		}
		if (iter->second >= 50)
		{
			DISTRIBUTE100 << iter->first.sWord << " -- " << iter->second << endl;
			continue;
		}
	}

	/* ------------------------- 分割线 ----------------------------- */

	// 释放map
	mapKey.clear();
	vecStr.clear();

	// 关闭文件
	DISTRIBUTE_KEY.close();
	DISTRIBUTE100.close();
	DISTRIBUTE50.close();
	DISTRIBUTE30.close();
	DISTRIBUTE10.close();
	DISTRIBUTE5.close();

	return true;
}

bool ProcessParagraphKeyWords(int iTravelSize)
{
	DISTRIBUTE_KEY.open(distributeKeyFile, ios::out);
	TEST_OUTPUT.open(testKeyFile, ios::out);
	fstream OUTPUT("./processed/key.txt", ios::out);

	string buffer;

	// 构建FP-Tree所需要的数据
	vector<vector<SString>> tmpTransRecords;
	// 开始对每一篇文章进行操作
	for (int i = 0; i < iTravelSize; ++i)
	{
		cout << i << endl;
		sprintf(inputFile, "./context/context%d.txt", i);

		/* ------------------------- 分割线 ----------------------------- */
		// GetKeyWords
		// 对文章的每一段落进行词性分析
		
		fstream INPUT(inputFile, ios::in);
		while (getline(INPUT, buffer))
		{
			if (buffer == "")
				continue;
			vector<SString> tmpSet;
			const char* tmpResult = NLPIR_GetKeyWords(buffer.c_str(), 20, true);
			OUTPUT << tmpResult << endl;
			// 对结果进行分割
			CSplit::split(tmpResult, '#', words, wordNum, 50);
			for (int j = 0; j < wordNum; ++j)
			{
				// 输出TEST、
				TEST_OUTPUT << words[j] << endl;

				// 对每一个词进行分割
				char **subWords = NULL;
				int subWordsNum = 0;
				CSplit::split(words[j], '/', subWords, subWordsNum, 5);
				if (subWords[1][0] != 'n')
					continue;
				tmpSet.push_back(SString(subWords[0]));
				mapKey[subWords[0]]++;

				// 释放空间
				for (int wordIndex = 0; wordIndex < subWordsNum; ++wordIndex)
					delete[] subWords[wordIndex];
				delete[] subWords;
			}
			// 压入临时变量
			tmpTransRecords.push_back(tmpSet);
			
			// 释放空间
			for (int wordIndex = 0; wordIndex < wordNum; ++wordIndex)
				delete[] words[wordIndex];
			delete[] words;
		}
		INPUT.close();
	}

	/* ------------------------- 分割线 ----------------------------- */
	// 过滤tmpTransRecords到transRecordsKey
	for (int i = 0; i < tmpTransRecords.size(); ++i)
	{
		vector<SString> tmpRecords;
		for (int j = 0; j < tmpTransRecords[i].size(); ++j)
		{
			if (mapKey[tmpTransRecords[i][j]] < minSup)
				continue;
			tmpRecords.push_back(tmpTransRecords[i][j]);
		}
		// 对tmpRecords进行排序
		sort(tmpRecords.begin(), tmpRecords.end(), cmp_by_key);
		// 将排好序的vector<SString>压如transRecordsNew
		if (tmpRecords.size() < 2)
			continue;
		transRecordsKey.push_back(tmpRecords);
	}

	/* ------------------------- 分割线 ----------------------------- */
	// map排序输出，效果特别好
	vector<PAIR> vecStr(mapKey.begin(), mapKey.end());
	sort(vecStr.begin(), vecStr.end(), cmp_by_value);
	for (int i = 0; i < vecStr.size(); ++i)
	{
		DISTRIBUTE_KEY << vecStr[i].first.sWord << " -- " << vecStr[i].second << endl;
	}

	/* ------------------------- 分割线 ----------------------------- */
	// 关闭文件输出
	DISTRIBUTE_KEY.close();
	TEST_OUTPUT.close();
	OUTPUT.close();

	return true;
}

bool cmp_by_value(const PAIR& a, const PAIR& b)
{
	return a.second > b.second;
}

bool cmp_by_new(const SString &a, const SString &b)
{
	return mapNew[a.sWord] > mapNew[b.sWord];
}

bool cmp_by_key(const SString &a, const SString &b)
{
	return mapKey[a.sWord] > mapKey[b.sWord];
}