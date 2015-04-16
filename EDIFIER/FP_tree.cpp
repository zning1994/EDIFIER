
#include "FP_tree.h"

void FPGrowth(vector<vector<SString>> transRecords, vector<SString> postPattern)
{
	LOG << "Start...1" << endl;
	cout << "growth...1" << endl;
	// ������ͷ��
	LOG << "Get header table...2" << endl;
	cout << "get header table...2" << endl;
	vector<STreeNode> headerTable = buildHeaderTable(transRecords);
	// ����FP-Tree
	LOG << "Get tree root...3" << endl;
	cout << "get tree root...3" << endl;
	STreeNode treeRoot = buildFPTree(transRecords, headerTable);
	// ���header table
	LOG << "*****************************************************" << endl;
	for (int i = 0; i < headerTable.size(); ++i)
	{
		LOG << headerTable[i].m_sWord << " -- " << headerTable[i].m_supNum << endl;
	}
	LOG << "**************************HT**************************" << endl;
	// �����
	LOG << "------------------------------------------------------" << endl;
	LOG << treeRoot.getChildrenNum() << endl;
	for (int i = 0; i < treeRoot.m_pstChildren.size(); ++i)
	{
		LOG << treeRoot.m_pstChildren[i]->m_sWord << " -- " << treeRoot.m_pstChildren[i]->getChildrenNum() << endl;
	}
	LOG << "------------------------TR----------------------------" << endl;
	// ���FP-TreeΪ���򷵻�
	if (treeRoot.getChildrenNum() == 0)
		return;
	// ������ͷ���ÿһ��+postPattern
	LOG << "Process header table...4" << endl;
	cout << "process header table...4" << endl;
	if (!postPattern.empty())
	{
		for (int i = 0; i < headerTable.size(); ++i)
		{
			int count = headerTable[i].m_supNum;
			RESULT << count << ", " << headerTable[i].m_sWord << "\t";
			for (int j = 0; j < postPattern.size(); ++j)
			{
				RESULT << postPattern[j].sWord << "\t";
			}
			RESULT << endl;
		}
	}
	LOG << "Process header table end...5" << endl;
	cout << "process header table end...5" << endl;
	// �ҵ���ͷ���ÿһ�������ģʽ��������ݹ����
	for (int i = headerTable.size() - 1; i > 0; --i)
	{
		LOG << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		LOG << "new post pattern = " << headerTable[i].m_sWord << endl;
		cout << "1" << endl;
		// ��׺ģʽ����һ��
		vector<SString> newPostPattern;
		newPostPattern.push_back(SString(headerTable[i].m_sWord));
		cout << "2" << endl;
		if (!postPattern.empty())
		{
			for (int j = 0; j < postPattern.size(); ++j)
			{
				newPostPattern.push_back(SString(postPattern[j].sWord));
				LOG << "old post pattern = " << postPattern[j].sWord << endl;
			}
		}
		cout << "3" << endl;
		// Ѱ��header������ģʽ��CPB������newTransRecords��
		vector<vector<SString>> newTransRecords;
		STreeNode *itemLink = headerTable[i].m_stBrother;
		/****************************************************/
		// �������
		fstream TMP_OUTPUT("tmp_test.txt", ios::out);
		STreeNode *tmpPtr = itemLink;
		while (tmpPtr != NULL)
		{
			TMP_OUTPUT << tmpPtr->m_sWord << " -- " << tmpPtr->m_supNum << endl;
			tmpPtr = tmpPtr->m_stBrother;
		}
		TMP_OUTPUT << "-------------------------------------------------------------------------------" << endl;
		TMP_OUTPUT.close();
		/****************************************************/
		cout << "4" << endl;
		while (itemLink != NULL)
		{
			cout << "4 - 1" << endl;
			vector<SString> preNode;
			// ����һ������ģʽ��
			STreeNode *curNode = itemLink->m_stParent;
			// ������ģʽ��������
			int counter = itemLink->m_supNum;
			LOG << "link num = " << counter << endl;
			cout << "4 - 2" << endl;
			// ����ýڵ�û�и��ڵ���߸��ڵ�Ϊ���ڵ㣬���ʾû������ģʽ��
			while (NULL != curNode && strcmp(curNode->m_sWord, "") != 0)
			{
				LOG << curNode->m_sWord << "\t";
				preNode.push_back(SString(curNode->m_sWord));
				curNode = curNode->m_stParent;
			}
			cout << "4 - 3" << endl;
			LOG << "......" << endl;
			// ��˳���������
			reverse(preNode.begin(), preNode.end());
			cout << "4 - 4" << endl;
			while (counter-- > 0)
				newTransRecords.push_back(preNode);
			itemLink = itemLink->m_stBrother;
			cout << "4 - 5" << endl;
		}
		LOG << "~~~~~~~~~~~~~~~~~NEW POST PATTERN~~~~~~~~~~~~~~~~~~~" << endl;
		FPGrowth(newTransRecords, newPostPattern);
	}
	LOG << "------------------ �ָ��� -------------------" << endl;
}

vector<STreeNode> buildHeaderTable(const vector<vector<SString>> transRecords)
{
	LOG << "Build header table..." << endl;
	vector<STreeNode> headerTable;
	// �洢�Ѿ��ڱ�ͷ����
	map<SString, int> tmpFlag;
	// ����transRecords��������ͷ
	for (int i = 0; i < transRecords.size(); ++i)
	{
		for (int j = 0; j < transRecords[i].size(); ++j)
		{
			if (tmpFlag[transRecords[i][j].sWord] > 0)
				continue;
			headerTable.push_back(STreeNode(transRecords[i][j].sWord));
			// ���
			tmpFlag[transRecords[i][j].sWord] = 1;
			TABLE << transRecords[i][j].sWord << "\t";
		}
		TABLE << endl;
	}
	TABLE << "------------------------------TABLE----------------------------" << endl;

	return headerTable;
}

STreeNode buildFPTree(const vector<vector<SString>> transRecords, vector<STreeNode> &headerTable)
{
	LOG << "Build FP Tree..." << endl;
	STreeNode *treeRoot = new STreeNode();
	STreeNode *curNode = NULL;
	// ����transRecords������FP-Tree
	for (int i = 0; i < transRecords.size(); ++i)
	{
		// ��ʼ��
		curNode = treeRoot;
		for (int j = 0; j < transRecords[i].size(); ++j)
		{
			// ���
			bool isExist = false;
			// ���±�ͷ
			LOG << "Update header table..." << endl;
			STreeNode *brotherNode = NULL;
			for (int headerIndex = 0; headerIndex < headerTable.size(); ++headerIndex)
			{
				if (strcmp(headerTable[headerIndex].m_sWord, transRecords[i][j].sWord) == 0)
				{
					headerTable[headerIndex].m_supNum++;
					brotherNode = &headerTable[headerIndex];
					while (brotherNode->m_stBrother != NULL)
						brotherNode = brotherNode->m_stBrother;
					break;
				}
			}
			if (brotherNode == NULL)
			{
				cout << "get brother ptr failed" << endl;
				system("puase");
			}
			// �������ӵ���
			LOG << "Add set to tree..." << endl;
			// ����Ѿ����ڸýڵ�curNode��
			for (int childIndex = 0; childIndex < curNode->m_pstChildren.size(); ++childIndex)
			{
				if (NULL == curNode->m_pstChildren[childIndex])
					continue;
				if (strcmp(curNode->m_pstChildren[childIndex]->m_sWord, transRecords[i][j].sWord) == 0)
				{
					//LOG << "Node is exist..." << endl;
					curNode->m_pstChildren[childIndex]->m_supNum++;
					curNode = curNode->m_pstChildren[childIndex];
					isExist = true;
					break;
				}
			}
			if (!isExist)
			{
				// ���򴴽�һ���ڵ�
				LOG << "New a node..." << endl;
				STreeNode *newNode = new STreeNode(transRecords[i][j].sWord, 1);
				// ���¸��ڵ�ָ��
				newNode->m_stParent = curNode;
				// ����brother�ڵ�
				brotherNode->m_stBrother = newNode;
				// ���¸��ڵ�
				curNode->m_pstChildren.push_back(newNode);
				curNode = newNode;
			}
			TREE << transRecords[i][j].sWord << "\t";
		}
		TREE << endl;
	}
	// ��headerTable��������
	sort(headerTable.begin(), headerTable.end(), cmp_by_sup);
	// ����headerTable
	for (vector<STreeNode>::iterator iter = headerTable.begin(); iter != headerTable.end(); iter++)
	{
		if (iter->m_supNum < minSup)
		{
			headerTable.erase(iter, headerTable.end());
			break;
		}
	}

	TREE << "-------------------------TREE-----------------------------" << endl;

	return *treeRoot;
}


void printTree(const STreeNode *treeRoot)
{
	fstream TREE("tree.txt", ios::app);



	TREE.close();
}

bool cmp_by_sup(const STreeNode &a, const STreeNode &b)
{
	return a.m_supNum > b.m_supNum;
}