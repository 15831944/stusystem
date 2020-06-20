#include "Console.h"

Console::Console(){
	this->hIn = GetStdHandle(STD_INPUT_HANDLE);		//��׼�����豸���
	this->hOut = GetStdHandle(STD_OUTPUT_HANDLE);	//��׼����豸���
	this->hWnd = GetConsoleWindow();//����̨���ھ��
}

Console::~Console(){
}


/**
* author:  ��ǿ
* methodsName: setCursorPos
* description: ���ÿ���̨���λ��
* param:  x	��λ��
* param:  y	��λ��
* return: ��
*/
void Console::setCursorPos(int x, int y) {
	this->coord.X = x;
	this->coord.Y = y;
	//���ÿ���̨���λ��
	SetConsoleCursorPosition(this->hOut, this->coord);
}

/**
* author:  ��ǿ
* methodsName: getCursorPos
* description: ��ȡ��ǰ����̨���λ��
* param:  ��
* return: ��
*/
void Console::cursorPosMigration(int x, int y) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(this->hOut, &csbi); //��ÿ���̨��Ϣ
	coord.X = csbi.dwCursorPosition.X + x;
	coord.Y = csbi.dwCursorPosition.Y + y;
	SetConsoleCursorPosition(this->hOut, coord);
}

COORD Console::getCurPos() const{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD crd;
	GetConsoleScreenBufferInfo(this->hOut, &csbi); //��ÿ���̨��Ϣ
	crd.X = csbi.dwCursorPosition.X;
	crd.Y = csbi.dwCursorPosition.Y;
	return crd;
}

/**
* author:  ��ǿ
* methodsName: setColor
* description: ��������ͱ�����ɫ
* param:  fontColor ������ɫ 0-F
* param:  backGroundColor ������ɫ 0-F
* return: ��
*/
void Console::setColor(int fontColor, int backGroundColor) {
	SetConsoleTextAttribute(this->hOut, fontColor + backGroundColor * 0x10);
}
//��ʼ������̨��Ϣ
void Console::init() {
	this->setColor();
	this->setConsolePosSize(1, 1, 100, 30); //��ʼ������̨����λ�úʹ�С
	this->setConsoleBufferSize(100, 500);    //��ʼ������̨���ڻ�������С
	this->setConsoleTitle("ѧ������ϵͳ");		//��ʼ������̨���ڱ���
	this->removeConsoleMode(ENABLE_QUICK_EDIT_MODE
		| ENABLE_INSERT_MODE
		| ENABLE_MOUSE_INPUT);//�Ƴ����ٱ༭��������������ģʽ
	//this->enableCursor(false); //�Ƿ���ʾ���
	this->removeConsoleStyle(WS_MINIMIZEBOX
		| WS_THICKFRAME
		| WS_MAXIMIZEBOX);//�Ƴ���󻯡���С���Ϳ����촰�ڵķ��
}

//�Ƿ���ʾ����̨���
void Console::enableCursor(bool bl, int dw) {
	CONSOLE_CURSOR_INFO curInfo; //����̨�����Ϣ
	curInfo.dwSize = dw;
	curInfo.bVisible = bl;
	SetConsoleCursorInfo(hOut, &curInfo); //winApi ���ÿ���̨�����Ϣ
}

//���ÿ���̨����
void Console::setConsoleTitle(const char* str) {
	//���ÿ���̨����
	SetConsoleTitle(TEXT(str));
}

//���ÿ���̨��������С
void Console::setConsoleBufferSize(SHORT width, SHORT height) {
	COORD size;	//����̨���ڻ�������С
	size = { width,height };
	SetConsoleScreenBufferSize(this->hOut, size);  //winApi���ÿ���̨��������С
}

//���ÿ���̨����λ�úʹ�С
void Console::setConsolePosSize(SHORT left, SHORT top, SHORT right, SHORT bottom) {
	SMALL_RECT rc;	 //����̨����λ�úʹ�С
	rc = { left, top, right, bottom };
	SetConsoleWindowInfo(this->hOut, 1, &rc);  //winApi���ÿ���̨������Ϣ
}

/*
* ENABLE_QUICK_EDIT_MODE  ���ٱ༭ģʽ
* ENABLE_INSERT_MODE	  ����ģʽ
* ENABLE_MOUSE_INPUT	  �������
* �Ƴ�����̨ģʽ
*/
void Console::removeConsoleMode(DWORD m) {
	DWORD mode;	//����̨����ģʽ
	GetConsoleMode(hIn, &mode);
	mode &= ~(m);  //�Ƴ����ٱ༭ģʽ
	SetConsoleMode(hIn, mode);
}
/*
* WS_MINIMIZEBOX	ȥ����С����ť
* WS_MAXIMIZEBOX	ȥ����󻯰�ť
* WS_THICKFRAME		ȥ�������촰��
* �Ƴ�����̨���ڷ��
*/
void Console::removeConsoleStyle(LONG lo) {
	LONG style = GetWindowLong(this->hWnd, GWL_STYLE); //��ȡ���ڷ��
	style &= ~(lo);	//ȥ�����
	SetWindowLong(this->hWnd, GWL_STYLE, style); // ���ô��ڷ��
}

//��ȡ���ھ��
HWND& Console::getHwnd() {
	return this->hWnd;
}

//������̨
void Console::fillConsole(SHORT x, SHORT y, int color, DWORD len) {
	this->coord.X = x;
	this->coord.Y = y;
	DWORD tmp;
	FillConsoleOutputAttribute(this->hOut, color, len, this->coord, &tmp);
}
