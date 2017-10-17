//-----------------------------------------
//			本原根的求解
//date:2017-10-13
//create_by:haoming
//-----------------------------------------

//头文件
#include "stdafx.h"
#include<iostream>

using namespace std;

//函数声明
int get_rank(int num);//获取一个数的欧拉函数
bool isrp(int a, int b);//判断两个正整数是否互素
bool ispr(int a, int b);//判断a是否为b的本原根
int get_mod(int a, int n, int t);//迭代求模避免大数运算,返回a的t此方mod b
bool isrepeat(int a[], int len);//判断一个数组是否有重复元素


//主函数
int _tmain(int argc, _TCHAR* argv[])
{
	int num;
	cout<<"求一个数的本原根:"<<'\n'<<"请输入正整数;";
	cin>>num;
	cout<<num<<"的本源根为:"<<endl;
	for (int i = 2; i < num; i++)
	{
		if (ispr(i, num))
		{
			cout<<i<<" ";
		}
	}
	cout<<endl;
	return 0;
}

//判断一个数组是否有重复元素
bool isrepeat(int a[], int len)
{
	for (int i = 0; i < len; i++)
	{
		for (int j = i+1 ; j < len; j++)
		{
			if (a[i] == a[j])
			{
				return true;
			}
		}
	}
	return false;
}

//获取一个数的欧拉函数
int get_rank(int num)
{
	int result = 0;
	if (num <= 0)
	{
		return 0;//负数没有阶
	}

	for (int i = 1; i < num; i++)
	{
		if (isrp(i, num))
		{
			//互素,阶数加一
			result++;
		}
	}
	return result;
}

//判断两个正整数是否互素
bool isrp(int a, int b)
{
	if (a==b || a<=0 || b<=0)
	{
		return false;//不能相等或者小于零
	}
	else if(a==1 || b==1)
	{
		return true;//其中一个为一两数互素
	}
	else//辗转相除法求最大公约数
	{
		int t = 1;
		while (true)
		{
			t = a%b;
			if (t == 0)
			{
				break;
			}
			else
			{
				a = b;
				b = t;
			}
		}
		if (b == 1)
		{
			return true;//最大公约数为1，互素
		}
		else
		{
			return false;//不是互素
		}
	}

}

//判断a是否为b的本原根
bool ispr(int a, int b)
{
	int *t = new int[b];
	for (int i = 1; i <= get_rank(b); i++)
	{
		t[i] = get_mod(a, b, i);
		//【1】条件一：满足求模与b互素
		if ( isrp(t[i], b))
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	//【2】条件二：满足相互不同
	if ( isrepeat(t, get_rank(b)))//有重复
	{
		return false;
	}
	
	//两个条件都满足，判断为本元根
	return true;
}

//迭代求模避免大数运算,a的t此方mod b
int get_mod(int a, int n, int t)
{
	int r = 1;
	for (int i = 1 ; i <= t; i++)
	{
		r = (a*r)%n;
	}
	return  r;
} 

 
