#include "MUI.h"

MUI::MUI(Console* console){
	this->console = console;
}

MUI::~MUI(){
	COORD tmp = this->console->getCurPos();
	this->console->setCursorPos(0,tmp.Y+5);
}

void MUI::setSet(int width, int height, int x, int y){
	for (int i = 0;i < height; i++) {
		this->console->fillConsole(x, y + i, 0, width);
	}
	int color = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED;
	for (int i = 0;i < height; i++) {
		this->console->fillConsole(x - 1, y - 1 + i, color, width);
	}
}

void MUI::showMainInit(){
	system("cls");
	int x = 25, y = 4;
	//���ñ���
	this->setSet(50, 11, x, y);
	this->console->setColor(0, 15);
	this->console->setCursorPos(x+8, y);
	cout << "      ��������������������" ;this->console->cursorPosMigration(-26, 1);
	cout << "      �T  ѧ������ϵͳ  �T" ;this->console->cursorPosMigration(-26, 1);
	cout << "      �T�T�T�T�T�T�T�T�T�T" ;this->console->cursorPosMigration(-26, 1);
	cout << "�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[" ;this->console->cursorPosMigration(-32, 1);
	cout << "�U    1. ���       2.ɾ��    �U" ;this->console->cursorPosMigration(-32, 1);
	cout << "�U    3. �޸�       4.��ѯ    �U" ;this->console->cursorPosMigration(-32, 1);
	cout << "�U    5. ����       0.�˳�    �U" ;this->console->cursorPosMigration(-32, 1);
	cout << "�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a" ;this->console->cursorPosMigration(-32, 1);
	cout << "ѡ��˵�: ";
	this->console->setColor();
}

void MUI::showTitleUi(const char* str){
	cout << endl << endl;
	COORD coord = this->console->getCurPos();
	int x = coord.X + 5;
	int y = coord.Y + 1;
	this->setSet(10, 3, x+1, y+1);
	this->console->setColor(13, 15);
	this->console->setCursorPos(x+1, y+1);
	cout << str << " ��" << endl << endl << endl;
	this->console->setColor();
}

void MUI::chooseUi(){
	COORD pos = this->console->getCurPos();
	this->setSet(50, 6, pos.X+1, pos.Y);
	this->console->setColor(0, 15);
	cout << "   �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�["<<endl;
	cout << "   �U    1. ѧ��       2.����       0�˳�    �U" << endl;
	cout << "   �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a" << endl;
	cout << "   ѡ���������: ";
	this->console->setColor();
}

void MUI::showDataTitle(){
	cout << "���������������Щ����������������������������Щ������������������Щ�������" << endl;
	cout << "�� " << setw(10) << setiosflags(ios::left) << "ѧ��" << " �� " << setw(26) << "����" << " �� " << setw(16) << "��������" << " �� " << "�Ա�" << " ��" << endl;
	cout << "���������������ة����������������������������ة������������������ة�������" << endl;
}

void MUI::showData(MString& data){
	char id[16] = { 0 };
	char name[26] = { 0 };
	char date[16] = { 0 };
	char sex;
	sscanf(data.getStr(), "%s \"%[^\"]\" %s %c", id, name, date, &sex);
	cout << "���������������Щ����������������������������Щ������������������Щ�������" << endl;
	cout << "�� " << setw(10) << setiosflags(ios::left) << id << " �� " << setw(26) << name << " �� " << setw(16) << date <<" ��  "<< setw(2) << sex <<"  ��"<< endl;
	cout << "���������������ة����������������������������ة������������������ة�������" << endl;
}


//vothis->console->setColor(0, 15);id MUI::showAddUi(){
//	int width = 80;
//	int height = 6;
//	int x = 10;
//	int y = 18;
//	for (int i = 0;i < height; i++) {
//		this->console->fillConsole(x, y + i, 0, width);
//	}
//	int color = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED;
//	for (int i = 0;i < height; i++) {
//		this->console->fillConsole(x - 1, y-1 + i, color, width);
//	}
//	this->console->setCursorPos(x+1, y);
//	this->console->setColor(13,15);
//	cout << "��� ��" << endl;this->console->cursorPosMigration(x+3, 0);
//	this->console->setColor(0, 15);
//	cout << "���������������������������������Щ������������������������Щ�����������" << endl;this->console->cursorPosMigration(x + 3, 0);
//	cout << "������:                         ����������:               �� �Ա�:    ��" << endl;this->console->cursorPosMigration(x + 3, 0);
//	cout << "���������������������������������ة������������������������ة�����������" << endl;
//	this->console->cursorPosMigration(x + 10, -2);
//	this->console->setColor();
//}
