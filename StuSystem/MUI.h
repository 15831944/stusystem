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
	void showMainInit(); //显示主界面
	void showTitleUi(const char* str);	//显示子标题
	void chooseUi();//显示删除界面
	void showDataTitle();//显示表标题
	void showData(MString& data);//显示数据
private:
	Console* console;
};


