
#include "process.h"


bool ProcessNewWords(int iTravelSize)
{
	BLACK_WORDS.open(blackWordsFile, ios::out);
	WHITE_WORDS.open(whiteWordsFile, ios::out);
	DISTRIBUTE_NEW.open(distributeNewFile, ios::out);
	TEST_OUTPUT.open(testNewFile, ios::out);
	// ����FP-Tree����Ҫ������
	vector<vector<SString>> tmpTransRecords;

	// ��ʼ��ÿһƪ���½��в���
	for (int i = 0; i < iTravelSize; ++i)
	{
		cout << i << endl;
		sprintf(inputFile, "./context/context%d.txt", i);
		sprintf(newOutputFile, "./result_new/new_words%d.txt", i);

		/* ------------------------- �ָ��� ----------------------------- */
		// �ִʣ���ȡ�ؼ��ʣ���������
		sResult = NLPIR_GetFileNewWords(inputFile, 50, true);

		// ��ʼ�ָ�
		CSplit::split(sResult, '#', words, wordNum, 30);

		// �ָ�󣬶�ÿ���ʽ��д�������
		// ��ʱ��洢����
		vector<SString> tmpSet;
		for (int j = 0; j < wordNum; ++j)
		{
			// �����TEST
			TEST_OUTPUT << words[j] << endl;

			// ��ÿ���ʵĵ�������Էָ��
			char **subWords = NULL;
			int subWordsNum = 0;
			CSplit::split(words[j], '/', subWords, subWordsNum, 4);
			if (subWords[1][0] != 'n') // ����Ƕ��ʣ�����˵�
			{
				continue;
			}

			// ��map�洢����
			mapNew[SString(subWords[0])] += 1; // (atoi(subWords[2]) > 0) ? atoi(subWords[2]) : 1;

			// �����
			tmpSet.push_back(SString(subWords[0]));

			// �ͷſռ�
			delete[] words[j];
			for (int wordIndex = 0; wordIndex < subWordsNum; ++wordIndex)
			{
				delete[] subWords[wordIndex];
			}
			delete[] subWords;
		}
		// ����transRecords
		tmpTransRecords.push_back(tmpSet);
		
		// �ͷ�words����Ŀռ�
		delete[] words;
	}
	/* ------------------------- �ָ��� ----------------------------- */
	// ����tmpTransRecords��transRecordsNew
	for (int i = 0; i < tmpTransRecords.size(); ++i)
	{
		vector<SString> tmpRecords;
		for (int j = 0; j < tmpTransRecords[i].size(); ++j)
		{
			if (mapNew[tmpTransRecords[i][j]] < minSup)
				continue;
			tmpRecords.push_back(tmpTransRecords[i][j]);
		}
		// ��tmpRecords��������
		sort(tmpRecords.begin(), tmpRecords.end(), cmp_by_new);
		// ���ź����vector<SString>ѹ��transRecordsNew
		if (tmpRecords.size() >= 2)
			transRecordsNew.push_back(tmpRecords);
	}


	/* ------------------------- �ָ��� ----------------------------- */
	// map���������Ч���ر��
	vector<PAIR> vecStr(mapNew.begin(), mapNew.end());
	sort(vecStr.begin(), vecStr.end(), cmp_by_value);
	for (int i = 0; i < vecStr.size(); ++i)
	{
		DISTRIBUTE_NEW << vecStr[i].first.sWord << " -- " << vecStr[i].second << endl;
	}

	/* ------------------------- �ָ��� ----------------------------- */
	// ���map�����ݣ������´ʹ��˼���ʵ�
	for (map<SString, int>::iterator iter = mapNew.begin(); iter != mapNew.end(); iter++)
	{
		// �ʹ���
		if (iter->second < 5)
			continue;
		WHITE_WORDS << iter->first.sWord << endl;
	}

	/* ------------------------- �ָ��� ----------------------------- */

	// �ͷ�map
	//mapStr.clear();
	vecStr.clear();

	// �ر��ļ�
	BLACK_WORDS.close();
	WHITE_WORDS.close();
	DISTRIBUTE_NEW.close();
	TEST_OUTPUT.close();

	return true;
}

bool ProcessKeyWords(int iTravelSize)
{
	DISTRIBUTE_KEY.open(distributeKeyFile, ios::out);
	// ��ʼ��ÿһƪ���½��в���
	for (int i = 0; i < iTravelSize; ++i)
	{
		cout << i << endl;
		sprintf(inputFile, "./context/context%d.txt", i);
		sprintf(keyOutputFile, "./result_key/key_words%d.txt", i);

		/* ------------------------- �ָ��� ----------------------------- */
		// GetFileKeyWords
		// �ִʣ���ȡ�ؼ��ʣ���������
		sResult = NLPIR_GetFileKeyWords(inputFile, 50, true);

		// ��ʼ�ָ�
		CSplit::split(sResult, '#', words, wordNum, 30);

		// �ָ�󣬶�ÿ���ʽ��д�������
		fstream KEY_WORDS;
		KEY_WORDS.open(keyOutputFile, ios::out);
		KEY_WORDS << wordNum << endl;
		sprintf(testKeyFile, "./test_key/test%d.txt", i);
		TEST_OUTPUT.open(testKeyFile, ios::out);
		for (int j = 0; j < wordNum; ++j)
		{
			// �����TEST
			TEST_OUTPUT << words[j] << endl;

			// ��ÿ���ʵĵ�������Էָ��
			char **subWords = NULL;
			int subWordsNum = 0;
			CSplit::split(words[j], '/', subWords, subWordsNum, 4);
			if (subWords[1][0] != 'n') // ����Ƕ��ʣ�����˵�
			{
				continue;
			}

			// ������ı�
			KEY_WORDS << subWords[0] << endl;

			// ��map�洢���ݣ����ִ���Ҳ�ӽ���
			mapKey[SString(subWords[0])] += atoi(subWords[2]);

			// �ͷſռ�
			delete[] words[j];
			for (int wordIndex = 0; wordIndex < subWordsNum; ++wordIndex)
			{
				delete[] subWords[wordIndex];
			}
			delete[] subWords;
		}
		KEY_WORDS.close();
		TEST_OUTPUT.close();
		
		// �ͷ�words����Ŀռ�
		delete[] words;
	}

	/* ------------------------- �ָ��� ----------------------------- */
	// ��map��������
	vector<PAIR> vecStr(mapKey.begin(), mapKey.end());
	sort(vecStr.begin(), vecStr.end(), cmp_by_value);
	for (int i = 0; i < vecStr.size(); ++i)
	{
		DISTRIBUTE_KEY << vecStr[i].first.sWord << " -- " << vecStr[i].second << endl;
	}

	/* ------------------------- �ָ��� ----------------------------- */

	// ���map�����ݣ������´ʹ��˼���ʵ�
	// ��ͬƵ�ʵĴ��������ͬ���ļ�
	fstream DISTRIBUTE100("./result/distribute_100.txt", ios::out);
	fstream DISTRIBUTE50("./result/distribute_50.txt", ios::out);
	fstream DISTRIBUTE30("./result/distribute_30.txt", ios::out);
	fstream DISTRIBUTE10("./result/distribute_10.txt", ios::out);
	fstream DISTRIBUTE5("./result/distribute_5.txt", ios::out);
	for (map<SString, int>::iterator iter = mapKey.begin(); iter != mapKey.end(); iter++)
	{
		// �ʹ���
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

	/* ------------------------- �ָ��� ----------------------------- */

	// �ͷ�map
	mapKey.clear();
	vecStr.clear();

	// �ر��ļ�
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

	// ����FP-Tree����Ҫ������
	vector<vector<SString>> tmpTransRecords;
	// ��ʼ��ÿһƪ���½��в���
	for (int i = 0; i < iTravelSize; ++i)
	{
		cout << i << endl;
		sprintf(inputFile, "./context/context%d.txt", i);

		/* ------------------------- �ָ��� ----------------------------- */
		// GetKeyWords
		// �����µ�ÿһ������д��Է���
		
		fstream INPUT(inputFile, ios::in);
		while (getline(INPUT, buffer))
		{
			if (buffer == "")
				continue;
			vector<SString> tmpSet;
			const char* tmpResult = NLPIR_GetKeyWords(buffer.c_str(), 20, true);
			OUTPUT << tmpResult << endl;
			// �Խ�����зָ�
			CSplit::split(tmpResult, '#', words, wordNum, 50);
			for (int j = 0; j < wordNum; ++j)
			{
				// ���TEST��
				TEST_OUTPUT << words[j] << endl;

				// ��ÿһ���ʽ��зָ�
				char **subWords = NULL;
				int subWordsNum = 0;
				CSplit::split(words[j], '/', subWords, subWordsNum, 5);
				if (subWords[1][0] != 'n')
					continue;
				tmpSet.push_back(SString(subWords[0]));
				mapKey[subWords[0]]++;

				// �ͷſռ�
				for (int wordIndex = 0; wordIndex < subWordsNum; ++wordIndex)
					delete[] subWords[wordIndex];
				delete[] subWords;
			}
			// ѹ����ʱ����
			tmpTransRecords.push_back(tmpSet);
			
			// �ͷſռ�
			for (int wordIndex = 0; wordIndex < wordNum; ++wordIndex)
				delete[] words[wordIndex];
			delete[] words;
		}
		INPUT.close();
	}

	/* ------------------------- �ָ��� ----------------------------- */
	// ����tmpTransRecords��transRecordsKey
	for (int i = 0; i < tmpTransRecords.size(); ++i)
	{
		vector<SString> tmpRecords;
		for (int j = 0; j < tmpTransRecords[i].size(); ++j)
		{
			if (mapKey[tmpTransRecords[i][j]] < minSup)
				continue;
			tmpRecords.push_back(tmpTransRecords[i][j]);
		}
		// ��tmpRecords��������
		sort(tmpRecords.begin(), tmpRecords.end(), cmp_by_key);
		// ���ź����vector<SString>ѹ��transRecordsNew
		if (tmpRecords.size() < 2)
			continue;
		transRecordsKey.push_back(tmpRecords);
	}

	/* ------------------------- �ָ��� ----------------------------- */
	// map���������Ч���ر��
	vector<PAIR> vecStr(mapKey.begin(), mapKey.end());
	sort(vecStr.begin(), vecStr.end(), cmp_by_value);
	for (int i = 0; i < vecStr.size(); ++i)
	{
		DISTRIBUTE_KEY << vecStr[i].first.sWord << " -- " << vecStr[i].second << endl;
	}

	/* ------------------------- �ָ��� ----------------------------- */
	// �ر��ļ����
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