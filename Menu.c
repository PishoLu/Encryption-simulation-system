#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "md5.h"
#include "rsa.h"
#include "Menu.h"
#include "base.h"
//创建一个私信的结点
list_letter *Creatletter()
{
	list_letter *node = (list_letter *)malloc(sizeof(list_letter));
	if (!node) 
	{
		printf("内存分配失败！");
		return NULL;
	}
	memset(node, 0x00, sizeof(list_letter));
	return node;
}
//创建一个公告的结点
list_notice *Creatnotice()
{
	list_notice *node = (list_notice *)malloc(sizeof(list_notice));
	if (!node)
	{
		printf("内存分配失败！");
		return NULL;
	}
	memset(node, 0x00, sizeof(list_notice));
	return node;
}
//创建一个用户的结点
list_user *Creatuser()
{
	list_user *node = (list_user *)malloc(sizeof(list_user));
	if (!node)
	{
		printf("内存分配失败！");
		return NULL;
	}
	memset(node, 0x00, sizeof(list_user));
	return node;
}

void menu()
{
	userhead = (list_user *)malloc(sizeof(list_user));
	noticehead = (list_notice *)malloc(sizeof(list_notice));
	memset(userhead, 0x00, sizeof(list_user));
	memset(noticehead, 0x00, sizeof(list_notice));
	ready_user();
	ready_notice();//程序运行开始，初始化全局变量公告链表和用户链表。
	int choose;
	while (1)
	{
		system("CLS");
		printf("欢迎使用本系统\n\n");
		printf("1 注册成为用户\n");
		printf("2 登陆\n");
		printf("3 退出\n");
		printf("您的选择是：");
		scanf("%d", &choose);
		switch (choose)
		{
		case 1:
			registered();
			break;
		case 2:
			login();
			break;
		case 3:
			exit(0);
		default:
			continue;
		}
	}
}
//检查是否存在相同的用户名
int check_name(char name[])
{
	list_user *temp = userhead->next;
	while (temp)
	{
		if (strcmp(temp->oneuser.name, name) == 0)
			return 1;
		temp = temp->next;
	}
	return 0;
}
//用户注册
int registered()
{
	system("CLS");
	printf("-----------注册-----------\n\n");
	list_user *temp = userhead;
	while (temp->next)
		temp = temp->next;
	temp->next = Creatuser();
	temp = temp->next;//在链表末尾追加用户结点
	RsaPriKey myprikey;
	char name[10];
	printf("请输入您的用户名：");
	scanf("%s", &name);
	while (check_name(name))
	{
		printf("已存在相同用户名，请重新选择用户名！");
		scanf("%s", &name);
	}
	strcpy(temp->oneuser.name, name);
	char password[16] = "\0";
	printf("请输入密码：");
	scanf("%s", &password);
	md5(password, strlen(password), temp->oneuser.md5);
	temp->oneuser.md5[16] = '\0';
	Rsa_main(&myprikey, &temp->oneuser.mypubkey);
	FILE *file = NULL;
	if ((file = fopen("User", "ab")) == NULL)
	{
		printf("文件打开失败！");
		system("pause");
		return 0;
	}
	fwrite(&temp->oneuser, sizeof(User_infor), 1, file);//将用户结点追加写入文件
	fclose(file);
	system("pause");
	return 0;
}
//用户登录
int login()
{
	system("CLS");
	list_user *temp = userhead;
	printf("-----------登录-----------\n\n");
	if (userhead->next == NULL)
	{
		printf("暂无用户注册！\n");
		system("pause");
		return 0;
	}
	printf("已有如下用户：\n");
	temp = temp->next;
	while (temp)
	{
		printf("%s\n", temp->oneuser.name);
		temp = temp->next;
	}
	temp = userhead;
	printf("\n请从上面的用户名中选择您的用户名输入：");
	char name[10];
	scanf("%s", &name);
	if (!check_name(name))
	{
		printf("\n用户不存在！\n");
		scanf("%s", &name);
	}
	printf("请输入密码：");
	char password[16];
	int flag = 0;
	while (password[flag] != 13)
	{
		password[flag] = getch();
		if (password[flag] == 13)
			break;
		if (password[flag] == 8 && flag > 0)
		{
			flag--;
			printf("\b \b");
			continue;
		}
		if (password[flag] == 8 && flag == 0)
			continue;
		else
		{
			printf("*");
			flag++;
			continue;
		}
	}
	password[flag] = '\0';
	char MD5[17];
	md5(password, strlen(password), MD5);
	MD5[16] = '\0';
	temp = temp->next;
	while (temp)
	{
		if (strcmp(temp->oneuser.name, name) == 0)
		{
			if (strcmp(temp->oneuser.md5, MD5) == 0)
			{
				fun_menu(name);//判断用户的用户名和密码的md5是否一致，一致则通过登录进入功能菜单
				return 0;
			}
		}
		temp = temp->next;
	}
	printf("\n登陆失败！\n");
	system("pause");
	return 0;
}
//功能菜单
int fun_menu(char name[])
{
	list_letter *letterhead = NULL;
	letterhead = (list_letter *)malloc(sizeof(list_letter));
	memset(letterhead, 0x00, sizeof(list_letter));
	ready_letter(name, letterhead);//通过登录的用户名初始化私信链表
	int choose;
	while (1)
	{
		system("CLS");
		printf("-----------功能-----------\n\n");
		printf("\n\n1 发送私信\n");
		printf("2 接收私信\n");
		printf("3 发布公告\n");
		printf("4 查看公告\n");
		printf("5 删除私信\n");
		printf("6 删除自己发送的公告\n");
		printf("7 退回到登陆界面\n");
		printf("您的选择是：");
		scanf("%d", &choose);
		switch (choose)
		{
		case 1:
			send_letter(name, letterhead);
			break;
		case 2:
			receive_letter(name, letterhead);
			break;
		case 3:
			send_notice(name);
			break;
		case 4:
			receive_notice(name);
			break;
		case 5:
			delete_letter(name, letterhead);
			break;
		case 6:
			delete_notice(name);
			break;
		case 7:
			return 0;
		default:
			continue;
		}
	}
}
//创建一个私信的链表。
int ready_letter(char name[], list_letter *letterhead)
{
	list_letter *temp = letterhead;
	FILE *letter_f = NULL;
	letter_f = fopen(name, "rb");
	if (letter_f == NULL)
	{
		return 0;
	}
	fseek(letter_f, 0, SEEK_END);
	int count = ftell(letter_f);
	count = count / sizeof(letter);
	fseek(letter_f, 0, SEEK_SET);
	for (int i = 0; i < count; i++)
	{
		temp->next = Creatletter();
		fread(&temp->next->oneletter, sizeof(letter), 1, letter_f);
		temp = temp->next;
	}//创建结点并读取用户信息文件。把文件信息保存在结点中
	fclose(letter_f);
	return 0;
}
//创建公告的链表
int ready_notice()
{
	list_notice *temp = noticehead;
	FILE *notice_f = NULL;
	notice_f = fopen("notice", "rb");
	if (!notice_f)
	{
		return 0;
	}
	fseek(notice_f, 0, SEEK_END);
	int count = ftell(notice_f);
	count = count / sizeof(notice);
	fseek(notice_f, 0, SEEK_SET);
	for (int i = 0; i < count; i++)
	{
		temp->next = Creatnotice();
		fread(&temp->next->onenotice, sizeof(notice), 1, notice_f);
		temp = temp->next;
	}
	fclose(notice_f);
	return 0;
}
//创建用户的链表
int ready_user()
{
	list_user *temp = userhead;
	FILE *user_f = NULL;
	user_f = fopen("User", "rb");
	if (user_f == NULL)
	{
		return 0;
	}
	fseek(user_f, 0, SEEK_END);
	int count = ftell(user_f);
	count = count / sizeof(User_infor);
	fseek(user_f, 0, SEEK_SET);
	for (int i = 0; i < count; i++)
	{
		temp->next = Creatnotice();
		fread(&temp->next->oneuser, sizeof(User_infor), 1, user_f);
		temp = temp->next;
	}
	fclose(user_f);
	return 0;
}