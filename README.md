# CryptographyLab


## lab1 维吉尼亚密码

>维吉尼亚密码（又译维热纳尔密码）是使用一系列凯撒密码组成密码字母表的加密算法，属于多表密码的一种简单形式。在一个凯撒密码中，字母表中的每一字母都会作一定的偏移，例如偏移量为3时，A就转换为了D、B转换为了E 维吉尼亚密码则是由一些偏移量不同的恺撒密码组成。


用数字0-25代替字母A-Z，维吉尼亚密码的加密文法可以写成同余的形式：

C[i] = M[i] + K[I] (mod26)

解密方法则能写成：

M[i] = C[i] - K[I] (mod26)

[维吉尼亚代码](https://github.com/IHaoMing/CryptographyLab/blob/master/lab1/virginia.cpp)

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


## lab3 求解本源根

### 定义

素数p的原根定义
如果a是素数p的原根，则数a mod p, a^2 mod p, … , a^(p-1) mod p 是不同的并且包含1到p-1的整数的某种排列。
特别地，如果a是素数p的本原根，则a, a^2, …, a^(p-1)在 mod p下都不相同。

### 性质

若A为模n的本原根，则A，A的平方，A的3次方，……，A的φ(n)次方模n的余数互不相同，而且构成一个模n的简化剩余系。
应用编辑

### 应用

本原根可以证明：若x的[φ(n)/2]次方模n余1，则x为模n的二次剩余；若x的[φ(n)/2]次方模n余-1，则x为模n的非二次剩余。

[解本源根代码](https://github.com/IHaoMing/CryptographyLab/blob/master/lab3/source_root.cpp)

## lab4 DES加密

>des对称加密，对称加密，是一种比较传统的加密方式，其加密运算、解密运算使用的是同样的密钥，信息的发送者和信息的接收者在进行信息的传输与处理时，必须共同持有该密码（称为对称密码），是一种对称加密算法。

数据加密标准(Data Encryption Standard, DES)中的算法是第一个也是最重要的现代对称加密算法，其分组长度为64比特，使用的密钥长度为56比特(实际上函数要求一个64位的密钥作为输入，但其中用到的有效长度只有56位，剩余8位可作为奇偶校验位或完全随意设置)，DES加解密过程类似，加解密使用同样的算法，唯一不同的是解密时子密钥的使用次序要反过来。DES的整个体制是公开的，系统安全性完全依靠密钥的保密。
DES的运算可分为如下三步：

- (1)	对输入分组进行固定的“初始置换”IP，可写为(L0,R0)=IP(输入分组)，其中L0和R0称为“(左,右)半分组”，都是32比特的分组，IP是公开的固定的函数，无明显的密码意义。
- (2)	将下面的运算迭代16轮(i=1,2,…,16)：Li=Ri-1 ，Ri-1=Li-1 f(Ri-1,ki)；这里ki称为轮密钥，是56比特输入密钥的一个48比特字串，f称为S盒函数(S表示交换)，是一个代换密码，目的是获得很大程度的信息扩散。
- (3)	将十六轮迭代后得到的结果(L16 ,R16)输入到IP的逆置换来消除初始置换的影响，这一步的输出就是DES算法的输出，即输出分组=IP-1(R16 , L16)，此处在输入IP-1之前，16轮迭代输出的两个半分组又进行了一次交换。


DES的加密与解密算法都是用上述三个步骤，不同的是如果在加密算法中使用的轮密钥为k1, k2,…, k16，则解密算法中的轮密钥就应当是k16, k15,…, k1，可记为(k1`, k2`,…, k16`)=( k16, k15,…, k1)。
DES算法的一轮迭代处理过程如下图所示。

![des1](https://github.com/IHaoMing/MarkdownImage/blob/master/cipher/des1.PNG?raw=true)


DES的计算过程如下图所示。

![des2](https://github.com/IHaoMing/MarkdownImage/blob/master/cipher/des2.PNG?raw=true)


在加密密钥k下，将明文消息m加密为密文c，使用DES将c在k下解密为明文，解密过程如下：(L0`,R0`)=IP(c)=IP(IP-1(R16 , L16))，即(L0`,R0`)=(R16 , L16)；在第一轮中，L1`= R0`= L16 =R15，R1`=L0` f(R0` ,k1`)= R16 f(L16 ,k1`)=[ L16 f(R15 ,k16)] f(R15 ,k16)= L15，即(L1`,R1`)=(R15 , L15)；同样的，在接下来的15轮迭代中，可以得到(L2`,R2`)=(R14 , L14)，……，(L16`,R16`)=(R0 , L0)；最后一轮结束后，交换L16`和R16`，即(R16`,L16`)=( L0, R0)，IP-1(L0, R0)= IP-1(IP(m))=m，解密成功。

[des加密代码](https://github.com/IHaoMing/CryptographyLab/blob/master/lab4/DES/DES/DES.cpp)
