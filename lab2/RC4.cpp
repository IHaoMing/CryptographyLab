// RC4.cpp : 定义控制台应用程序的入口点。
//
//-----------------RC4加密算法------------------------------------

//头文件声明
#include "stdafx.h"
#include<iostream>
#include<string>

using namespace std;

//函数声明
void swap_value(int &a, int &b);
void init_s(string k);
void display(int S[], int len);
string output(string input);


//显示s盒子的内容
void display(int S[], int len)
{
	cout<<"S[]:"<<endl;
	for (int i = 0; i < len; i++)
	{
		cout<<S[i]<<'\t';
	}
}

/*
----------【1】产生S盒子--------------------------------------

初始化长度为256的S盒。第一个for循环将0到255的互不重复的元素装入S盒。
第二个for循环根据密钥打乱S盒。

伪代码
 for i from 0 to 255
     S[i] := i
 endfor
 j := 0
 for( i=0 ; i<256 ; i++)
     j := (j + S[i] + key[i mod keylength]) % 256
     swap values of S[i] and S[j]
 endfor

*/
int S[256] = {0};

void init_s(string k)
{
	for (int i = 0; i < 256; i++)
	{
		S[i] = i;
	}
	int j = 0;
	for (int i = 0; i < 256; i++)
	{
		j = (j+S[i]+int(k[i%k.length()]))%256;//得到j的算法
		swap_value(S[i],S[j]);
	}
}

//字符转换为asscii码值
int chage_to_asc(char c)
{
	return int(c);
}
/*
----------------【2】加密解密----------------------------------------
下面i,j是两个指针。每收到一个字节，就进行while循环。通过一定的算法（(a),(b)）
定位S盒中的一个元素，并与输入字节异或，得到k。循环中还改变了S盒（(c)）。
如果输入的是明文，输出的就是密文；
如果输入的是密文，输出的就是明文。

伪代码
 i := 0
 j := 0
 while GeneratingOutput:
     i := (i + 1) mod 256   //a
     j := (j + S[i]) mod 256 //b
     swap values of S[i] and S[j]  //c
     k := inputByte ^ S[(S[i] + S[j]) % 256]
     output K
 endwhile
 */
string output(string input)
{
	int i = 0;
	int j = 0;
	int k;
	string output = input;
	for (int t= 0; t< input.length(); t++)
	{
		i = (i+1)%256;
		j = (j+S[i])%256;
		swap_value(S[i], S[j]);
		k = int(input[t])^S[ (S[i] + S[j])%256 ];
		output[t] = char(k);
	}
	return output;
}


//交换两个变量值
void swap_value(int &a, int &b)
{
	int temp = 0;
	temp = a;
	a = b;
	b = temp;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//密钥、明文、密文
	string k,m,c;
	
	//【1】输入明文和密钥
	cout<<"input the m"<<endl;
	cin>>m;

	cout<<"input the k:"<<endl;
	cin>>k;

	//【2】初始化S盒子
	init_s(k);

	//【3】产生输出
	c = output(m);
	cout<<c<<endl;

	//【2】初始化S盒子
	init_s(k);

	//【3】产生输出
	m = output(c);
	cout<<m<<endl;
	//display(S, 256);
	return 0;
}

