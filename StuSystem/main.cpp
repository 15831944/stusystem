#include "Action.h"

int main() {
	//��ʼ������̨����
	Console cons;
	cons.init();
	
	//��ʼ��������
	MUI mui(&cons);

	//��ȡ�û�����
	UserInput input(&cons);
	
	system("cls");
	//��������
	loadingData(cons);
	

	while (true) {
		mui.showMainInit();
		control(input, mui, cons);
		system("pause");
	}

	return 0;
}