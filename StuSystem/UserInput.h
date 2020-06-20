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
	char inputCh(); //��ȡ�û������ַ�
	MString inputStr(int size); //��ȡ�û������ַ���
	void clear(); //������뻺����
	MString intToStr(int num, int size,int base = 10);
	int strToInt(const char* str, int base = 10);

private:
	Console* console;
};

