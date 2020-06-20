#include "UserInput.h"

UserInput::UserInput(Console* console){
	this->console = console;
}

char UserInput::inputCh(){
	this->clear();
	char ch = '\0';
	ch = getch();
	this->console->setColor(0, 15);
	putchar(ch);
	this->console->setColor();
	return ch;
}

MString UserInput::inputStr(int size){
	this->clear();
	char sz[1024] = { 0 };
	std::cin.getline(sz, size);
	if (strlen(sz) == 0 || sz[0] == ' ') {
		sz[0] = '-';
		sz[1] = '1';
	}
	return std::move(MString(sz));
}

//Çå¿ÕÊäÈë»º´æÇø
void UserInput::clear() {
	std::cin.clear();
	rewind(stdin);
}

MString UserInput::intToStr(int num, int size, int base){
	MString tmp(size);
	_itoa(num, tmp.getStr(), base);
	return std::move(tmp);
}

int UserInput::strToInt(const char* str, int base){
	return strtol(str, nullptr, base);
}

