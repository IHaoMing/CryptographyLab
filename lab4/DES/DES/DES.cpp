//-----------------------------------------
//    ����ѧʵ��;des����
//    date:2017-10-22
//    coded_by:haoming
//-----------------------------------------

//-----------------------------------------
//				����ͷ�ļ�
//-----------------------------------------
#include "stdafx.h"
#include<iostream>
#include<bitset>
#include<string>
#include<fstream>

using namespace std;

//-----------------------------------------
//			ȫ�ֱ���
//-----------------------------------------
bitset<64> key;                // 64λ��Կ
bitset<48> sub_key[16];        // ���16������Կ

//-----------------------------------------
//				��������
//-----------------------------------------
bitset<32> f(bitset<32> R, bitset<48> k);			//des���������f����
bitset<28> left_shift(bitset<28> k, int shift);		//��Կ���������е����ƺ���
void generateKeys();								//��Կ���ɺ���������16��48λ������Կ
bitset<64> char_to_bitset(const char s[8]);			//�ַ���ת������
bitset<64> encrypt(bitset<64>& plain);				//���ܺ���
bitset<64> decrypt(bitset<64>& cipher);				//���ܺ���



//--------------------------------------------
//			��������ʹ�õ��û���
//--------------------------------------------
//��1����ʼ�û���

int ip[] = { 58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 20, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17, 9,  1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7 };

// ��2��β�û���
int ip_1[] = { 40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41,  9, 49, 17, 57, 25 };


//----------------------------------------------
//				��Կʹ�õ��û���
//----------------------------------------------
// ��1����Կ�û�������64λ��Կ���56λ
int pc_1[] = { 57, 49, 41, 33, 25, 17, 9,
1, 58, 50, 42, 34, 26, 18,
10,  2, 59, 51, 43, 35, 27,
19, 11,  3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
7, 62, 54, 46, 38, 30, 22,
14,  6, 61, 53, 45, 37, 29,
21, 13,  5, 28, 20, 12,  4 };

// ��2��ѹ���û�����56λ��Կѹ����48λ����Կ
int pc_2[] = { 14, 17, 11, 24,  1,  5,
3, 28, 15,  6, 21, 10,
23, 19, 12,  4, 26,  8,
16,  7, 27, 20, 13,  2,
41, 52, 31, 37, 47, 55,
30, 40, 51, 45, 33, 48,
44, 49, 39, 56, 34, 53,
46, 42, 50, 36, 29, 32 };

// ��3��ÿ�����Ƶ�λ��
int shift_bits[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };


//-------------------------------------------------
//			des��������ʹ�õ��û���
//-------------------------------------------------
// ��1����չ�û������� 32λ ��չ�� 48λ
int expand[] = { 32,  1,  2,  3,  4,  5,
4,  5,  6,  7,  8,  9,
8,  9, 10, 11, 12, 13,
12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21,
20, 21, 22, 23, 24, 25,
24, 25, 26, 27, 28, 29,
28, 29, 30, 31, 32,  1 };

// ��2��S�У�ÿ��S����4x16���û�����6λ -> 4λ
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

// ��3��P�û���32λ -> 32λ
int p[] = { 16,  7, 20, 21,
29, 12, 28, 17,
1, 15, 23, 26,
5, 18, 31, 10,
2,  8, 24, 14,
32, 27,  3,  9,
19, 13, 30,  6,
22, 11,  4, 25 };




//������
int main()
{

	//-----------------------------------
	//			�ַ����ļӽ���
	//-----------------------------------

	//��1������
	string s = "12345678";
	string k = "ihaoming";
	bitset<64> plain = char_to_bitset(s.c_str());
	key = char_to_bitset(k.c_str());
	// ����16������Կ
	generateKeys();
	// ����д�� a.txt
	bitset<64> cipher = encrypt(plain);
	fstream file1;
	file1.open("D://a.txt", ios::binary | ios::out);
	file1.write((char*)&cipher, sizeof(cipher));
	file1.close();

	// ���ļ� a.txt
	bitset<64> temp;
	file1.open("D://a.txt", ios::binary | ios::in);
	file1.read((char*)&temp, sizeof(temp));
	file1.close();

	//��2������
	bitset<64> temp_plain = decrypt(temp);
	file1.open("D://b.txt", ios::binary | ios::out);
	file1.write((char*)&temp_plain, sizeof(temp_plain));
	file1.close();

	//�ļ��ļӽ���
	ifstream in;
	ofstream out;
	in.open("D://flower.jpg", ios::binary);
	out.open("D://cipher.txt", ios::binary);
	while (in.read((char*)&plain, sizeof(plain)))
	{
		bitset<64> cipher = encrypt(plain);
		out.write((char*)&cipher, sizeof(cipher));
		plain.reset();  // ��0
	}
	in.close();
	out.close();

	// ���� cipher.txt����д��ͼƬ flower1.jpg
	in.open("D://cipher.txt", ios::binary);
	out.open("D://flower1.jpg", ios::binary);
	while (in.read((char*)&plain, sizeof(plain)))
	{
		bitset<64> temp = decrypt(plain);
		out.write((char*)&temp, sizeof(temp));
		plain.reset();  // ��0
	}
	in.close();
	out.close();
    return 0;
}



//���������f��������
//���룺32λ������R,48λ������Կk
//�����32λ�����
bitset<32> f(bitset<32> R, bitset<48> k)
{
	bitset<48> expandR;
	// ��1��32λ������չ��48λ
	for (int i = 0; i<48; ++i)
		expandR[47 - i] = R[32 - expand[i]];
	// ��2������չ���������48λ������Կ���������
	expandR = expandR ^ k;
	// ��3��������s_box�û���
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
	// ��4����P-�û���32 -> 32
	bitset<32> tmp = output;
	for (int i = 0; i<32; ++i)
		output[31 - i] = tmp[32 - p[i]];
	return output;
}


//��Կ����������CD�����ֵ����Ʋ���
//����:28λ��Կ��λ�Ƴ���
//��������ƽ��
bitset<28> left_shift(bitset<28> k, int shift)
{
	//��ʱ����
	bitset<28> tmp = k;
	for (int i = 27; i >= 0; --i)
	{
		if (i - shift<0)//��ֹԽ��
			k[i] = tmp[i - shift + 28];
		else
			k[i] = tmp[i - shift];
	}
	return k;
}

//��Կ���ɺ���������16��48λ������Կ
void generateKeys()
{
	//��������
	bitset<56> realKey;
	bitset<28> left;
	bitset<28> right;
	bitset<48> compressKey;

	//��1�� ȥ����ż���λ����64λ��Կ���56λ
	for (int i = 0; i<56; ++i)
		realKey[55 - i] = key[64 - pc_1[i]];
	//��2�� ��������Կ�������� sub_keys[16] ��
	for (int round = 0; round<16; ++round)
	{
		// ǰ28λ���28λ
		for (int i = 28; i<56; ++i)
			left[i - 28] = realKey[i];
		for (int i = 0; i<28; ++i)
			right[i] = realKey[i];
		// ����
		left = left_shift(left, shift_bits[round]);
		right = left_shift(right, shift_bits[round]);
		// ѹ���û�����56λ�õ�48λ����Կ
		for (int i = 28; i<56; ++i)
			realKey[i] = left[i - 28];
		for (int i = 0; i<28; ++i)
			realKey[i] = right[i];
		for (int i = 0; i<48; ++i)
			compressKey[47 - i] = realKey[56 - pc_2[i]];
		sub_key[round] = compressKey;
	}
}

//��char�ַ�����תΪ������
//���룺����Ϊ8���ַ�����
//���:64λbitset����
//ע��const char s[8]��8*8 = 64bits
bitset<64> char_to_bitset(const char s[8])
{
	bitset<64> bits;
	for (int i = 0; i<8; ++i)
		for (int j = 0; j<8; ++j)
			//����һλ����1����ʵ�ְ�λ��ȡ������
			bits[i * 8 + j] = ((s[i] >> j) & 1);
	return bits;
}

//DES����
//���룺����plain
//���������cipher
bitset<64> encrypt(bitset<64>& plain)
{
	//��������
	bitset<64> cipher;
	bitset<64> currentBits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	// ��1������ʼ�û�IP
	for (int i = 0; i<64; ++i)
		currentBits[63 - i] = plain[64 - ip[i]];
	// ��2������ȡ Li �� Ri
	for (int i = 32; i<64; ++i)
		left[i - 32] = currentBits[i];
	for (int i = 0; i<32; ++i)
		right[i] = currentBits[i];
	// ��3������16�ֵ���
	for (int round = 0; round<16; ++round)
	{
		newLeft = right;
		right = left ^ f(right, sub_key[round]);
		left = newLeft;
	}
	// ��4�����ϲ�L16��R16��ע��ϲ�Ϊ R16L16
	for (int i = 0; i<32; ++i)
		cipher[i] = left[i];
	for (int i = 32; i<64; ++i)
		cipher[i] = right[i - 32];
	// ��5������β�û�IP-1
	currentBits = cipher;
	for (int i = 0; i<64; ++i)
		cipher[63 - i] = currentBits[64 - ip_1[i]];
	// ��������
	return cipher;
}


//DES����
//����:����cipher
//���:����:plain
bitset<64> decrypt(bitset<64>& cipher)
{
	//��������
	bitset<64> plain;
	bitset<64> currentBits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	// ��1������ʼ�û�IP
	for (int i = 0; i<64; ++i)
		currentBits[63 - i] = cipher[64 - ip[i]];
	// ��2������ȡ Li �� Ri
	for (int i = 32; i<64; ++i)
		left[i - 32] = currentBits[i];
	for (int i = 0; i<32; ++i)
		right[i] = currentBits[i];
	// ��3������16�ֵ���������Կ����Ӧ�ã�
	for (int round = 0; round<16; ++round)
	{
		newLeft = right;
		right = left ^ f(right, sub_key[15 - round]);
		left = newLeft;
	}
	// ��4�����ϲ�L16��R16��ע��ϲ�Ϊ R16L16
	for (int i = 0; i<32; ++i)
		plain[i] = left[i];
	for (int i = 32; i<64; ++i)
		plain[i] = right[i - 32];
	// ��5������β�û�IP-1
	currentBits = plain;
	for (int i = 0; i<64; ++i)
		plain[63 - i] = currentBits[64 - ip_1[i]];
	// ��������
	return plain;
}
