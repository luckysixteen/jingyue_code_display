//
//  scanner.cpp
//  compile_fjy
//
//  Created by 景玥 on 16/5/26.
//  Copyright (c) 2016年 Jen. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include<vector>
using namespace std;


Scanner :: Scanner() {
	scanSuccess = true;
	charIndex = 0;
	str = "";
	commentFlag = true;
	sourseString = "";
	lineCount = 0;
}

void Scanner :: getSourseString(string path) {
    ifstream fin(path.c_str());
    string temp;
    sourseString = "";
    
    while(getline(fin,temp)) {
        sourseString += temp;
        sourseString += '\n';
    }
    fin.close();
    charIndex = 0;
}

void Scanner :: deleteComments() {
    cout<<"--------分析开始--------"<<endl;
    cout<<"1.删除注释"<<endl;
    ofstream fout_Sourse("/Users/Jen/Documents/Development/compile_fjy/compile_fjy/sourseFile.txt");
    int state = 1;
    char ch;
    while(state<6) {
        ch = getNextChar();
        if(ch=='\0')
            break;
        if(state==1) {
            if(ch=='/')
                state = 2;
            else {
                state = 1;
                fout_Sourse<<ch; //状态1，非注释写入sourseFile文件
            }
        }
        else if(state==2) {
            if(ch=='*') {
                state = 3;
                commentFlag = false;
            }
            else {
                state = 1;
                fout_Sourse<<"/"<<ch;//回退，上一个状态都如的“／”写入sourseFile文件
            }
        }
        else if(state==3) {
            if(ch=='*')
                state = 4;
            else {
                state = 3;
            }
        }		
        else if(state==4) {
            if(ch=='*')
                state = 4;
            else if(ch=='/')
                state = 5;
            else {
                state = 3;
            }
        }		
        if(state==5) {
            commentFlag = true;
            state = 1;
        }
    }
    if(!commentFlag) {
        cout<<"Error: 注释没有结束符！"<<endl;
        scanSuccess = false;
    }
    else
        cout<<"  删除注释成功！已将无注释源码存入sourseFile.txt"<<endl;
}

void Scanner :: scan() {
	cout<<"2.词法分析"<<endl;
	getSourseString("/Users/Jen/Documents/Development/compile_fjy/compile_fjy/sourseFile.txt");
	int state = START;
	lineCount = 0;
	char ch;
    bool doubleSym = false;//判断符号是否为双标志符
	while(state<6) {
		ch = getNextChar();
		if(ch=='\0') {
			Token t;
			t.lineNo = lineCount;
			t.tokenString = "";
			t.tokenType = ENDFILE;
			tokenList.push_back(t);
			break;
		}

		if(state==START) {
			state = charType(ch);
			if(state!=START)
				str += ch;
		}
		else if(state==INNUM) {
			state = charType(ch);
			if(state!=INNUM)
				state = DONE;
			else
				str += ch;
		}		
		else if(state==INID) {
			state = charType(ch);
			if(state!=INID)
				state = DONE;
			else
				str += ch;
		}		
		else if(state==INSYMBOL) {
			if(ch=='='){
				str += ch;
				doubleSym = true;
			}
			else
				doubleSym = false;
			state = DONE;
		}
        
		if(DONE==state) {
			int flag = 0;
			if(ch=='\n')
				flag = 1;
            
			Token t;
			t.lineNo = lineCount-flag;
			t.tokenString = str;
			t.tokenType = getTokenType(str);
			tokenList.push_back(t);
            
			if(t.tokenType==ERROR)
				scanSuccess = false;
            
			int lastState = charType(str[str.length()-1]);
			if(lastState==INNUM || lastState==INID || (lastState==INSYMBOL && doubleSym==false))
				backToLastChar();
            
			str = "";
			state = START;
			if(doubleSym==true)
				doubleSym = false;
		}
     }

	if(scanSuccess==false)
		cout<<"  词法分析出错！"<<endl;
	else
		cout<<"  词法分析成功！已将词法分析结果存入Token.txt"<<endl;
	printToken();//输出Token到文件Token.txt中
}

DFAState Scanner ::  charType(char c) {
    //返回字符的类型，即词法分析DFA状态
    if(' '==c || '\n'==c || '\t'==c ||'\r'==c)
        return START;
    else if(c>='0'&&c<='9')
        return INNUM;
    else if((c>='A'&&c<='Z')||(c>='a'&&c<='z'))
        return INID;
    else if(c=='<' || c=='>' || c=='=' || c=='!')
        return INSYMBOL;
    else
        return DONE;
    
}

char Scanner :: getNextChar() {
    if(charIndex<sourseString.length()) {
        char ch =  sourseString[charIndex];
        charIndex++;
        if('\n'==ch)
            lineCount++;
        return ch;
    }
    else
        return '\0';
}

void Scanner :: backToLastChar() {
    if(charIndex>0) {
        char ch = sourseString[charIndex-1];
        charIndex--;
        if(ch=='\n')
            lineCount--;
    }
}

Token Scanner :: getTokenAt(int tokenIndex) {
	Token token;
	token.lineNo = lineCount;
	token.tokenString = "";
	token.tokenType = ENDFILE;
	if(tokenIndex<tokenList.size()) {
		token = tokenList.at(tokenIndex++);
	}
	return token;
}

TokenType Scanner ::  getTokenType(string s) {
	TokenType t;
	if(s=="else") {
		t = ELSE;
	}
	else if(s=="if") {
		t = IF;
	}
	else if(s=="int") {
		t = INT;
	}
	else if(s=="return") {
		t = RETURN;
	}
	else if(s=="void") {
		t = VOID;
	}
	else if(s=="while") {
		t = WHILE;
	}
	
	else if(s=="+") {
		t = PLUS;
	}
	else if(s=="-") {
		t = MINUS;
	}
	else if(s=="*") {
		t = TIMES;
	}
	else if(s=="/") {
		t = OVER;
	}
	else if(s=="<") {
		t = LT;
	}
	else if(s=="<=") {
		t = LEQ;
	}
	else if(s==">") {
		t = GT;
	}
	else if(s==">=") {
		t = GEQ;
	}
	else if(s=="==") {
		t = EQ;
	}
	else if(s=="!=") {
		t = NEQ;
	}
	else if(s=="=") {
		t = ASSIGN;
	}
	else if(s==";") {
		t = SEMI;
	}
	else if(s==",") {
		t = COMMA;
	}
	else if(s=="(") {
		t = LPAREN;
	}
	else if(s==")") {
		t = RPAREN;
	}
	else if(s=="[") {
		t = LMBRACKET;
	}
	else if(s=="]") {
		t = RMBRACKET;
	}
	else if(s=="{") {
		t = LBBRACKET;
	}
	else if(s=="}") {
		t = RBBRACKET;
	}
	else if(s=="/*") {
		t = LCOMMENT;
	}
	else if(s=="*/") {
		t = RCOMMENT;
	}
	else if(charType(s[s.length()-1])==2) {
		t = NUM;
	}
	else if(charType(s[s.length()-1])==3) {
		t = ID;
	}
	else {
		t = ERROR;
	}
	return t;
}

void Scanner :: printToken() {
	ofstream fout_Token("/Users/Jen/Documents/Development/compile_fjy/compile_fjy/Token.txt");
	ifstream fin("/Users/Jen/Documents/Development/compile_fjy/compile_fjy/sourseFile.txt");
	string temp;   
	int lineCount = 0;
	int index = 0;
	while(getline(fin,temp)) {
        //首先输出源代码及其行数
		fout_Token<<lineCount<<": ";
		fout_Token<<temp<<endl;
        
        //然后输出该行代码中各个Token的分析
		while(index<tokenList.size()) {
			Token t = tokenList.at(index);
			if(lineCount==t.lineNo) {
				fout_Token<<"     "<<lineCount<<":   [";
				index++;
				int width = 10;
				string headS = " ";
				if(t.tokenType>=1&&t.tokenType<=6) {//关键字keyWord
					string tp = "";
					for(int i = 0; i<width-t.tokenString.length(); i++)
						tp += " ";
					fout_Token<<"keyWord]："<<headS<<t.tokenString<<" "<<tp<<"["<<tokenTypeString[t.tokenType]<<"]"<<endl;
				}
				else if(t.tokenType>=7&&t.tokenType<=27)//符号symbols
				{
					string tp = "";
					for(int i = 0; i<width-t.tokenString.length(); i++)
						tp += " ";
					fout_Token<<"symbols]："<<headS<<t.tokenString<<" "<<tp<<"["<<tokenTypeString[t.tokenType]<<"]"<<endl;
				}
				else if(t.tokenType==28)//数字NUM
				{
					string tp = "";
					for(int i = 0; i<width-t.tokenString.length(); i++)
						tp += " ";
					fout_Token<<"    NUM]："<<headS<<t.tokenString<<" "<<tp<<"["<<tokenTypeString[t.tokenType]<<"]"<<endl;
				}
				else if(t.tokenType==29)//标示符ID
				{
					string tp = "";
					for(int i = 0; i<width-t.tokenString.length(); i++)
						tp += " ";
					fout_Token<<"     ID]："<<headS<<t.tokenString<<" "<<tp<<"["<<tokenTypeString[t.tokenType]<<"]"<<endl;
				}
				else if(t.tokenType==30)//错误error
				{
					string tp = "";
					for(int i = 0; i<width-t.tokenString.length(); i++)
						tp += " ";
					fout_Token<<"  error]："<<headS<<t.tokenString<<" "<<tp<<"["<<tokenTypeString[t.tokenType]<<"]"<<endl;
				}
				else if(t.tokenType==ENDFILE)//结束
				{
					fout_Token<<"     "<<lineCount<<": ";
					fout_Token<<t.tokenString<<" "<<"["<<tokenTypeString[t.tokenType]<<"]"<<endl;
				}
			}			
			if(lineCount<t.lineNo)
			break;
		}
		lineCount++;
	}
	fin.close();
	fout_Token.close();
}
