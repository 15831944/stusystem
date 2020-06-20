#pragma once
#include <iostream>
#include <iomanip>
#include <strsafe.h>
#include "Console.h"
#include "MString.h"
using namespace std;
extern bool isExit;

class MUI
{
public:
	MUI(Console* console);
	~MUI();
private:
	void setSet(int width, int height, int x, int y);
	
public:
	void showMainInit(); //��ʾ������
	void showTitleUi(const char* str);	//��ʾ�ӱ���
	void chooseUi();//��ʾɾ������
	void showDataTitle();//��ʾ�����
	void showData(MString& data);//��ʾ����
private:
	Console* console;
};


