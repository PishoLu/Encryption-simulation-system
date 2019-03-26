 
#ifndef __BASE_H__
#define __BASE_H__
#include <time.h>
#include "rsa.h"

//用户信息结构体
typedef struct User
{
	char name[10];
	char md5[17];
	RsaPubKey mypubkey;
}User_infor;
//私信信息 结构体
typedef struct letter
{
	char sender[10];//发件人
	char name[10];//收件人
	char ciphertext[200];//DES加密
	long ciphertext2[10];//RSA加密
	char time[30];//时间
}letter;
//公告信息结构体
typedef struct notice
{
	char sender[10];
	char text[200];
	long signature[16];
	char time[30];
}notice;
//私信链表的结点结构体
typedef struct node_letter
{
	letter oneletter;
	struct node_letter *next;
}list_letter;
//公告链表的结点结构体
typedef struct node_notice
{
	notice onenotice;
	struct node_notice *next;
}list_notice;
//用户链表的结点结构体
typedef struct node_user
{
	User_infor oneuser;
	struct node_user *next;
} list_user;
/*功能：判断公告内容是否由本人发送
参数：公告结构体的one
返回：判定成功返回1，不成功返回0*/
int Check_text(notice one);
/*功能：得到一个随机字符串，主要用于des生成随机密钥
参数：length：需要生成的随机字符串的长度， output：随机字符串的首地址
返回：完成返回0*/
int genRandomString(int length, char* ouput);
/*功能：发送私信
参数：name是发件人的名字也是登录用户的名字， letterhead是私信链表的头结点
返回：完成返回0*/
int send_letter(char name[], list_letter *letterhead);
/*功能：接收私信
参数：name是发件人的名字也是登录用户的名字， letterhead是私信链表的头结点
返回：完成返回0*/
int receive_letter(char name[], list_letter *letterhead);
/*功能：发送公告
参数：name，发送者的名字；
返回：完成返回0*/
int send_notice(char name[]);
/*功能：接收公告
参数：name，接收者的名字
返回：完成返回0*/
int receive_notice(char name[]);
/*功能：删除指定的私信
参数：name， 当前登录帐号的名字， letterhead：私信连接的头结点
返回：完成返回0；*/
int delete_letter(char name[], list_letter *letterhead);
/*功能：删除指定自己发送的公告
参数 ：name，当前登录帐号的名字
返回：完成返回0*/
int delete_notice(char name[]);
#endif // __BASE_H__
