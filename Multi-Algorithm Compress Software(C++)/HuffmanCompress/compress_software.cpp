//
//  compress_software.cpp
//  HuffmanCompress
//
//  Created by 景玥 on 14-12-17.
//  Copyright (c) 2014年 Jen. All rights reserved.
//

#include "utility.h"
#include "string.h"
#include "lk_list.h"
#include "node.h"
#include "min_priority_heap_queue.h"
#include "compress_software.h"
#include <iostream>

int main(void)
{
	MyHuffmanCompress obj;
	int m,iContiune = 1;
	
    std::cout<<"==========================================="<<std::endl;
    std::cout<<"|           多媒体技术基础课程实验            |"<<std::endl;
    std::cout<<"|          Huffman 编码与译码的实现          |"<<std::endl;
    std::cout<<"|           付景玥 201314146****           |"<<std::endl;
    std::cout<<"==========================================="<<std::endl;
	while(iContiune == 1)
	{
		std::cout << "          1.  压缩文件" << std::endl;
		std::cout << "          2.  解压文件" << std::endl;
		std::cout << "          3.  退出程序" << std::endl<< std::endl;
        
        
        
		std::cout << "请输入您的选择：";
        
		
		std::cin >> m;
        
		switch(m)
		{
            case 1:
                obj.Compress();
                break;
            case 2:
                obj.Compress();
                break;
            case 3:
                std::cout << std::endl;
                std::cout<<"==========================================="<<std::endl;
                std::cout<<"|               多媒体技术基础              |"<<std::endl;
                std::cout<<"|                2014年12月               |"<<std::endl;
                std::cout<<"|           付景玥-201314146****           |"<<std::endl;
                std::cout<<"==========================================="<<std::endl;
                std::cout << std::endl;
                system("PAUSE");
                exit(1);
                break;
            default:
                std::cout << "您的输入有误!"  << std::endl;
		}
        
		std::cout <<"是否继续使用本程序？（-继续使用；-退出）:" ;
		std::cin >> iContiune;
		std::cout << std::endl;
	}
    
	std::cout << std::endl;
	std::cout<<"==========================================="<<std::endl;
    std::cout<<"|               多媒体技术基础              |"<<std::endl;
    std::cout<<"|                2014年12月               |"<<std::endl;
    std::cout<<"|           付景玥-201314146****           |"<<std::endl;
    std::cout<<"==========================================="<<std::endl;
	std::cout << std::endl;
	//system("PAUSE");
	return 0;
}
