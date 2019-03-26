#ifndef __RSA_H__
#define __RSA_H__
//RSA��Կ
typedef struct PubKey
{
	long e;
	long n;
}RsaPubKey;
 
//RSA˽Կ
typedef struct PriKey
{
	long d;
	long n;
}RsaPriKey;

/*
���ܣ����� RSA �㷨���������ַ������õ�����
���룺plaintext �����ܵ������ַ���
���룺pubkey ��Կ
����&�����ciphertext ���ص�����
*/
void RsaEncipher(char *plaintext, long length, long *ciphertext, RsaPubKey pubkey);

void RsaEncipher2(char *plaintext, long length, long *ciphertext, RsaPriKey prikey);

/*
���ܣ����� RSA �㷨�������ģ��õ������ַ���
���룺ciphertext ���ܺ������
���룺prikey ˽Կ
����&�����plaintext ���ص������ַ���
*/
void RsaDecipher(long *ciphertext, long length, char *plaintext, RsaPriKey prikey);

void RsaDecipher2(long *ciphertext, long length, char*plaintext, RsaPubKey pubkey);

/*
���ܣ�����a��b�η�ģn��ģ�ݣ��Ľ��
���룺a ���ķ���
���룺b ��Կ�� e �� ˽Կ��d
���룺n ��Կ�� n �� ˽Կ��n
�������ģ���
*/
long modexp(long a, long b, long n);

/*
���ܣ������ж�
���룺һ������
�����1��ʾΪ������0��ʾ��Ϊ����
*/
int isPrime(long number);

/*
˵������e��n���ص�ʱ��e mod n�г˷���Ԫ
���ܣ�����չŷ������㷨��e mod n�ĳ˷���Ԫd����ed = 1 mod n
���룺e��n
�����1��ʾ�г˷���Ԫ����ʱdΪ����ĳ˷���Ԫ
�����0��ʾû�г˷���Ԫ����ʱdΪe��n���������
*/
int ExtendedEuclid(int e, int n, int *d);


int Rsa_main(RsaPriKey *pri_key, RsaPubKey *pub_key);

#endif // __RSA_H__
