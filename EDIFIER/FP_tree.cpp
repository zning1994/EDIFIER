
#include "FP_tree.h"

void FPGrowth(vector<vector<SString>> transRecords, vector<SString> postPattern)
{
	LOG << "Start...1" << endl;
	cout << "growth...1" << endl;
	// 构建项头表
	LOG << "Get header table...2" << endl;
	cout << "get header table...2" << endl;
	vector<STreeNode> headerTable = buildHeaderTable(transRecords);
	// 构建FP-Tree
	LOG << "Get tree root...3" << endl;
	cout << "get tree root...3" << endl;
	STreeNode treeRoot = buildFPTree(transRecords, headerTable);
	// 输出header table
	LOG << "*****************************************************" << endl;
	for (int i = 0; i < headerTable.size(); ++i)
	{
		LOG << headerTable[i].m_sWord << " -- " << headerTable[i].m_supNum << endl;
	}
	LOG << "**************************HT**************************" << endl;
	// 输出树
	LOG << "------------------------------------------------------" << endl;
	LOG << treeRoot.getChildrenNum() << endl;
	for (int i = 0; i < treeRoot.m_pstChildren.size(); ++i)
	{
		LOG << treeRoot.m_pstChildren[i]->m_sWord << " -- " << treeRoot.m_pstChildren[i]->getChildrenNum() << endl;
	}
	LOG << "------------------------TR----------------------------" << endl;
	// 如果FP-Tree为空则返回
	if (treeRoot.getChildrenNum() == 0)
		return;
	// 处理项头表的每一项+postPattern
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
	// 找到项头表的每一项的条件模式基，进入递归迭代
	for (int i = headerTable.size() - 1; i > 0; --i)
	{
		LOG << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		LOG << "new post pattern = " << headerTable[i].m_sWord << endl;
		cout << "1" << endl;
		// 后缀模式增加一项
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
		// 寻找header的条件模式基CPB，放入newTransRecords中
		vector<vector<SString>> newTransRecords;
		STreeNode *itemLink = headerTable[i].m_stBrother;
		/****************************************************/
		// 输出测试
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
			// 遍历一条条件模式基
			STreeNode *curNode = itemLink->m_stParent;
			// 该条件模式基的数量
			int counter = itemLink->m_supNum;
			LOG << "link num = " << counter << endl;
			cout << "4 - 2" << endl;
			// 如果该节点没有父节点或者父节点为根节点，则表示没有条件模式基
			while (NULL != curNode && strcmp(curNode->m_sWord, "") != 0)
			{
				LOG << curNode->m_sWord << "\t";
				preNode.push_back(SString(curNode->m_sWord));
				curNode = curNode->m_stParent;
			}
			cout << "4 - 3" << endl;
			LOG << "......" << endl;
			// 将顺序调整过来
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
	LOG << "------------------ 分割线 -------------------" << endl;
}

vector<STreeNode> buildHeaderTable(const vector<vector<SString>> transRecords)
{
	LOG << "Build header table..." << endl;
	vector<STreeNode> headerTable;
	// 存储已经在表头的项
	map<SString, int> tmpFlag;
	// 遍历transRecords，构建表头
	for (int i = 0; i < transRecords.size(); ++i)
	{
		for (int j = 0; j < transRecords[i].size(); ++j)
		{
			if (tmpFlag[transRecords[i][j].sWord] > 0)
				continue;
			headerTable.push_back(STreeNode(transRecords[i][j].sWord));
			// 标记
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
	// 遍历transRecords，构建FP-Tree
	for (int i = 0; i < transRecords.size(); ++i)
	{
		// 初始化
		curNode = treeRoot;
		for (int j = 0; j < transRecords[i].size(); ++j)
		{
			// 标记
			bool isExist = false;
			// 更新表头
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
			// 将该项集添加到树
			LOG << "Add set to tree..." << endl;
			// 如果已经存在该节点curNode中
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
				// 否则创建一个节点
				LOG << "New a node..." << endl;
				STreeNode *newNode = new STreeNode(transRecords[i][j].sWord, 1);
				// 更新父节点指针
				newNode->m_stParent = curNode;
				// 更新brother节点
				brotherNode->m_stBrother = newNode;
				// 更新父节点
				curNode->m_pstChildren.push_back(newNode);
				curNode = newNode;
			}
			TREE << transRecords[i][j].sWord << "\t";
		}
		TREE << endl;
	}
	// 对headerTable进行排序
	sort(headerTable.begin(), headerTable.end(), cmp_by_sup);
	// 过滤headerTable
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