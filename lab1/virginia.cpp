// veriginia.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<string>

using namespace std;

//加密过程
string m_to_c(string m, string k)
{
	string c = m;
	if (m.length() != k.length())
	{
		printf("明文和秘钥的长度不相等!");
		return "error!";
	}
	
	for (int i = 0; i < c.length(); i++)
	{
		int t = 0;//位移量
		//计算位移量
		if (k[i] >= 'a' && k[i] <= 'z')
		{
			t = k[i] - 'a';
		}
		else if (k[i] >= 'A' && k[i] <= 'Z')
		{
			t = k[i] - 'A';
		}
		else
		{
			t =  k[i]-48;
		}
		//位移替换
		if (m[i] >= 'a' &&  m[i] <= 'z')
		{
			c[i] = char((m[i] - 'a' + t)%26 + 97);
		}else if (m[i] >= 'A' && m[i] <= 'Z')
		{
			c[i] = char((m[i] - 'A' + t)%26 + 65);
		}
		else
		{
			c[i] = m[i];//明文为数字不加密
		}

		
	}

	return  c;
}

//解密过程
string c_to_m(string c, string k)
{
	string m = c;
	if (c.length() != k.length())
	{
		printf("密文和秘钥的长度不相等!");
		return "error!";
	}
	
	for (int i = 0; i < c.length(); i++)
	{
		int t = 0;//位移量
		//计算位移量
		if (k[i] >= 'a' && k[i] <= 'z')
		{
			t = k[i] - 'a';
		}
		else if (k[i] >= 'A' && k[i] <= 'Z')
		{
			t = k[i] - 'A';
		}
		else
		{
			t =  k[i]-48;
		}
		//位移替换
		if (c[i] >= 'a' &&  c[i] <= 'z')
		{
			m[i] = char((c[i] - 'a' - t)%26 + 97);
		}else if (c[i] >= 'A' && c[i] <= 'Z')
		{
			m[i] = char((c[i] - 'A' - t)%26 + 65);
		}
		else
		{
			m[i] = c[i];//明文为数字不加密
		}

		
	}

	return  m;
}

//菜单
int menu()
{
		string m;//明文
		string c;//密文
		string k;//秘钥
		cout<<"---------------------------\n\t维吉尼亚密码实验"<<endl;
		cout<<"---------------------------"<<endl;
		cout<<"\t1、加密\n\t2、解密\n\t3、清屏\n\t0、退出"<<endl;
		cout<<"---------------------------"<<endl;
		int opt = 0;
		cin>>opt;

		switch (opt)
		{
		case 1:
			cout<<"请输入明文："<<endl;
			cin>>m;
			cout<<"请输入秘钥:"<<endl;
			cin>>k;
			c = m_to_c(m, k );
			cout<<"密文："<<c<<endl;
			break;
		case 2:
			cout<<"请输入密文："<<endl;
			cin>>c;
			cout<<"请输入秘钥:"<<endl;
			cin>>k;
			m = c_to_m(c, k);
			cout<<"明文:"<<m<<endl;
			break;
		case 3:
			system("cls");
			break;
		case 0:
			break;
		default:
			cout<<"请输入正确的选项!"<<endl;
			break;
		}
		return opt;
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	while (menu())
	{

	}
	return 0;
}

