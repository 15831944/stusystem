#pragma once
#include <windows.h>
class Console
{
public:
	Console();
	~Console();
public:
	void init();
	void enableCursor(bool bl, int dw = 1);	//是否显示光标
	void setConsoleTitle(const char* str);	//设置控制台标题
	void setConsoleBufferSize(SHORT width, SHORT height);	//设置控制台缓冲区大小
	void setConsolePosSize(SHORT x, SHORT y, SHORT width, SHORT height);	//设置控制台位置和大小
	void removeConsoleMode(DWORD m); //移除控制台模式
	void removeConsoleStyle(LONG lo);//移除控制台窗口风格
	void setColor(int fontColor = 7, int backGroundColor = 9); //设置颜色
	void setCursorPos(int x, int y);
	void cursorPosMigration(int x, int y);
	COORD getCurPos() const;
	HWND& getHwnd();	//获取窗口句柄
	void fillConsole(SHORT x, SHORT y, int color, DWORD len);
private:
	HANDLE hIn;	    //标准输入设备句柄
	HANDLE hOut;    //标准输出设备句柄
	COORD coord;	//保存光标位置
	HWND hWnd;		//控制台窗口句柄
};



