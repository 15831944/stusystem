#pragma once
#include <windows.h>
class Console
{
public:
	Console();
	~Console();
public:
	void init();
	void enableCursor(bool bl, int dw = 1);	//�Ƿ���ʾ���
	void setConsoleTitle(const char* str);	//���ÿ���̨����
	void setConsoleBufferSize(SHORT width, SHORT height);	//���ÿ���̨��������С
	void setConsolePosSize(SHORT x, SHORT y, SHORT width, SHORT height);	//���ÿ���̨λ�úʹ�С
	void removeConsoleMode(DWORD m); //�Ƴ�����̨ģʽ
	void removeConsoleStyle(LONG lo);//�Ƴ�����̨���ڷ��
	void setColor(int fontColor = 7, int backGroundColor = 9); //������ɫ
	void setCursorPos(int x, int y);
	void cursorPosMigration(int x, int y);
	COORD getCurPos() const;
	HWND& getHwnd();	//��ȡ���ھ��
	void fillConsole(SHORT x, SHORT y, int color, DWORD len);
private:
	HANDLE hIn;	    //��׼�����豸���
	HANDLE hOut;    //��׼����豸���
	COORD coord;	//������λ��
	HWND hWnd;		//����̨���ھ��
};



