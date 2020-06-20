#pragma once
#include <conio.h>
#include <iostream>
#include "Mstring.h"
#include "Console.h"
class UserInput
{
public:
	UserInput(Console* console);
public:
	char inputCh(); //获取用户输入字符
	MString inputStr(int size); //获取用户输入字符串
	void clear(); //清空输入缓冲区
	MString intToStr(int num, int size,int base = 10);
	int strToInt(const char* str, int base = 10);

private:
	Console* console;
};

