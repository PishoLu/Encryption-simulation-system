#include "rsa.h"
#include <math.h>


/*
    ���ܣ����� RSA �㷨���������ַ������õ�����
    ���룺plaintext �����ܵ������ַ���
    ���룺pubkey ��Կ
    ����&�����ciphertext ���ص�����
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
    ���ܣ����� RSA �㷨�������ģ��õ������ַ���
    ���룺ciphertext ���ܺ������
    ���룺prikey ˽Կ
    ����&�����plaintext ���ص������ַ���
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
    ���ܣ�����a��b�η�ģn��ģ�ݣ��Ľ��
    ���룺a ���ķ���
    ���룺b ��Կ�� e �� ˽Կ��d
    ���룺n ��Կ�� n �� ˽Կ��n
    �������ģ���
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
    ���ܣ������ж�
    ���룺һ������
    �����1��ʾΪ������0��ʾ��Ϊ����
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
    ˵������e��n���ص�ʱ��e mod n�г˷���Ԫ
    ���ܣ�����չŷ������㷨��e mod n�ĳ˷���Ԫd����ed = 1 mod n
    ���룺e��n
    �����1��ʾ�г˷���Ԫ����ʱdΪ����ĳ˷���Ԫ
    �����0��ʾû�г˷���Ԫ����ʱdΪe��n���������
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
        if(y3 == 0)//����Ԫ
        {
            *d = x3;//��ʱx3Ϊe��n���������
            return 0;
        }
        if(y3 == 1)//����Ԫ��e��n���������Ϊ1
        {
            if(y2 < 0)
                y2 += n;
            *d = y2;//��ʱy2Ϊe mod n�ĳ˷���Ԫ
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

	// ����Ϸ��� p��q
	int p, q, e;
	while (1)
	{
		do 
		{
			printf("���� p��");
			scanf("%d", &p);
		} while (!isPrime(p));
		do
		{
			printf("���� q��");
			scanf("%d", &q);
		} while (!isPrime(q));
		if (p * q > 127)
			break;
		else
			printf("p��q �Ļ�С�� 128������������ p �� q��\n");
	};

	// ����Ϸ��� e
	int *d = NULL;
	int n = p * q;
	int f = (p - 1) * (q - 1);
	printf("���� e��");
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
			printf("û����Ԫ��\n");
		printf("���������� e��");
	} while (!d || e < 2 || e >= f);

	// ��ӡ��Կ��˽Կ
	printf("��Կ��(%d, %d)\n", pub_key->e, pub_key->n);
	printf("˽Կ��(%d, %d)\n", pri_key->d, pri_key->n);

	// ������ܵ�����
	printf("�������ַ�����\n");
	char string[100] = "\0";
	int chiper[100] = { 0 };
	int plain[100] = { 0 };
	scanf("%s", &string);
	int length = strlen(string);

	// ���ü��ܺ���
	RsaEncipher(string, length, chiper, *pub_key);
	printf("���ܺ�����������ǣ�\n");
	for (int i = 0; i < length - 1; i++)
		printf("%d, ", chiper[i]);
	printf("%d\n", chiper[length - 1]);

	// ���ý��ܺ���
	RsaDecipher(chiper, length, plain, *pri_key);
	printf("ͨ�����Ľ��ܵ������ǣ�\n");
	for (int i = 0; i < length - 1; i++)
		printf("%c", plain[i]);
	printf("%c\n", plain[length - 1]);

	return 0;
}