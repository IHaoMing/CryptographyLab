# CryptographyLab

[TOC]

## lab1 维吉尼亚密码

>维吉尼亚密码（又译维热纳尔密码）是使用一系列凯撒密码组成密码字母表的加密算法，属于多表密码的一种简单形式。在一个凯撒密码中，字母表中的每一字母都会作一定的偏移，例如偏移量为3时，A就转换为了D、B转换为了E 维吉尼亚密码则是由一些偏移量不同的恺撒密码组成。


用数字0-25代替字母A-Z，维吉尼亚密码的加密文法可以写成同余的形式：

C[i] = M[i] + K[I] (mod26)

解密方法则能写成：

M[i] = C[i] - K[I] (mod26)

[维吉尼亚代码](https://github.com/IHaoMing/CryptographyLab/blob/master/lab1/verginia.cpp)

## lab2 RC4密码

>在密码学中，RC4（来自Rivest Cipher 4的缩写）是一种流加密算法，密钥长度可变。它加解密使用相同的密钥，因此也属于对称加密算法。RC4是有线等效加密（WEP）中采用的加密算法，也曾经是TLS可采用的算法之一。
由美国密码学家罗纳德·李维斯特（Ronald Rivest）在1987年设计的。由于RC4算法存在弱点，2015年2月所发布的 RFC 7465 规定禁止在TLS中使用RC4加密算法[1]。
RC4由伪随机数生成器和异或运算组成。RC4的密钥长度可变，范围是[1,255]。RC4一个字节一个字节地加解密。给定一个密钥，伪随机数生成器接受密钥并产生一个S盒。S盒用来加密数据，而且在加密过程中S盒会变化。
由于异或运算的对合性，RC4加密解密使用同一套算法。
```
初始化长度为256的S盒。第一个for循环将0到255的互不重复的元素装入S盒。第二个for循环根据密钥打乱S盒。
 for i from 0 to 255
     S[i] := i
 endfor
 j := 0
 for( i=0 ; i<256 ; i++)
     j := (j + S[i] + key[i mod keylength]) % 256
     swap values of S[i] and S[j]
 endfor
下面i,j是两个指针。每收到一个字节，就进行while循环。通过一定的算法（(a),(b)）定位S盒中的一个元素，并与输入字节异或，得到k。循环中还改变了S盒（(c)）。如果输入的是明文，输出的就是密文；如果输入的是密文，输出的就是明文。
 i := 0
 j := 0
 while GeneratingOutput:
     i := (i + 1) mod 256   //a
     j := (j + S[i]) mod 256 //b
     swap values of S[i] and S[j]  //c
     k := inputByte ^ S[(S[i] + S[j]) % 256]
     output K
 endwhile
此算法保证每256次循环中S盒的每个元素至少被交换过一次。
```
[RC4代码](https://github.com/IHaoMing/CryptographyLab/blob/master/lab1/virginia.cpp)
