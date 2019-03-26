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
//用来检验公告的可靠性
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
			md5(one.text, strlen(one.text), MD5);//利用rsa公钥解密前面得到正文的md5，与获得的正文的md5比较。相同就是正确的
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

//获得des的随机密钥
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
//添加一个新的私信结点到私信链表，并且追加到收件人的文件中
int send_letter(char name[], list_letter *letterhead)
{
	system("CLS");
	list_letter *temp = letterhead;
	list_letter *temp2 = Creatletter();
	while (temp->next)
		temp = temp->next;
	temp->next = temp2;
	temp2->next = NULL;//将新的私信放在链表的最后
	printf("-----------发送私信-----------\n\n");
	char text[200];
	printf("请选择发送对象：");
	scanf("%s", &temp2->oneletter.name);
	while (!check_name(temp2->oneletter.name))
	{
		printf("用户不存在，请重新选择！\n");
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
	printf("请输入正文：");
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
	fwrite(&temp2->oneletter, sizeof(letter), 1, self);//将私信存入收件人的文件中
	fclose(self);
	return 0;
}
//从链表中读取私信并输出
int receive_letter(char name[], list_letter *letterhead)
{
	system("CLS");
	list_letter *temp = letterhead;
	list_user *temp2 = userhead;
	printf("-----------接收私信-----------\n\n");
	if (letterhead->next == NULL)
	{
		printf("暂无私信！\n");
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
	printf("请输入您的密钥：");
	scanf("%d", &self.d);
	temp = temp->next;
	int count = 0;
	while (temp)
	{
		if (strcmp(temp->oneletter.name, name) == 0)//因为会有发送给其他人的私信也是保存在链表里面。所以判断是否是接收人
		{
			printf("____________________________________________________________________________________________________________\n");
			printf("编号：%d\n", ++count);
			printf("\n发送人：%s\n", temp->oneletter.sender);
			RsaDecipher(temp->oneletter.ciphertext2, 8, key, self);
			DES_Decrypt(temp->oneletter.ciphertext, strlen(temp->oneletter.ciphertext), key, plaintext);
			printf("正文：%s\n", plaintext);
			printf("时间：%s\n", temp->oneletter.time);
		}
		temp = temp->next;
	}
	system("pause");
	return 0;
}
//发布公告
int send_notice(char name[])
{
	system("CLS");
	list_notice *temp = noticehead;
	list_user *temp2 = userhead;
	printf("-----------发送公告-----------\n\n");
	while (temp->next)
		temp = temp->next;
	temp->next = Creatnotice();//将新公告追加到链表的末尾
	temp = temp->next;
	int i = 0;
	while (name[i])
		temp->onenotice.sender[i] = name[i++];
	char MD5[16];
	RsaPriKey self;
	printf("请输入公告内容：");
	scanf("%s", &temp->onenotice.text);
	md5(temp->onenotice.text, strlen(temp->onenotice.text), MD5);
	while (temp2)
	{
		temp2 = temp2->next;
		if (strcmp(temp2->oneuser.name, name) == 0)
		{
			self.n = temp2->oneuser.mypubkey.n;//获得收件人的公钥
			break;
		}
	}
	printf("请输入您的密钥：");
	scanf("%d", &self.d);
	RsaEncipher2(MD5, 16, temp->onenotice.signature, self);//用公钥加密md5得到签名
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
//接收公告
int receive_notice(char name[])
{
	system("CLS");
	printf("-----------接收公告-----------\n\n");
	list_notice *temp = noticehead;
	if (temp->next == NULL)
	{
		printf("暂无公告！\n");
		system("pause");
		return 0;
	}
	temp = temp->next;
	int count = 0;
	int flag = 0;
	while (temp)
	{
		printf("____________________________________________________________________________________________________________\n\n");
		printf("编号：%d\n", ++count);
		printf("发布人：%s\n", temp->onenotice.sender);
		printf("公告：%s\n", temp->onenotice.text);
		printf("发布时间：%s\n", temp->onenotice.time);
		flag = Check_text(temp->onenotice);
		if (!flag)
			printf("该公告已被验证！\n\n");
		else
			printf("该公告未被验证成功！\n\n");
		temp = temp->next;
	}
	system("pause");
	return 0;
}
//在私信信箱中选择删除
int delete_letter (char name[], list_letter *letterhead)
{
	system("CLS");
	printf("-----------删除私信-----------\n\n");
	char sender[10]; 
	list_letter *temp = letterhead;
	if (!letterhead->next)
	{
		printf("没有私信内容！");
		return 0;
	}
	int choose = 0;
	int count = 0;
	receive_letter(name, letterhead);
	printf("请选择编号删除私信：");
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
	printf("删除完成！\n");
	return 0;
}
//删除自己发布过的公告
int delete_notice(char name[])
{
	system("CLS");
	printf("-----------删除公告-----------\n\n");
	printf("本删除功能只能删除由自己发送的公告！\n\n");
	list_notice *temp = noticehead->next;
	if (!temp)
	{
		printf("没有公告信息可供删除！");
		return 0;
	}
	receive_notice(name);
	int choose = 0;
	printf("\n\n以上公告均为自己发送，请选择需要删除的公告编号：");
	scanf("%d", &choose);
	temp = noticehead;
	int count2 = 0;
	while (temp)
	{
		if (!temp->next)//因为的temp->next,如果为空就不能识别下面的sender，所以在链表的最后单独判断
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
	printf("删除完成！\n");
	return 0;
}