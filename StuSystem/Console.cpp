#include "Console.h"

Console::Console(){
	this->hIn = GetStdHandle(STD_INPUT_HANDLE);		//标准输入设备句柄
	this->hOut = GetStdHandle(STD_OUTPUT_HANDLE);	//标准输出设备句柄
	this->hWnd = GetConsoleWindow();//控制台窗口句柄
}

Console::~Console(){
}


/**
* author:  吴强
* methodsName: setCursorPos
* description: 设置控制台光标位置
* param:  x	列位置
* param:  y	行位置
* return: 无
*/
void Console::setCursorPos(int x, int y) {
	this->coord.X = x;
	this->coord.Y = y;
	//设置控制台光标位置
	SetConsoleCursorPosition(this->hOut, this->coord);
}

/**
* author:  吴强
* methodsName: getCursorPos
* description: 获取当前控制台光标位置
* param:  无
* return: 无
*/
void Console::cursorPosMigration(int x, int y) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(this->hOut, &csbi); //获得控制台信息
	coord.X = csbi.dwCursorPosition.X + x;
	coord.Y = csbi.dwCursorPosition.Y + y;
	SetConsoleCursorPosition(this->hOut, coord);
}

COORD Console::getCurPos() const{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD crd;
	GetConsoleScreenBufferInfo(this->hOut, &csbi); //获得控制台信息
	crd.X = csbi.dwCursorPosition.X;
	crd.Y = csbi.dwCursorPosition.Y;
	return crd;
}

/**
* author:  吴强
* methodsName: setColor
* description: 设置字体和背景颜色
* param:  fontColor 字体颜色 0-F
* param:  backGroundColor 背景颜色 0-F
* return: 无
*/
void Console::setColor(int fontColor, int backGroundColor) {
	SetConsoleTextAttribute(this->hOut, fontColor + backGroundColor * 0x10);
}
//初始化控制台信息
void Console::init() {
	this->setColor();
	this->setConsolePosSize(1, 1, 100, 30); //初始化控制台窗口位置和大小
	this->setConsoleBufferSize(100, 500);    //初始化控制台窗口缓冲区大小
	this->setConsoleTitle("学生管理系统");		//初始化控制台窗口标题
	this->removeConsoleMode(ENABLE_QUICK_EDIT_MODE
		| ENABLE_INSERT_MODE
		| ENABLE_MOUSE_INPUT);//移除快速编辑、插入和鼠标输入模式
	//this->enableCursor(false); //是否显示光标
	this->removeConsoleStyle(WS_MINIMIZEBOX
		| WS_THICKFRAME
		| WS_MAXIMIZEBOX);//移除最大化、最小化和可拉伸窗口的风格
}

//是否显示控制台光标
void Console::enableCursor(bool bl, int dw) {
	CONSOLE_CURSOR_INFO curInfo; //控制台光标信息
	curInfo.dwSize = dw;
	curInfo.bVisible = bl;
	SetConsoleCursorInfo(hOut, &curInfo); //winApi 设置控制台光标信息
}

//设置控制台标题
void Console::setConsoleTitle(const char* str) {
	//设置控制台标题
	SetConsoleTitle(TEXT(str));
}

//设置控制台缓存区大小
void Console::setConsoleBufferSize(SHORT width, SHORT height) {
	COORD size;	//控制台窗口缓存区大小
	size = { width,height };
	SetConsoleScreenBufferSize(this->hOut, size);  //winApi设置控制台缓存区大小
}

//设置控制台窗口位置和大小
void Console::setConsolePosSize(SHORT left, SHORT top, SHORT right, SHORT bottom) {
	SMALL_RECT rc;	 //控制台窗口位置和大小
	rc = { left, top, right, bottom };
	SetConsoleWindowInfo(this->hOut, 1, &rc);  //winApi设置控制台窗口信息
}

/*
* ENABLE_QUICK_EDIT_MODE  快速编辑模式
* ENABLE_INSERT_MODE	  插入模式
* ENABLE_MOUSE_INPUT	  鼠标输入
* 移除控制台模式
*/
void Console::removeConsoleMode(DWORD m) {
	DWORD mode;	//控制台窗口模式
	GetConsoleMode(hIn, &mode);
	mode &= ~(m);  //移除快速编辑模式
	SetConsoleMode(hIn, mode);
}
/*
* WS_MINIMIZEBOX	去除最小化按钮
* WS_MAXIMIZEBOX	去除最大化按钮
* WS_THICKFRAME		去除可拉伸窗口
* 移除控制台窗口风格
*/
void Console::removeConsoleStyle(LONG lo) {
	LONG style = GetWindowLong(this->hWnd, GWL_STYLE); //获取窗口风格
	style &= ~(lo);	//去除风格
	SetWindowLong(this->hWnd, GWL_STYLE, style); // 设置窗口风格
}

//获取窗口句柄
HWND& Console::getHwnd() {
	return this->hWnd;
}

//填充控制台
void Console::fillConsole(SHORT x, SHORT y, int color, DWORD len) {
	this->coord.X = x;
	this->coord.Y = y;
	DWORD tmp;
	FillConsoleOutputAttribute(this->hOut, color, len, this->coord, &tmp);
}
