//
//  main.cpp
//  Shannon
//
//  Created by 景玥 on 14-12-17.
//  Copyright (c) 2014年 Jen. All rights reserved.
//

#include"iostream"
#include "queue"
#include "map"
#include "string"
#include "iterator"
#include "vector"
#include "algorithm"
#include "math.h"
//using namespace std;

#define NChar 8	//用8位表示所有符号
#define Nsymbols 1<<NChar	//可以表示256种符号（包括a-z,A-Z）
#define INF 1<<31-1

typedef std::vector<bool> SF_Code;//定义一个字符占八位
std::map<char,SF_Code> SF_Dic;	//哈夫曼编码词典
int Sumvec[Nsymbols];	//记录排序后的符号数

class HTree
{
public :
	HTree* left;
	HTree* right;
	char ch;
	int weight;

	HTree(){left = right = NULL; weight=0;ch ='\0';}
	HTree(HTree* l,HTree* r,int w,char c){left = l;	right = r;	weight=w;	ch=c;}
	~HTree(){delete left; delete right;}
	bool Isleaf(){return !left && !right; }
};

bool comp(const HTree* t1, const HTree* t2)//排序函数
{	return (*t1).weight>(*t2).weight;	}

typedef std::vector<HTree*> TreeVector;
TreeVector TreeArr;//记录排序后的符号指针数

void Optimize_Tree(int a,int b,HTree& root)//查找最优树分点
{
	if(a==b)//构建一个叶子结点
	{
		root = *TreeArr[a-1];
		return;
	}
	else if(b-a==1)//构建两个叶子结点
	{
		root.left = TreeArr[a-1];
		root.right=TreeArr[b-1];
		return;
	}
	//查找最优点x
	int x,minn=INF,curdiff;
	for(int i=a;i<b;i++)//查找左右之差最小的点
	{
		curdiff = Sumvec[i]*2-Sumvec[a-1]-Sumvec[b];
		if(abs(curdiff)<minn){
			x=i;
			minn = abs(curdiff);
		}
		else break;
	}
	HTree*lc = new HTree;
    HTree *rc = new HTree;
	root.left = lc;
    root.right = rc;
	Optimize_Tree(a,x,*lc);
	Optimize_Tree(x+1,b,*rc);
}

HTree* BuildTree(int* freqency)//用Optimize_Tree构造一个树
{
	int i;
	for(i=0;i<Nsymbols;i++)//统计数字
	{
		if(freqency[i])
			TreeArr.push_back(new HTree (NULL,NULL,freqency[i], (char)i));
	}
	sort(TreeArr.begin(), TreeArr.end(), comp);
	memset(Sumvec,0,sizeof(Sumvec));
	for(i=1;i<=TreeArr.size();i++)
		Sumvec[i] = Sumvec[i-1]+TreeArr[i-1]->weight;
	HTree* root = new HTree;
	Optimize_Tree(1,TreeArr.size(),*root);
	return root;
}

//利用Shanno给树编码
void Generate_Coding(HTree* root, SF_Code& curcode)
{
	if(root->Isleaf())
	{
		SF_Dic[root->ch] = curcode;
		return;
	}
	SF_Code lcode = curcode;
	SF_Code rcode = curcode;
	lcode.push_back(false);
	rcode.push_back(true);
	Generate_Coding(root->left,lcode);
	Generate_Coding(root->right,rcode);
}

//主函数
int main()
{
	int freq[Nsymbols] = {0};
	char *str = "aaabbbccccccddddeeeeefffffgh";


	//统计字符的频数
	while (*str!='\0')		freq[*str++]++;

	//构建树
	HTree* r = BuildTree(freq);
	SF_Code nullcode;
	Generate_Coding(r,nullcode);

	for(std::map<char,SF_Code>::iterator it = SF_Dic.begin(); it != SF_Dic.end(); it++) {  
		std::cout<<(*it).first<<'\t';
		std::copy(it->second.begin(),it->second.end(),std::ostream_iterator<bool>(std::cout));  
		std::cout<<std::endl;  
	}  
}