 
#ifndef __BASE_H__
#define __BASE_H__
#include <time.h>
#include "rsa.h"

//�û���Ϣ�ṹ��
typedef struct User
{
	char name[10];
	char md5[17];
	RsaPubKey mypubkey;
}User_infor;
//˽����Ϣ �ṹ��
typedef struct letter
{
	char sender[10];//������
	char name[10];//�ռ���
	char ciphertext[200];//DES����
	long ciphertext2[10];//RSA����
	char time[30];//ʱ��
}letter;
//������Ϣ�ṹ��
typedef struct notice
{
	char sender[10];
	char text[200];
	long signature[16];
	char time[30];
}notice;
//˽������Ľ��ṹ��
typedef struct node_letter
{
	letter oneletter;
	struct node_letter *next;
}list_letter;
//��������Ľ��ṹ��
typedef struct node_notice
{
	notice onenotice;
	struct node_notice *next;
}list_notice;
//�û�����Ľ��ṹ��
typedef struct node_user
{
	User_infor oneuser;
	struct node_user *next;
} list_user;
/*���ܣ��жϹ��������Ƿ��ɱ��˷���
����������ṹ���one
���أ��ж��ɹ�����1�����ɹ�����0*/
int Check_text(notice one);
/*���ܣ��õ�һ������ַ�������Ҫ����des���������Կ
������length����Ҫ���ɵ�����ַ����ĳ��ȣ� output������ַ������׵�ַ
���أ���ɷ���0*/
int genRandomString(int length, char* ouput);
/*���ܣ�����˽��
������name�Ƿ����˵�����Ҳ�ǵ�¼�û������֣� letterhead��˽�������ͷ���
���أ���ɷ���0*/
int send_letter(char name[], list_letter *letterhead);
/*���ܣ�����˽��
������name�Ƿ����˵�����Ҳ�ǵ�¼�û������֣� letterhead��˽�������ͷ���
���أ���ɷ���0*/
int receive_letter(char name[], list_letter *letterhead);
/*���ܣ����͹���
������name�������ߵ����֣�
���أ���ɷ���0*/
int send_notice(char name[]);
/*���ܣ����չ���
������name�������ߵ�����
���أ���ɷ���0*/
int receive_notice(char name[]);
/*���ܣ�ɾ��ָ����˽��
������name�� ��ǰ��¼�ʺŵ����֣� letterhead��˽�����ӵ�ͷ���
���أ���ɷ���0��*/
int delete_letter(char name[], list_letter *letterhead);
/*���ܣ�ɾ��ָ���Լ����͵Ĺ���
���� ��name����ǰ��¼�ʺŵ�����
���أ���ɷ���0*/
int delete_notice(char name[]);
#endif // __BASE_H__
