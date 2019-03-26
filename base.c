#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "base.h"
#include "time.h"
#include "des.h"
#include "md5.h"
#include "Menu.h"
#include "rsa.h"
//�������鹫��Ŀɿ���
int Check_text(notice one)
{ 
	char sign[17];
	char MD5[17];
	memset(sign, 0x00, 16);
	memset(MD5, 0x00, 16);
	list_user *temp = userhead;
	temp = temp->next;
	while (temp)
	{
		if (strcmp(temp->oneuser.name, one.sender) == 0)
		{
			RsaDecipher2(one.signature, 16, sign, temp->oneuser.mypubkey);
			md5(one.text, strlen(one.text), MD5);//����rsa��Կ����ǰ��õ����ĵ�md5�����õ����ĵ�md5�Ƚϡ���ͬ������ȷ��
			sign[16] = '\0';
			MD5[16] = '\0';
			if (strcmp(MD5, sign) == 0)
			{
				return 0;
			}
			temp = temp->next;
		}
		return 1;
	}
}

//���des�������Կ
int genRandomString(int length, char* ouput)
{
	int flag, i;
	srand((unsigned)time(NULL));
	for (i = 0; i < length - 1; i++)
	{
		flag = rand() % 3;
		switch (flag)
		{
		case 0:
			ouput[i] = 'A' + rand() % 26;
			break;
		case 1:
			ouput[i] = 'a' + rand() % 26;
			break;
		case 2:
			ouput[i] = '0' + rand() % 10;
			break;
		default:
			ouput[i] = 'x';
			break;
		}
	}
	return 0;
}
//���һ���µ�˽�Ž�㵽˽����������׷�ӵ��ռ��˵��ļ���
int send_letter(char name[], list_letter *letterhead)
{
	system("CLS");
	list_letter *temp = letterhead;
	list_letter *temp2 = Creatletter();
	while (temp->next)
		temp = temp->next;
	temp->next = temp2;
	temp2->next = NULL;//���µ�˽�ŷ�����������
	printf("-----------����˽��-----------\n\n");
	char text[200];
	printf("��ѡ���Ͷ���");
	scanf("%s", &temp2->oneletter.name);
	while (!check_name(temp2->oneletter.name))
	{
		printf("�û������ڣ�������ѡ��\n");
		scanf("%s", &temp2->oneletter.name);
	}
	list_user *usertemp = userhead;
	usertemp = usertemp->next;
	while (usertemp)
	{
		if (strcmp(usertemp->oneuser.name, name) == 0)
		{
			break;
		}
		usertemp = usertemp->next;
	}
	int strkey;
	char key[9] = { 0 };
	genRandomString(9, key);
	printf("���������ģ�");
	scanf("%s", &text);
	DES_Encrypt(text, strlen(text), key, temp2->oneletter.ciphertext);
	RsaEncipher(key, strlen(key), temp2->oneletter.ciphertext2, usertemp->oneuser.mypubkey);
	int i = 0;
	while (name[i])
	{
		temp2->oneletter.sender[i] = name[i++];
	}
	time_t rawtime;
	struct tm *timer;
	time(&rawtime);
	timer = localtime(&rawtime);
	strcpy(temp2->oneletter.time, asctime(timer));
	FILE *self = NULL;
	self = fopen(temp2->oneletter.name, "ab");
	fwrite(&temp2->oneletter, sizeof(letter), 1, self);//��˽�Ŵ����ռ��˵��ļ���
	fclose(self);
	return 0;
}
//�������ж�ȡ˽�Ų����
int receive_letter(char name[], list_letter *letterhead)
{
	system("CLS");
	list_letter *temp = letterhead;
	list_user *temp2 = userhead;
	printf("-----------����˽��-----------\n\n");
	if (letterhead->next == NULL)
	{
		printf("����˽�ţ�\n");
		system("pause");
		return 0;
	}
	char key[10];
	char plaintext[200] = "\0";
	RsaPriKey self = { 0,0 };
	while (temp2)
	{
		temp2 = temp2->next;
		if (strcmp(temp2->oneuser.name, name) == 0)
		{
			self.n = temp2->oneuser.mypubkey.n;
			break;
		}
	}
	printf("������������Կ��");
	scanf("%d", &self.d);
	temp = temp->next;
	int count = 0;
	while (temp)
	{
		if (strcmp(temp->oneletter.name, name) == 0)//��Ϊ���з��͸������˵�˽��Ҳ�Ǳ������������档�����ж��Ƿ��ǽ�����
		{
			printf("____________________________________________________________________________________________________________\n");
			printf("��ţ�%d\n", ++count);
			printf("\n�����ˣ�%s\n", temp->oneletter.sender);
			RsaDecipher(temp->oneletter.ciphertext2, 8, key, self);
			DES_Decrypt(temp->oneletter.ciphertext, strlen(temp->oneletter.ciphertext), key, plaintext);
			printf("���ģ�%s\n", plaintext);
			printf("ʱ�䣺%s\n", temp->oneletter.time);
		}
		temp = temp->next;
	}
	system("pause");
	return 0;
}
//��������
int send_notice(char name[])
{
	system("CLS");
	list_notice *temp = noticehead;
	list_user *temp2 = userhead;
	printf("-----------���͹���-----------\n\n");
	while (temp->next)
		temp = temp->next;
	temp->next = Creatnotice();//���¹���׷�ӵ������ĩβ
	temp = temp->next;
	int i = 0;
	while (name[i])
		temp->onenotice.sender[i] = name[i++];
	char MD5[16];
	RsaPriKey self;
	printf("�����빫�����ݣ�");
	scanf("%s", &temp->onenotice.text);
	md5(temp->onenotice.text, strlen(temp->onenotice.text), MD5);
	while (temp2)
	{
		temp2 = temp2->next;
		if (strcmp(temp2->oneuser.name, name) == 0)
		{
			self.n = temp2->oneuser.mypubkey.n;//����ռ��˵Ĺ�Կ
			break;
		}
	}
	printf("������������Կ��");
	scanf("%d", &self.d);
	RsaEncipher2(MD5, 16, temp->onenotice.signature, self);//�ù�Կ����md5�õ�ǩ��
	time_t rawtime;
	struct tm *timer;
	time(&rawtime);
	timer = localtime(&rawtime);
	strcpy(temp->onenotice.time, asctime(timer));
	FILE *fp;
	fp = fopen("notice", "ab");
	fwrite(&temp->onenotice, sizeof(notice), 1, fp);
	fclose(fp);
	return 0; 
}
//���չ���
int receive_notice(char name[])
{
	system("CLS");
	printf("-----------���չ���-----------\n\n");
	list_notice *temp = noticehead;
	if (temp->next == NULL)
	{
		printf("���޹��棡\n");
		system("pause");
		return 0;
	}
	temp = temp->next;
	int count = 0;
	int flag = 0;
	while (temp)
	{
		printf("____________________________________________________________________________________________________________\n\n");
		printf("��ţ�%d\n", ++count);
		printf("�����ˣ�%s\n", temp->onenotice.sender);
		printf("���棺%s\n", temp->onenotice.text);
		printf("����ʱ�䣺%s\n", temp->onenotice.time);
		flag = Check_text(temp->onenotice);
		if (!flag)
			printf("�ù����ѱ���֤��\n\n");
		else
			printf("�ù���δ����֤�ɹ���\n\n");
		temp = temp->next;
	}
	system("pause");
	return 0;
}
//��˽��������ѡ��ɾ��
int delete_letter (char name[], list_letter *letterhead)
{
	system("CLS");
	printf("-----------ɾ��˽��-----------\n\n");
	char sender[10]; 
	list_letter *temp = letterhead;
	if (!letterhead->next)
	{
		printf("û��˽�����ݣ�");
		return 0;
	}
	int choose = 0;
	int count = 0;
	receive_letter(name, letterhead);
	printf("��ѡ����ɾ��˽�ţ�");
	scanf("%d", &choose);
	while (temp)
	{
		count++;
		if (choose == count)
		{
			list_letter *temp2 = temp->next;
			temp->next = temp2->next;
			free(temp2);
		}
		temp = temp->next;
	}
	temp = letterhead->next;
	FILE *fp = NULL;
	fp = fopen(name, "wb");
	while (temp)
	{
		fwrite(&temp->oneletter, sizeof(letter), 1, fp);
		temp = temp->next;
	}
	fclose(fp);
	printf("ɾ����ɣ�\n");
	return 0;
}
//ɾ���Լ��������Ĺ���
int delete_notice(char name[])
{
	system("CLS");
	printf("-----------ɾ������-----------\n\n");
	printf("��ɾ������ֻ��ɾ�����Լ����͵Ĺ��棡\n\n");
	list_notice *temp = noticehead->next;
	if (!temp)
	{
		printf("û�й�����Ϣ�ɹ�ɾ����");
		return 0;
	}
	receive_notice(name);
	int choose = 0;
	printf("\n\n���Ϲ����Ϊ�Լ����ͣ���ѡ����Ҫɾ���Ĺ����ţ�");
	scanf("%d", &choose);
	temp = noticehead;
	int count2 = 0;
	while (temp)
	{
		if (!temp->next)//��Ϊ��temp->next,���Ϊ�վͲ���ʶ�������sender���������������󵥶��ж�
		{
			if (strcmp(temp->onenotice.sender, name) == 0)
			{
				count2++;
				if (count2 == choose)
				{
					list_notice *temp2 = temp->next;
					temp->next = NULL;
					free(temp2);
					temp = temp->next;
					continue;
				}
			}
		}
		if (temp->next)
		{
			if (strcmp(temp->next->onenotice.sender, name) == 0)
			{
				count2++;
				if (count2 == choose)
				{
					list_notice *temp2 = temp->next;
					temp->next = temp2->next;
					free(temp2);
					temp = temp->next;
					continue;
				}
			}
		}
		temp = temp->next;
	}
	temp = noticehead->next;
	FILE *fp = NULL;
	fp = fopen("notice", "wb");
	while (temp)
	{
		fwrite(&temp->onenotice, sizeof(notice), 1, fp);
		temp = temp->next;
	}
	fclose(fp);
	printf("ɾ����ɣ�\n");
	return 0;
}