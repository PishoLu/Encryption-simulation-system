#ifndef __MENU_H__
#define __MENU_H__
#include "base.h"
#include "rsa.h"
#include <malloc.h>

//创建私信链表
list_letter *Creatletter();
//创建公告链表
list_notice *Creatnotice();
//创建用户链表
list_user *Creatuser();
/*功能：登录界面菜单*/
void menu();
/*功能：注册用户*/
int registered();
/*登录帐号*/
int login();
/*功能：功能界面菜单  
参数：name：当前登录的用户名*/
int fun_menu(char name[]);
/*功能：判断是否存在用户名
参数：name：需要判断的用户名
返回：存在返回1，不存在返回0*/
int check_name(char name[]);
/*生成私信链表*/
int ready_letter(char name[], list_letter *userhead);
/*生成公告链表*/
int ready_notice();
/*生成用户链表*/
int ready_user();
//用户头结点
list_user *userhead;
//公告头结点
list_notice *noticehead;
#endif
