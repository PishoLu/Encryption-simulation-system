#include "rsa.h"
#include <math.h>


/*
    功能：采用 RSA 算法加密明文字符串，得到密文
    输入：plaintext 待加密的明文字符串
    输入：pubkey 公钥
    输入&输出：ciphertext 返回的密文
*/
void RsaEncipher(char *plaintext, long length, long *ciphertext, RsaPubKey pubkey)
{
    long i = 0;
    for (; i < length; i++){
        ciphertext[i] = modexp(plaintext[i], pubkey.e, pubkey.n);
    }
  return;
}

void RsaEncipher2(char *plaintext, long length, long *ciphertext, RsaPriKey prikey)
{
	long i = 0;
	for (; i < length; i++) {
		ciphertext[i] = modexp(plaintext[i], prikey.d, prikey.n);
	}
	return;
}
/*
    功能：采用 RSA 算法解密密文，得到明文字符串
    输入：ciphertext 加密后的密文
    输入：prikey 私钥
    输入&输出：plaintext 返回的明文字符串
*/
void RsaDecipher(long *ciphertext, long length, char *plaintext, RsaPriKey prikey)
{
    long i = 0;
    for (; i < length; i++){
        plaintext[i] = modexp(ciphertext[i], prikey.d, prikey.n);
    }
  return;
}

void RsaDecipher2(long *ciphertext, long length, char *plaintext, RsaPubKey pubkey)
{
	long i = 0;
	for (; i < length; i++) {
		plaintext[i] = modexp(ciphertext[i], pubkey.e, pubkey.n);
	}
	return;
}
/*
    功能：计算a的b次方模n（模幂）的结果
    输入：a 明文分组
    输入：b 公钥的 e 或 私钥的d
    输入：n 公钥的 n 或 私钥的n
    输出：幂模结果
*/
long modexp(long a, long b, long n)
{
    a = a % n;
    if(b == 0) return 1;
    if(b == 1) return a;
    if( b % 2 == 0){
        return ( modexp(a * a % n, b/2, n) % n );
    }
    if( b % 2 == 1){
        return ( a * modexp(a, (b-1), n) % n );
    }
    return 0;
}
/*
    功能：素数判断
    输入：一个整数
    输出：1表示为素数，0表示不为素数
*/
int isPrime(long number)
{
    int i = 2;
    int j = sqrt(number);
    for(; i <= j; i++)
    {
        if(number % i == 0)
        return 0;
    }
    return 1;
}

/*
    说明：当e和n互素的时候，e mod n有乘法逆元
    功能：用扩展欧几里得算法求e mod n的乘法逆元d，即ed = 1 mod n
    输入：e和n
    输出：1表示有乘法逆元，此时d为求出的乘法逆元
    输出：0表示没有乘法逆元，此时d为e和n的最大公因子
*/
int ExtendedEuclid(int e, int n, int *d)
{
    int x1, x2, x3, y1, y2, y3, t1, t2, t3, q;
    x1 = y2 = 1;
    x2 = y1 = 0;
    x3 = n;
    y3 = e;

    while(1)
    {
        if(y3 == 0)//无逆元
        {
            *d = x3;//此时x3为e和n的最大公因子
            return 0;
        }
        if(y3 == 1)//有逆元，e和n的最大公因子为1
        {
            if(y2 < 0)
                y2 += n;
            *d = y2;//此时y2为e mod n的乘法逆元
            return 1;
        }
        q = x3 / y3;
        t1 = x1 - q * y1;
        t2 = x2 - q * y2;
        t3 = x3 - q * y3;
        x1 = y1;
        x2 = y2;
        x3 = y3;
        y1 = t1;
        y2 = t2;
        y3 = t3;
    }
}

int Rsa_main(RsaPriKey *pri_key, RsaPubKey *pub_key)
{

	// 输入合法的 p、q
	int p, q, e;
	while (1)
	{
		do 
		{
			printf("输入 p：");
			scanf("%d", &p);
		} while (!isPrime(p));
		do
		{
			printf("输入 q：");
			scanf("%d", &q);
		} while (!isPrime(q));
		if (p * q > 127)
			break;
		else
			printf("p、q 的积小于 128，请重新输入 p 和 q！\n");
	};

	// 输入合法的 e
	int *d = NULL;
	int n = p * q;
	int f = (p - 1) * (q - 1);
	printf("输入 e：");
	do
	{
		scanf("%d", &e);
		ExtendedEuclid(f, e, d);
		if (d)
		{
			pub_key->e = e;
			pub_key->n = n;
			pri_key->d = d;
			pri_key->n = n;
			break;
		}
		else
			printf("没有逆元！\n");
		printf("请重新输入 e：");
	} while (!d || e < 2 || e >= f);

	// 打印公钥和私钥
	printf("公钥：(%d, %d)\n", pub_key->e, pub_key->n);
	printf("私钥：(%d, %d)\n", pri_key->d, pri_key->n);

	// 输入加密的内容
	printf("待加密字符串：\n");
	char string[100] = "\0";
	int chiper[100] = { 0 };
	int plain[100] = { 0 };
	scanf("%s", &string);
	int length = strlen(string);

	// 调用加密函数
	RsaEncipher(string, length, chiper, *pub_key);
	printf("加密后的密文内容是：\n");
	for (int i = 0; i < length - 1; i++)
		printf("%d, ", chiper[i]);
	printf("%d\n", chiper[length - 1]);

	// 调用解密函数
	RsaDecipher(chiper, length, plain, *pri_key);
	printf("通过密文解密的内容是：\n");
	for (int i = 0; i < length - 1; i++)
		printf("%c", plain[i]);
	printf("%c\n", plain[length - 1]);

	return 0;
}