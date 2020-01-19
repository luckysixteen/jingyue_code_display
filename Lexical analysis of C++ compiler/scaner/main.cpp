//
//  main.cpp
//  scaner
//
//  Created by 景玥 on 16/4/18.
//  Copyright (c) 2016年 Jen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ifstream fin;
void testout(string str);
bool outp=true;

class Scanner
{
public:
    Scanner()
    {
        char chl='a';
        char chh='A';
        for(int i=0;i<26;i++)
        {
            letter[i]=chl;
            chl++;
        }
        for(int i=26;i<52;i++)
        {
            letter[i]=chh;
            chh++;
        }
    }
    
    string str;
    string keyword[6]={"else","if","int","return","void","while"};
    string symbol[19]={"+","-","*","/","<","<=",">",">=","==","!=","=",";",",","(",")","[","]","{","}"};
    string digit[10]={"0","1","2","3","4","5","6","7","8","9"};
    
    bool iskeyword(string str)
    {
        for(int i=0;i<6;i++)
        {
            if(keyword[i]==str)
                return true;
        }
        return false;
    }
    
    bool issymbol(string str)
    {
        for(int i=0;i<21;i++)
        {
            if(symbol[i]==str)
                return true;
        }
        return false;
    }
    
    bool isletter(string str)
    {
        for(int i=0;i<52;i++)
        {
            if(letter[i]==str)
                return true;
        }
        return false;
    }
    
    bool isdigit(string str)
    {
        for(int i=0;i<52;i++)
        {
            if(digit[i]==str)
                return true;
        }
        return false;
    }
private:
    string letter[52];
};

int rightcom()
{
    string str;
    fin>>str;
    int len=int(str.length());
    int rightcomnum=-1;
    for(int j=0;j<len;j++)
    {
        if(str[j]=='*'&&str[j+1]=='/')
        {
            rightcomnum=j;
            break;
        }
    }
    if(rightcomnum>-1)
    {
        char str1[len-rightcomnum-2];
        for(int k=0;k<len-rightcomnum-2;k++)
        {
            str1[k]=str[rightcomnum+k+2];
        }
        testout(str1);
        return rightcomnum;
    }
    return -1;
}

void testout(string str)
{
    Scanner test;
    test.str=str;
    if(test.iskeyword(str))
    {
        cout<<str<<"\t\t"<<"$关键字"<<endl;
    }
    else if(test.issymbol(str))
    {
        cout<<str<<"\t\t"<<"$专用符号"<<endl;
    }
    else if(test.isletter(str))
    {
        cout<<str<<"\t\t"<<"$字母"<<endl;
    }
    else if(test.isdigit(str))
    {
        cout<<str<<"\t\t"<<"$数字"<<endl;
    }
    else
    {
        int len=str.length();
        int leftcomnum=-1;
        for(int i=0;i<len-1;i++)
        {
            if(str[i]=='/'&&str[i+1]=='*')
            {
                leftcomnum=i;
                break;
            }
        }
        bool un=false;
        int type0;
        int unum;
        Scanner test1;
        test1.str=str[0];
        if(test1.isdigit(test1.str)||test1.isletter(test1.str))
            type0=1;
        else if(test1.issymbol(test1.str))
            type0=2;
        for(int i=0;i<len;i++)
        {
            int typei;
            Scanner test2;
            test2.str=str[i];
            if(test2.isdigit(test2.str)||test2.isletter(test2.str))
                typei=1;
            else if(test2.issymbol(test2.str))
                typei=2;
            if(typei!=type0)
            {
                un=true;
                unum=i;
                break;
            }
        }
        if(leftcomnum>-1)
        {
            int rightcomnum=-1;
            for(int j=leftcomnum+2;j<len-1;j++)
            {
                if(str[j]=='*'&&str[j+1]=='/')
                {
                    rightcomnum=j;
                    break;
                }
            }
            if(rightcomnum>-1)
            {
                if(leftcomnum)
                {
                    char str1[leftcomnum];
                    for(int k=0;k<leftcomnum;k++)
                    {
                        str1[k]=str[k];
                    }
                    testout(str1);
                }
                if(len-rightcomnum-2)
                {
                    char str2[len-rightcomnum-2];
                    for(int k=0;k<len-rightcomnum-2;k++)
                    {
                        str2[k]=str[rightcomnum+k+2];
                    }
                    testout(str2);
                }
            }
            else
            {
                char str1[leftcomnum];
                for(int k=0;k<leftcomnum;k++)
                {
                    str1[k]=str[k];
                }
                testout(str1);
                int a=-1;
                while(a==-1)
                {
                    a=rightcom();
                }
            }
        }
        else if(un)
        {
            int len1=unum;
            string str1=str.substr(0,unum);
            testout(str1);
            int len2=len-unum;
            string str2=str.substr(unum,len);
            testout(str2);
        }
        else
        {
            if(str!="")
            {
                Scanner test1;
                test1.str=str[0];
                if(test1.isdigit(test1.str)||test1.isletter(test1.str))
                {
                    for(int i=0;i<len;i++)
                    {
                        Scanner test2;
                        test2.str=str[i];
                        if(test2.isdigit(test2.str)||test2.isletter(test2.str))
                            cout<<str[i];
                    }
                    cout<<"\t\t"<<"$自定义标识符"<<endl;
                }
                else if(test1.issymbol(test1.str))
                {
                    string str1=str.substr(0,1);
                    testout(str1);
                    string str2=str.substr(1,2);
                    testout(str2);
                }
            }
        }
    }
}

int main(int argc, const char * argv[])
{
    string fname;
    //cout<<"请输入欲分析的源代码地址：";
    //cin>>fname;
    fin.open("/Users/Jen/Desktop/test.txt");
    //fin.open(fname);
    cout<<"**********词法分析结果**********"<<endl;
    string str;
    while(fin>>str)
    {
        testout(str);
    }
    fin.close();
    return 0;
}