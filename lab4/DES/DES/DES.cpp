//-----------------------------------------
//    密码学实验;des加密
//    date:2017-10-22
//    coded_by:haoming
//-----------------------------------------

//-----------------------------------------
//				所需头文件
//-----------------------------------------
#include "stdafx.h"
#include<iostream>
#include<bitset>
#include<string>
#include<fstream>

using namespace std;

//-----------------------------------------
//			全局变量
//-----------------------------------------
bitset<64> key;                // 64位密钥
bitset<48> sub_key[16];        // 存放16轮子密钥

//-----------------------------------------
//				函数声明
//-----------------------------------------
bitset<32> f(bitset<32> R, bitset<48> k);			//des加密运算的f函数
bitset<28> left_shift(bitset<28> k, int shift);		//密钥产生操作中的左移函数
void generateKeys();								//密钥生成函数：生成16个48位的子密钥
bitset<64> char_to_bitset(const char s[8]);			//字符串转二进制
bitset<64> encrypt(bitset<64>& plain);				//加密函数
bitset<64> decrypt(bitset<64>& cipher);				//解密函数



//--------------------------------------------
//			输入数据使用的置换表
//--------------------------------------------
//【1】初始置换表

int ip[] = { 58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 20, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17, 9,  1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7 };

// 【2】尾置换表
int ip_1[] = { 40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41,  9, 49, 17, 57, 25 };


//----------------------------------------------
//				密钥使用的置换表
//----------------------------------------------
// 【1】密钥置换表，将64位密钥变成56位
int pc_1[] = { 57, 49, 41, 33, 25, 17, 9,
1, 58, 50, 42, 34, 26, 18,
10,  2, 59, 51, 43, 35, 27,
19, 11,  3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
7, 62, 54, 46, 38, 30, 22,
14,  6, 61, 53, 45, 37, 29,
21, 13,  5, 28, 20, 12,  4 };

// 【2】压缩置换，将56位密钥压缩成48位子密钥
int pc_2[] = { 14, 17, 11, 24,  1,  5,
3, 28, 15,  6, 21, 10,
23, 19, 12,  4, 26,  8,
16,  7, 27, 20, 13,  2,
41, 52, 31, 37, 47, 55,
30, 40, 51, 45, 33, 48,
44, 49, 39, 56, 34, 53,
46, 42, 50, 36, 29, 32 };

// 【3】每轮左移的位数
int shift_bits[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };


//-------------------------------------------------
//			des加密运算使用的置换表
//-------------------------------------------------
// 【1】扩展置换表，将 32位 扩展至 48位
int expand[] = { 32,  1,  2,  3,  4,  5,
4,  5,  6,  7,  8,  9,
8,  9, 10, 11, 12, 13,
12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21,
20, 21, 22, 23, 24, 25,
24, 25, 26, 27, 28, 29,
28, 29, 30, 31, 32,  1 };

// 【2】S盒，每个S盒是4x16的置换表，6位 -> 4位
int s_box[8][4][16] = {
	{
		{ 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
		{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
		{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
		{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 }
	},
	{
		{ 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
		{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
		{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
		{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 }
	},
	{
		{ 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
		{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
		{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
		{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 }
	},
	{
		{ 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
		{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
		{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
		{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 }
	},
	{
		{ 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
		{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
		{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
		{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 }
	},
	{
		{ 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
		{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
		{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
		{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 }
	},
	{
		{ 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
		{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
		{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
		{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 }
	},
	{
		{ 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
		{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
		{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
		{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 }
	}
};

// 【3】P置换，32位 -> 32位
int p[] = { 16,  7, 20, 21,
29, 12, 28, 17,
1, 15, 23, 26,
5, 18, 31, 10,
2,  8, 24, 14,
32, 27,  3,  9,
19, 13, 30,  6,
22, 11,  4, 25 };




//主函数
int main()
{

	//-----------------------------------
	//			字符串的加解密
	//-----------------------------------

	//【1】加密
	string s = "12345678";
	string k = "ihaoming";
	bitset<64> plain = char_to_bitset(s.c_str());
	key = char_to_bitset(k.c_str());
	// 生成16个子密钥
	generateKeys();
	// 密文写入 a.txt
	bitset<64> cipher = encrypt(plain);
	fstream file1;
	file1.open("D://a.txt", ios::binary | ios::out);
	file1.write((char*)&cipher, sizeof(cipher));
	file1.close();

	// 读文件 a.txt
	bitset<64> temp;
	file1.open("D://a.txt", ios::binary | ios::in);
	file1.read((char*)&temp, sizeof(temp));
	file1.close();

	//【2】解密
	bitset<64> temp_plain = decrypt(temp);
	file1.open("D://b.txt", ios::binary | ios::out);
	file1.write((char*)&temp_plain, sizeof(temp_plain));
	file1.close();

	//文件的加解密
	ifstream in;
	ofstream out;
	in.open("D://flower.jpg", ios::binary);
	out.open("D://cipher.txt", ios::binary);
	while (in.read((char*)&plain, sizeof(plain)))
	{
		bitset<64> cipher = encrypt(plain);
		out.write((char*)&cipher, sizeof(cipher));
		plain.reset();  // 置0
	}
	in.close();
	out.close();

	// 解密 cipher.txt，并写入图片 flower1.jpg
	in.open("D://cipher.txt", ios::binary);
	out.open("D://flower1.jpg", ios::binary);
	while (in.read((char*)&plain, sizeof(plain)))
	{
		bitset<64> temp = decrypt(plain);
		out.write((char*)&temp, sizeof(temp));
		plain.reset();  // 置0
	}
	in.close();
	out.close();
    return 0;
}



//加密运算的f函数处理
//输入：32位的数据R,48位的子密钥k
//输出：32位的输出
bitset<32> f(bitset<32> R, bitset<48> k)
{
	bitset<48> expandR;
	// 【1】32位数据扩展成48位
	for (int i = 0; i<48; ++i)
		expandR[47 - i] = R[32 - expand[i]];
	// 【2】：扩展后的数据与48位的子密钥作异或运算
	expandR = expandR ^ k;
	// 【3】：查找s_box置换表
	bitset<32> output;
	int x = 0;
	for (int i = 0; i<48; i = i + 6)
	{
		int row = expandR[47 - i] * 2 + expandR[47 - i - 5];
		int col = expandR[47 - i - 1] * 8 + expandR[47 - i - 2] * 4 + expandR[47 - i - 3] * 2
			+ expandR[47 - i - 4];
		int num = s_box[i / 6][row][col];
		bitset<4> binary(num);
		output[31 - x] = binary[3];
		output[31 - x - 1] = binary[2];
		output[31 - x - 2] = binary[1];
		output[31 - x - 3] = binary[0];
		x += 4;
	}
	// 【4】：P-置换，32 -> 32
	bitset<32> tmp = output;
	for (int i = 0; i<32; ++i)
		output[31 - i] = tmp[32 - p[i]];
	return output;
}


//密钥产生过程中CD两部分的左移操作
//输入:28位密钥，位移长度
//输出：左移结果
bitset<28> left_shift(bitset<28> k, int shift)
{
	//临时变量
	bitset<28> tmp = k;
	for (int i = 27; i >= 0; --i)
	{
		if (i - shift<0)//防止越界
			k[i] = tmp[i - shift + 28];
		else
			k[i] = tmp[i - shift];
	}
	return k;
}

//密钥生成函数：生成16个48位的子密钥
void generateKeys()
{
	//变量声明
	bitset<56> realKey;
	bitset<28> left;
	bitset<28> right;
	bitset<48> compressKey;

	//【1】 去掉奇偶标记位，将64位密钥变成56位
	for (int i = 0; i<56; ++i)
		realKey[55 - i] = key[64 - pc_1[i]];
	//【2】 生成子密钥，保存在 sub_keys[16] 中
	for (int round = 0; round<16; ++round)
	{
		// 前28位与后28位
		for (int i = 28; i<56; ++i)
			left[i - 28] = realKey[i];
		for (int i = 0; i<28; ++i)
			right[i] = realKey[i];
		// 左移
		left = left_shift(left, shift_bits[round]);
		right = left_shift(right, shift_bits[round]);
		// 压缩置换，由56位得到48位子密钥
		for (int i = 28; i<56; ++i)
			realKey[i] = left[i - 28];
		for (int i = 0; i<28; ++i)
			realKey[i] = right[i];
		for (int i = 0; i<48; ++i)
			compressKey[47 - i] = realKey[56 - pc_2[i]];
		sub_key[round] = compressKey;
	}
}

//将char字符数组转为二进制
//输入：长度为8的字符数组
//输出:64位bitset对象
//注：const char s[8]，8*8 = 64bits
bitset<64> char_to_bitset(const char s[8])
{
	bitset<64> bits;
	for (int i = 0; i<8; ++i)
		for (int j = 0; j<8; ++j)
			//左移一位再与1相与实现按位存取二进制
			bits[i * 8 + j] = ((s[i] >> j) & 1);
	return bits;
}

//DES加密
//输入：明文plain
//输出：密文cipher
bitset<64> encrypt(bitset<64>& plain)
{
	//变量声明
	bitset<64> cipher;
	bitset<64> currentBits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	// 【1】：初始置换IP
	for (int i = 0; i<64; ++i)
		currentBits[63 - i] = plain[64 - ip[i]];
	// 【2】：获取 Li 和 Ri
	for (int i = 32; i<64; ++i)
		left[i - 32] = currentBits[i];
	for (int i = 0; i<32; ++i)
		right[i] = currentBits[i];
	// 【3】：共16轮迭代
	for (int round = 0; round<16; ++round)
	{
		newLeft = right;
		right = left ^ f(right, sub_key[round]);
		left = newLeft;
	}
	// 【4】：合并L16和R16，注意合并为 R16L16
	for (int i = 0; i<32; ++i)
		cipher[i] = left[i];
	for (int i = 32; i<64; ++i)
		cipher[i] = right[i - 32];
	// 【5】：结尾置换IP-1
	currentBits = cipher;
	for (int i = 0; i<64; ++i)
		cipher[63 - i] = currentBits[64 - ip_1[i]];
	// 返回密文
	return cipher;
}


//DES解密
//输入:密文cipher
//输出:明文:plain
bitset<64> decrypt(bitset<64>& cipher)
{
	//变量声明
	bitset<64> plain;
	bitset<64> currentBits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	// 【1】：初始置换IP
	for (int i = 0; i<64; ++i)
		currentBits[63 - i] = cipher[64 - ip[i]];
	// 【2】：获取 Li 和 Ri
	for (int i = 32; i<64; ++i)
		left[i - 32] = currentBits[i];
	for (int i = 0; i<32; ++i)
		right[i] = currentBits[i];
	// 【3】：共16轮迭代（子密钥逆序应用）
	for (int round = 0; round<16; ++round)
	{
		newLeft = right;
		right = left ^ f(right, sub_key[15 - round]);
		left = newLeft;
	}
	// 【4】：合并L16和R16，注意合并为 R16L16
	for (int i = 0; i<32; ++i)
		plain[i] = left[i];
	for (int i = 32; i<64; ++i)
		plain[i] = right[i - 32];
	// 【5】：结尾置换IP-1
	currentBits = plain;
	for (int i = 0; i<64; ++i)
		plain[63 - i] = currentBits[64 - ip_1[i]];
	// 返回明文
	return plain;
}

