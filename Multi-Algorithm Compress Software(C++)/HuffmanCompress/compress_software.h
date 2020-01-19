//
//  compress_software.h
//  HuffmanCompress
//
//  Created by 景玥 on 14-12-17.
//  Copyright (c) 2014年 Jen. All rights reserved.
//

#ifndef __COMPRESS_SOFTWARE_H__
#define __COMPRESS_SOFTWARE_H__

#include <iostream>
#include <string>


//=====================================哈夫曼树结点基类===================================
template<class CharType,class WeightType>
class MyHuffNode
{//哈夫曼树结点基类
public:
    virtual WeightType Weight() = 0;                                     //返回结点权值
    virtual int IsLeaf() = 0;                                            //判断是否为叶结点，真为，假为
    virtual MyHuffNode<CharType,WeightType> *Left() = 0;                 //返回左孩子
    virtual MyHuffNode<CharType,WeightType> *Right() = 0;                //返回右孩子
    
};

//哈夫曼树结点基类函数的定义
template<class CharType,class WeightType>
MyHuffNode<CharType,WeightType> *MyHuffNode<CharType,WeightType>::Left()
{
	return 0;						//返回左孩子为空
}

template<class CharType,class WeightType>
MyHuffNode<CharType,WeightType> *MyHuffNode<CharType,WeightType>::Right()
{
	return 0;						//返回右孩子为空
}


//========================================哈夫曼树叶结点类==================
template<class CharType,class WeightType>
class MyLeafNode:public MyHuffNode<CharType,WeightType>
{//叶结点类
private:
    CharType cha;              //叶结点包含字符
    WeightType weight;         //权值
    MyHuffNode<CharType,WeightType> *lChild;          //左孩子为空
    MyHuffNode<CharType,WeightType> *rChild;          //右孩子为空
    
public:
    MyLeafNode(const CharType &ch,const WeightType &w);          //构造函数
    virtual ~MyLeafNode(){};                                     //析构函数
    CharType LeafChar();                                         //返回叶结点字符
    WeightType Weight();                                         //返回叶结点权值
    int IsLeaf();                                                //判断是否为叶结点
    MyHuffNode<CharType,WeightType> *Left();                     //返回左孩子
    MyHuffNode<CharType,WeightType> *Right();                    //返回右孩子
    
};

//哈夫曼树叶结点类函数的定义
template<class CharType,class WeightType>
MyLeafNode<CharType,WeightType>::MyLeafNode(const CharType &ch,const WeightType &w)
{//构造函数
	cha = ch;
	weight = w;
}

template<class CharType,class WeightType>
CharType MyLeafNode<CharType,WeightType>::LeafChar()
{
	return cha;										//返回叶结点字符
}

template<class CharType,class WeightType>
int MyLeafNode<CharType,WeightType>::IsLeaf()
{
	return 1;										//为叶结点，返回
}

template<class CharType,class WeightType>
WeightType MyLeafNode<CharType,WeightType>::Weight()
{
	return weight;									//返回结点权值
}

template<class CharType,class WeightType>
MyHuffNode<CharType,WeightType> *MyLeafNode<CharType,WeightType>::Left()
{
	return 0;										//为叶结点，返回左孩子为空
}


template<class CharType,class WeightType>
MyHuffNode<CharType,WeightType> *MyLeafNode<CharType,WeightType>::Right()
{
	return 0;										//为叶结点，返回右孩子为空
}



//===========================================哈夫曼树内部结点类====================
template<class CharType,class WeightType>
class MyInNode:public MyHuffNode<CharType,WeightType>
{//哈夫曼树内部结点类
private:
    WeightType weight;                                //权值
    MyHuffNode<CharType,WeightType> *lChild;          //左孩子
    MyHuffNode<CharType,WeightType> *rChild;          //右孩子
    
    
public:
    MyInNode(const WeightType &w,MyHuffNode<CharType,WeightType> *lc,MyHuffNode<CharType,WeightType> *rc);   //构造函数
    virtual ~MyInNode(){};										 //析构函数
    WeightType Weight();                                         //返回叶结点权值
    int IsLeaf();                                                //判断是否为叶结点
    MyHuffNode<CharType,WeightType> *Left();                     //返回左孩子
    MyHuffNode<CharType,WeightType> *Right();                    //返回右孩子
    
};

//哈夫曼树内部结点类函数的定义
template<class CharType,class WeightType>
MyInNode<CharType,WeightType>::MyInNode(const WeightType &w,MyHuffNode<CharType,WeightType> *lc,MyHuffNode<CharType,WeightType> *rc)
{//构造函数
	weight = w;
	lChild = lc;
	rChild = rc;
}


template<class CharType,class WeightType>
int MyInNode<CharType,WeightType>::IsLeaf()
{
	return 0;										//不是叶节点，返回
}

template<class CharType,class WeightType>
WeightType MyInNode<CharType,WeightType>::Weight()
{
	return weight;									//返回结点权值
}

template<class CharType,class WeightType>
MyHuffNode<CharType,WeightType> *MyInNode<CharType,WeightType>::Left()
{
	return lChild;									//返回左孩子指针
}


template<class CharType,class WeightType>
MyHuffNode<CharType,WeightType> *MyInNode<CharType,WeightType>::Right()
{
	return rChild;									//返回右孩子指针
}



//============================================哈夫曼树类===================
template<class CharType,class WeightType>
class MyHuffTree
{//哈夫曼树类
protected:
    MyHuffNode<CharType,WeightType> *root;              //根
    std::string *charCodes;                                  //字符编码信息
    MyHuffNode<CharType,WeightType> *pCurNode;			//当前结点
    int m;												//叶结点个数
    
    
	//辅助函数
    void CreatCode(MyHuffNode<CharType,WeightType> *r,char code[],int len = 0);			//生成字符编码
    
public:
    MyHuffTree(CharType ch[],WeightType w[],int n);		//由字符、权值、字符个数构造哈夫曼树
    virtual ~MyHuffTree(){};							//析构函数
    std::string Encode(CharType ch);							//编码
    LinkList<CharType> Decode(std::string strCode);			//译码
};

//哈夫曼树类函数的定义
template<class CharType,class WeightType>
void MyHuffTree<CharType,WeightType>::CreatCode(MyHuffNode<CharType,WeightType> *r,char code[],int len)
{//生成字符编码
	
	//使用递归的遍历哈夫曼树的方法生成字符编码
	if( r->IsLeaf() == 1 )									//如果遍历到达叶结点，则将其路径编码存入相应的数组元素
	{
		CharType ch = ((MyLeafNode<CharType,WeightType> *) r)->LeafChar();
		code[len] = '\0';
		std::string strCode(code);
		charCodes[(unsigned char)ch] = strCode;
	}
	
	else
	{
		code[len] = '0';									//向左搜索
		CreatCode(r->Left(),code,len+1);
        
		code[len] = '1';									//向右搜索
		CreatCode(r->Right(),code,len+1);
	}
	
}



//使用结构定义指向结点的指针
template<class CharType,class WeightType>
struct MyHuffNodehelp
{
	MyHuffNode<CharType,WeightType> * ptr;//结点的抽象基类指针
};

//关系运算符重载
template<class CharType,class WeightType>
bool operator < (const MyHuffNodehelp<CharType,WeightType> &first,const MyHuffNodehelp<CharType,WeightType> &second)
{//关系运算符<重载
	return first.ptr->Weight() < second.ptr->Weight();
}

template<class CharType,class WeightType>
bool operator > (const MyHuffNodehelp<CharType,WeightType>&first,const MyHuffNodehelp<CharType,WeightType>&second)
{//关系运算符>重载
	return first.ptr->Weight() > second.ptr->Weight();
}

template<class CharType,class WeightType>
bool operator <= (const MyHuffNodehelp<CharType,WeightType>&first,const MyHuffNodehelp<CharType,WeightType>&second)
{//关系运算符<=重载
	return first.ptr->Weight() <= second.ptr->Weight();
}

template<class CharType,class WeightType>
MyHuffTree<CharType,WeightType>::MyHuffTree(CharType ch[],WeightType w[],int n)
{//哈夫曼树构造函数
    
    
	int num = n;
	charCodes= new std::string[num];
	int i;
	MinPriorityHeapQueue< MyHuffNodehelp<CharType,WeightType> > minHeap;	//定义一个小顶堆
    
	for (i = 0;i <= num;i++)
	{//生成森林
		MyHuffNodehelp<CharType,WeightType> tem;
		tem.ptr=new MyLeafNode<CharType,WeightType>(ch[i],w[i]);
		minHeap.InQueue(tem);
	}
    
	for(i = 0;i < num-1;i++)
	{//生成哈夫曼树
		MyHuffNodehelp<CharType,WeightType>r,r1,r2;
		
		//权值最小的两个结点合并
		minHeap.OutQueue(r1);
		minHeap.OutQueue(r2);
		r.ptr = new MyInNode<CharType,WeightType>(r1.ptr->Weight() + r2.ptr->Weight(),r1.ptr,r2.ptr);
		
		minHeap.InQueue(r);						//将新生成的结点放回堆中
	}
    
	MyHuffNodehelp<CharType,WeightType> rt;
	minHeap.OutQueue(rt);						//堆中唯一的元素出列
	root = rt. ptr;
	pCurNode = root;							//其为构建完成的哈夫曼树
	char *code = new char[num];
	CreatCode(root,code,0);						//以构建成的哈夫曼树为基础对各个字符进行编码
	delete []code;
    
}


template<class CharType,class WeightType>
LinkList<CharType> MyHuffTree<CharType,WeightType>::Decode(std::string strCode)
{//译码
	LinkList<CharType> charList;            //编码前的字符序列
    
	for (int pos = 0;pos < strCode.length();pos++)
	{//处理每位编码
		if(strCode[pos] == '0')
			pCurNode = pCurNode->Left();
		else
			pCurNode = pCurNode->Right();
        
		if((pCurNode->IsLeaf()) == 1)
		{
			charList.Insert(charList.Length()+1,((MyLeafNode<CharType,WeightType> *) pCurNode)->LeafChar());
			pCurNode = root;
		}
        
	}
    
	return charList;						//返回修改后的顺序表
}

template<class CharType,class WeightType>
std::string MyHuffTree<CharType,WeightType>::Encode(CharType ch)
{
	return charCodes[(unsigned char)ch];		//返回对应字符的编码
}



//=================================================哈夫曼树压缩类==================
struct BufferType
{//定义缓存器
	char ch;              //字符
	unsigned int bits;    //实际比特数
};

class MyHuffmanCompress
{// 哈夫曼压缩类
protected:
	MyHuffTree<char,unsigned long> *pMyHuffTree;
	FILE *infp,*outfp;
	BufferType buf;
    
	//辅助函数
	void Write(unsigned int bit);		//写入
	void WriteOver();					//强制写入
    
public:
	
	MyHuffmanCompress()
	{//构造函数
		pMyHuffTree = NULL;
	}
	
    
	~MyHuffmanCompress()
	{//析构函数
		pMyHuffTree = NULL;
	}
	
	void Compress();
	void Decompress();
    
};

void MyHuffmanCompress::Write(unsigned int bit)
{
	buf.bits++;									//缓存中写入一比特
	buf.ch = (buf.ch << 1) | bit;
	if(buf.bits == 8)							//缓存中达到为则输出
	{
		fputc(buf.ch,outfp);
		buf.bits = 0;
		buf.ch = 0;
	}
    
}

void MyHuffmanCompress::WriteOver()
{
	unsigned int len = buf.bits;
	//文件结束，强制写入
	if(len > 0)
	{
		for(unsigned int i = 0;i < 8 - len;i++)
			Write(0);
	}
}

void MyHuffmanCompress::Compress()
{//压缩操作
	char infName[256],outfName[256];             //存储输入输出文件名
    
	cout << "请输入要压缩的文件名（文件小于GB，文件名包含扩展名）：";
	cin >> infName;
	if((infp = fopen(infName,"rb")) == NULL)
		throw Error("打开文件失败！（要压缩的文件必须与程序处于同一目录下）");
    
	fgetc(infp);
	if(feof(infp))
		throw Error("要压缩的文件为空文件！（空文件压缩后体积会加大）");
    
	cout << endl;
	
	//对输出文件名进行处理
	strcpy(outfName,infName);
	
	char chtmp = outfName[0];
	int itmp = 0,qtmp = 0;
	
	//保留原文件扩展名
	while(chtmp != '\0')
	{
		if (chtmp == '.')
			qtmp = itmp;
		itmp++;
		chtmp = outfName[itmp];
	}
	outfName[qtmp] = '_';
	qtmp = strlen(outfName) - 1;
	
	//自定义压缩后的文件扩展名为cxy
	outfName[++qtmp] = '.';
	outfName[++qtmp] = 'f';
	outfName[++qtmp] = 'j';
	outfName[++qtmp] = 'y';
	outfName[++qtmp] = '\0';
    
	outfp = fopen(outfName,"wb");
    
	cout << "正在处理，请稍候……" << endl;
    
	const unsigned long n = 256;
	char ch[n];
	unsigned long w[n];
	unsigned long size = 0;
	unsigned int i;
	char cha;
    
	for(i = 0;i < n;i++)
	{
		ch[i] = (char) i;
		w[i] = 0;
	}
    
    
	rewind(infp);
	cha = fgetc(infp);
	while(!feof(infp))
	{
		w[(unsigned char)cha]++;
		size++;
		cha = fgetc(infp);
	}
    
	if(pMyHuffTree != NULL)
		delete []pMyHuffTree;      //第二次调用前释放空间
    
    
	pMyHuffTree = new MyHuffTree<char,unsigned long>(ch,w,n);
    
	rewind(outfp);
	fwrite(&size,sizeof(unsigned long),1,outfp);
	for(i = 0;i < n;i++)
	{
		fwrite(&w[i],sizeof(unsigned long),1,outfp);
	}
    
	buf.bits = 0;
	buf.ch = 0;
	rewind(infp);
	cha = fgetc(infp);
	while(!feof(infp))
	{
		std::string strTmp = pMyHuffTree->Encode(cha);
		for (i = 0;i < strTmp.length();i++)
		{
			if(strTmp[i] == '0')
				Write(0);
			else
				Write(1);
		}
		cha = fgetc(infp);
	}
	WriteOver();							//文件结束，强制写入
    
	fclose(infp);
	fclose(outfp);
	cout << "压缩结束！" << endl;
    
}

void MyHuffmanCompress::Decompress()
{/*//解压操作
	char infName[256],outfName[256];             //存储输入输出文件名
    
	cout << "请输入要解压的文件名（文件名包含扩展名，本程序仅解压扩展名为cxy的文件）：";
	cin >> infName;
	if((infp = fopen(infName,"rb")) == NULL)
		throw Error("打开文件失败！（要解的文件必须与程序处于同一目录下）");
    
	fgetc(infp);
	if(feof(infp))
		throw Error("要解压的文件为空文件！");
    
	cout << endl;
	
	//对输出文件名进行处理
	strcpy(outfName,infName);
    
	
	char chtmp = outfName[0];
	int itmp = 0,qtmp = 0;
	
	while(chtmp != '\0')
	{
		if (chtmp == '_')
			qtmp = itmp;
		itmp++;
		chtmp = outfName[itmp];
	}
	outfName[qtmp] = '.';
	
	qtmp = strlen(outfName) - 1;
	outfName[qtmp - 3] = '\0';
	
	outfp = fopen(outfName,"wb");
    
	cout << "正在处理，请稍候……" << endl;
	cout << "友情提示：本程序解压时间十分漫长，请耐心等待，一般情况下，不会出现死机现象！" << endl;
    
	const unsigned long n = 256;
	char ch[n];
	unsigned long w[n];
	unsigned long i,size = 0;
	char cha;
    
	rewind(infp);
	fread(&size,sizeof(unsigned long),1,infp);
	for (i = 0;i < n;i++)
	{
		ch[i] = (char) i;
		fread(&w[i],sizeof(unsigned long),1,infp);
	}
    
    
	if(pMyHuffTree != NULL)
		delete []pMyHuffTree;      //第二次调用前释放空间
	pMyHuffTree = new MyHuffTree<char,unsigned long>(ch,w,n);
    
	unsigned long len = 0;
	cha = fgetc (infp);
	while(!feof(infp))
	{
		std::string strTmp = "";
		unsigned char c = (unsigned char) cha;
        
		for(i = 0;i < 8;i++)
		{
			if(c < 128)
				Concat(strTmp,"0");
			else
				Concat(strTmp,"1");
            
			c = c << 1;
		}
        
		LinkList<char> lkText = pMyHuffTree->Decode(strTmp);   //译码
		
		
		std::string strTemp(lkText);
		for (i = 0;i < strTemp.Length();i++)
		{
			len++;
			fputc(strTemp[i],outfp);
			if(len == size)
				break;
		}
        
		if(len == size)
			break;
		cha = fgetc(infp);
	}
    
	fclose(infp);
	fclose(outfp);
	cout << "解压结束！" << endl;
	
*/}

#endif
