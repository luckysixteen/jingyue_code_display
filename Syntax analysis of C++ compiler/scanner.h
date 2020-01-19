//
//  scanner.h
//  compile_fjy
//
//  Created by 景玥 on 16/5/26.
//  Copyright (c) 2016年 Jen. All rights reserved.
//

#include<string>
#include<vector>
using namespace std;

//定义的Token的类型_31种
//包括：else、if、int、return、void、while、+、-、*、/、<、<=、>、>=、==、!=、=、;、,、(、)、[、]、{、}、/*、*/、num、id、错误、结束
typedef enum {
	OTHER,ELSE,IF,INT,RETURN,VOID,WHILE,
	PLUS,MINUS,TIMES,OVER,LT,LEQ,GT,GEQ,EQ,NEQ,ASSIGN,SEMI,COMMA,LPAREN,RPAREN,LMBRACKET,RMBRACKET,LBBRACKET,RBBRACKET,LCOMMENT,RCOMMENT,
	NUM,ID,ERROR,ENDFILE
} TokenType;

const string tokenTypeString[32] = {
    //每种TokenType对应的串，如tokenTypeString[ELSE]=="ELSE"
    "OTHER", "ELSE", "IF", "INT", "RETURN", "VOID", "WHILE", "PLUS", "MINUS", "TIMES", "OVER", "LT", "LEQ", "GT", "GEQ", "EQ", "NEQ", "ASSIGN", "SEMI", "COMMA", "LPAREN", "RPAREN", "LMBRACKET", "RMBRACKET", "LBBRACKET", "RBBRACKET", "LCOMMENT", "RCOMMENT", "NUM", "ID", "ERROR", "ENDFILE"};

struct Token {
    //定义Token结构体
    //包括：类型、字符串、所在代码的行号
	TokenType tokenType;
	string tokenString;
	int lineNo;
};

typedef enum {
    //定义DFA状态的类型_5种
    //包括：初始、数字、字母、符号、结束
    START=1, INNUM, INID, INSYMBOL, DONE
} DFAState;

/*********     Scanner类描述     *********/
class Scanner {
public:
    Scanner();
	void getSourseString(string s);//获取源代码
	void deleteComments();//删除注释
	void scan();//词法分析，将分析的Token放在tokenList数组中
	Token getTokenAt(int);//根据下标从tokenList数组中获取Token
    bool scanSuccess;//判断词法分析是否成功
	
private:
    string sourseString;//源代码字符串
    string str;//Token缓存串
    bool commentFlag;//判断注释是否结束的标志
    int charIndex;//配合getNextChar()，指定要取的字符位置
    int lineCount;//对行号计数，每次获取到'/n'就自增

	DFAState charType(char);
	char getNextChar();
	void backToLastChar();
    void printToken();//将词法分析好的Token输出到文件Token.txt中
    TokenType getTokenType(string s);//根据字符串返回Token类型
	vector<Token> tokenList;//保存的Token序列
};
