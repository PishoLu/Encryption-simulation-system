#ifndef __MENU_H__
#define __MENU_H__
#include "base.h"
#include "rsa.h"
#include <malloc.h>

//����˽������
list_letter *Creatletter();
//������������
list_notice *Creatnotice();
//�����û�����
list_user *Creatuser();
/*���ܣ���¼����˵�*/
void menu();
/*���ܣ�ע���û�*/
int registered();
/*��¼�ʺ�*/
int login();
/*���ܣ����ܽ���˵�  
������name����ǰ��¼���û���*/
int fun_menu(char name[]);
/*���ܣ��ж��Ƿ�����û���
������name����Ҫ�жϵ��û���
���أ����ڷ���1�������ڷ���0*/
int check_name(char name[]);
/*����˽������*/
int ready_letter(char name[], list_letter *userhead);
/*���ɹ�������*/
int ready_notice();
/*�����û�����*/
int ready_user();
//�û�ͷ���
list_user *userhead;
//����ͷ���
list_notice *noticehead;
#endif
