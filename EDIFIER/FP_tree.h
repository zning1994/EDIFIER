
#include "struct.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <map>
using namespace std;

#ifndef _FP_TREE_H_
#define _FP_TREE_H_

extern fstream LOG;
extern fstream TREE;
extern fstream TABLE;
extern fstream RESULT;

extern const int minSup;

void FPGrowth(vector<vector<SString>> transRecords, vector<SString> postPattern);

// ������ͷ��
vector<STreeNode> buildHeaderTable(const vector<vector<SString>> transRecords);

// ����FP-Tree
STreeNode buildFPTree(const vector<vector<SString>> transRecords, vector<STreeNode> &headerTable);

void printTree(const STreeNode *treeRoot);

bool cmp_by_sup(const STreeNode &a, const STreeNode &b);

#endif