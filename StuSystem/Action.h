#pragma once
#include <iostream>
#include <fstream>
#include <conio.h>
#include <climits>
#include <thread>
#include <functional>
#include <strsafe.h>
#include "MString.h"
#include "MFile.h"
#include "Mdata.h"
#include "UserInput.h"
#include "MTree.hpp"
#include "MUI.h"
#include "MException.h"
using namespace std;
extern bool isExit;
extern MTree<Mdata> tree;
extern MTree<Mdata> treeName;

//�������
void add(MString& data, int id);
//����ɾ��
bool del(int id);
//�ָ��ַ����е�ѧ�ź�����
void idAndName(MString& des, MString& id, MString& name);
//��ȡ�ļ��ַ���
MString read(Mdata& data);
//��������
void clearUp();
//��ȡ�û�����������������ں��Ա�
MString inputAllData(UserInput& input);
//��������
void loading();
void loadingName();
//�������ݵȴ�ui
void loadingUi(Console& cons);
//������,���Ʋ˵��͹��ܵ�����
void control(UserInput& input, MUI& mui, Console& cons);
//�����ַ���hashֵ
unsigned int mHash(char* str, unsigned int len);
//��ȡ����hash���ڵĽڵ�
MTree<Mdata>::TreeNode* findName(MString& name);

void loadingData(Console& cons);